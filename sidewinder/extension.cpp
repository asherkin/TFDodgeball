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

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

Sidewinder g_Sidewinder;

SMEXT_LINK(&g_Sidewinder);

ICvar *icvar = NULL;

IGameConfig *g_pGameConf = NULL;

IServerGameEnts *gameents;

CGlobalVars *gpGlobals;

ConVar SideWinderVersion("sidewinder_version", "3.0.0", FCVAR_SPONLY|FCVAR_REPLICATED|FCVAR_NOTIFY, "Sidewinder Version");

void *g_EntList = NULL;
int gMaxClients = 0;

/** hooks **/
SH_DECL_HOOK3_void(IServerGameDLL, ServerActivate, SH_NOATTRIB, 0, edict_t *, int, int)

bool Sidewinder::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	char conf_error[255] = "";
	if (!gameconfs->LoadGameConfigFile("centity.offsets", &g_pGameConf, conf_error, sizeof(conf_error)))
	{
		if (conf_error[0])
		{
			g_pSM->Format(error, maxlength, "Could not read centity.offsets.txt: %s", conf_error);
		}
		return false;
	}

	GetEntityManager()->Init(g_pGameConf);

	return true;
}

bool Sidewinder::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	GET_V_IFACE_CURRENT(GetServerFactory, gameents, IServerGameEnts, INTERFACEVERSION_SERVERGAMEENTS);
	gpGlobals = ismm->GetCGlobals();

	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);

	g_pCVar = icvar;
	
	ConVar_Register(0, this);

	// add standard plugin hooks
	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, gamedll, &g_Sidewinder, &Sidewinder::ServerActivate, true);

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
	return true;
}

// *************************************************
// Standard MM:S Plugin Hooks
void Sidewinder::ServerActivate(edict_t *pEdictList, int edictCount, int clientMax)
{
	gMaxClients = clientMax;
	RETURN_META(MRES_IGNORED);
}