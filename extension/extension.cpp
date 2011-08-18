/**
 * vim: set ts=4 sw=4 :
 * =============================================================================
 * SourceMod Sample Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id: extension.cpp 249 2008-08-27 21:48:35Z pred $
 */

#include "extension.h"
#include <mathlib.h>
#include "sh_list.h"
#include "datamap.h"
#include "const.h"
#include "bspflags.h"
#include "convar.h"
#include "filesystem.h"
#include "CEntityManager.h"

#include "CEntity.h"

#include "CDetour/detours.h"

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

Sidewinder g_Sidewinder;

SMEXT_LINK(&g_Sidewinder);

ICvar *icvar = NULL;

IGameConfig *g_pGameConf = NULL;

IServerGameEnts *gameents;
INetworkStringTableContainer *netstringtables;
IEngineSound *engsound;
IEngineTrace *enginetrace;
IServerGameClients *gameclients;

CSharedEdictChangeInfo *g_pSharedChangeInfo = NULL;

CGlobalVars *gpGlobals;

ConVar DodgeballVersion("tfdodgeball_version", SMEXT_CONF_VERSION, FCVAR_SPONLY|FCVAR_REPLICATED|FCVAR_NOTIFY, "TFDodgeball Version");

#ifndef WIN32
#include <execinfo.h>
static void EnabledHook(IConVar *var, const char *pOldValue, float flOldValue)
{
	void *trace[32];
	char **messages = (char **)NULL;
	int i, trace_size = 0;

	trace_size = backtrace(trace, 32);
	messages = backtrace_symbols(trace, trace_size);
	g_pSM->LogError(myself, "sm_dodgeball_enabled cvar changed (%s -> %s):\n", pOldValue, ConVarRef(var).GetString());
	for (i = 0; i < trace_size; ++i)
		g_pSM->LogError(myself, "> [%d] %s\n", i, messages[i]);
}
ConVar DodgeballEnabled("sm_dodgeball_enabled", "1", FCVAR_NONE, "", true, 0.0, true, 1.0, EnabledHook);
#else
ConVar DodgeballEnabled("sm_dodgeball_enabled", "1", FCVAR_NONE, "", true, 0.0, true, 1.0);
#endif

void *g_EntList = NULL;
int gMaxClients = 0;
int gCmdIndex;

/** hooks **/
SH_DECL_HOOK3_void(IServerGameDLL, ServerActivate, SH_NOATTRIB, 0, edict_t *, int, int)
SH_DECL_HOOK1_void(IServerGameClients, SetCommandClient, SH_NOATTRIB, 0, int);

bool Sidewinder::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	char conf_error[255] = "";
	if (!gameconfs->LoadGameConfigFile("tfdodgeball.games", &g_pGameConf, conf_error, sizeof(conf_error)))
	{
		if (conf_error[0])
		{
			g_pSM->Format(error, maxlength, "Could not read tfdodgeball.games: %s", conf_error);
		}
		return false;
	}

	if (!GetEntityManager()->Init(g_pGameConf))
		g_pSM->LogError(myself, "CEntity failed to init.");

	g_EntList = gamehelpers->GetGlobalEntityList();

	return true;
}

bool Sidewinder::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	GET_V_IFACE_CURRENT(GetServerFactory, gameents, IServerGameEnts, INTERFACEVERSION_SERVERGAMEENTS);
	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, netstringtables, INetworkStringTableContainer, INTERFACENAME_NETWORKSTRINGTABLESERVER);
	GET_V_IFACE_CURRENT(GetEngineFactory, engsound, IEngineSound, IENGINESOUND_SERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, enginetrace, IEngineTrace, INTERFACEVERSION_ENGINETRACE_SERVER);
	GET_V_IFACE_CURRENT(GetServerFactory, gameclients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS);

	gpGlobals = ismm->GetCGlobals();
	g_pCVar = icvar;
	
	ConVar_Register(0, this);

	g_pSharedChangeInfo = engine->GetSharedEdictChangeInfo();

	// add standard plugin hooks
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, gamedll, &g_Sidewinder, &Sidewinder::ServerActivate, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, SetCommandClient, serverclients, &g_Sidewinder, &Sidewinder::SetCommandClient, true);

	return true;
}

bool Sidewinder::RegisterConCommandBase(ConCommandBase *pCommand)
{
	META_REGCVAR(pCommand);

	return true;
}

bool Sidewinder::SDK_OnMetamodUnload(char *error, size_t maxlength)
{
	// remove standard plugin hooks
	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, gamedll, &g_Sidewinder, &Sidewinder::ServerActivate, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, SetCommandClient, serverclients, &g_Sidewinder, &Sidewinder::SetCommandClient, true);
	return true;
}

// Stupid shim required for CBaseEdict::StateChanged to work
IChangeInfoAccessor *CBaseEdict::GetChangeAccessor()
{
	return engine->GetChangeAccessor( (const edict_t *)this );
}

// *************************************************
// Standard MM:S Plugin Hooks
void Sidewinder::ServerActivate(edict_t *pEdictList, int edictCount, int clientMax)
{
	gMaxClients = clientMax;
	RETURN_META(MRES_IGNORED);
}

void Sidewinder::SetCommandClient( int cmd )
{
	gCmdIndex = cmd + 1; //offset by -1 due to using the engine's client index.
}
