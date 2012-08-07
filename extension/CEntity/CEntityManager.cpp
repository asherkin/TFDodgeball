/**
* =============================================================================
* CEntity Entity Handling Framework
* Copyright (C) 2011 Matt Woodrow.  All rights reserved.
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
*/

#include "CEntityManager.h"
#include "sourcehook.h"
#include "IEntityFactory.h"
#include "ehandle.h"
class CBaseEntity;
typedef CHandle<CBaseEntity> EHANDLE;
#include "takedamageinfo.h"
#include "server_class.h"
#include "CEntity.h"
#include "shareddefs.h"
#include "usercmd.h"
//#ifdef WIN32
#include "rtti.h"
//#endif
#include "../CDetour/detours.h"


SH_DECL_HOOK1(IEntityFactoryDictionary, Create, SH_NOATTRIB, 0, IServerNetworkable *, const char *);
SH_DECL_HOOK1_void(IVEngineServer, RemoveEdict, SH_NOATTRIB, 0, edict_t *);

IEntityFactoryReal *IEntityFactoryReal::m_Head;

EntityFactoryDictionaryCall EntityFactoryDictionary = NULL;

CEntityManager *GetEntityManager()
{
	static CEntityManager *entityManager = new CEntityManager();
	return entityManager;
}

CEntityManager::CEntityManager()
{
	m_bEnabled = false;
}

bool CEntityManager::Init(IGameConfig *pConfig)
{
	/* Find the IEntityFactoryDictionary* */
	void *addr;
	if (!pConfig->GetMemSig("EntityFactory", &addr) || addr == NULL)
	{
		g_pSM->LogError(myself, "[CENTITY] Couldn't find sig: %s.", "EntityFactory");
		return false;
	}

	EntityFactoryDictionary = (EntityFactoryDictionaryCall)addr;
	pDict = EntityFactoryDictionary();

	IEntityFactoryReal *pList = IEntityFactoryReal::m_Head;
	while (pList)
	{
		pList->AddToList();
		pList = pList->m_Next;
	}

	if (!pConfig->GetMemSig("FireOutput", &addr) || addr == NULL)
	{
		g_pSM->LogError(myself, "[CENTITY] Couldn't find sig: %s.", "FireOutput");
		return false;
	}

	FireOutputFunc= (FireOutputFuncType)addr;

	if (!pConfig->GetMemSig("PhysIsInCallback", &addr) || addr == NULL)
	{
		g_pSM->LogError(myself, "[CENTITY] Couldn't find sig: %s.", "PhysIsInCallback");
		return false;
	}

	PhysIsInCallback = (PhysIsInCallbackFuncType)addr;

	/* Reconfigure all the hooks */
	IHookTracker *pTracker = IHookTracker::m_Head;
	while (pTracker)
	{
		pTracker->ReconfigureHook(pConfig);
		pTracker = pTracker->m_Next;
	}

	CDetourManager::Init(g_pSM->GetScriptingEngine(), pConfig);

	IDetourTracker *pDetourTracker = IDetourTracker::m_Head;
	while (pDetourTracker)
	{
		pDetourTracker->AddHook();
		pDetourTracker = pDetourTracker->m_Next;
	}

	ISigOffsetTracker *pSigOffsetTracker = ISigOffsetTracker::m_Head;
	while (pSigOffsetTracker)
	{
		pSigOffsetTracker->FindSig(pConfig);
		pSigOffsetTracker = pSigOffsetTracker->m_Next;
	}

	srand(time(NULL));
	m_bEnabled = true;

	/* Find any already existing entities */
	for (int i = 0; i < gpGlobals->maxEntities; i++)
	{
		edict_t *pEntity = gamehelpers->EdictOfIndex(i);
		if (!pEntity || pEntity->IsFree())
		{
			continue;
		}

		IServerNetworkable *pNetworkable = pEntity->GetNetworkable();
		if (!pNetworkable)
		{
			continue;
		}

		HookEntity(pNetworkable);
	}

	/* Start the creation hooks! */
	SH_ADD_HOOK(IEntityFactoryDictionary, Create, pDict, SH_MEMBER(this, &CEntityManager::Create_Hook), true);
	SH_ADD_HOOK(IVEngineServer, RemoveEdict, engine, SH_MEMBER(this, &CEntityManager::RemoveEdict_Hook), true);
	return true;
}

void CEntityManager::Shutdown()
{
	SH_REMOVE_HOOK(IEntityFactoryDictionary, Create, pDict, SH_MEMBER(this, &CEntityManager::Create_Hook), true);
	SH_REMOVE_HOOK(IVEngineServer, RemoveEdict, engine, SH_MEMBER(this, &CEntityManager::RemoveEdict_Hook), true);

	IDetourTracker *pDetourTracker = IDetourTracker::m_Head;
	while (pDetourTracker)
	{
		pDetourTracker->RemoveHook();
		pDetourTracker = pDetourTracker->m_Next;
	}
}

void CEntityManager::LinkEntityToClass(IEntityFactory *pFactory, const char *className, bool internalClass)
{
	assert(pFactory);
	if (internalClass)
	{
		pInternalFactoryTrie.insert(className, pFactory);
	} else {
		pFactoryTrie.insert(className, pFactory);
	}
}

void CEntityManager::LinkEntityToClass(IEntityFactory *pFactory, const char *className, const char *replaceName)
{
	assert(pFactory);
	LinkEntityToClass(pFactory, className);
	pSwapTrie.insert(className, replaceName);
}

void CEntityManager::HookEntity(IServerNetworkable *pNetworkable, const char *pClassName)
{
	if (!pNetworkable)
	{
		return;
	}

	if (!pClassName)
	{
		pClassName = pNetworkable->GetClassName();
	}

	CBaseEntity *pEnt = pNetworkable->GetBaseEntity();
	edict_t *pEdict = pNetworkable->GetEdict();

	if (!pEdict || !pEnt)
	{
		return;
	}

	IEntityFactory **value = NULL;
	value = FindFactoryInTrie(&pFactoryTrie, pEnt, pClassName);

	if (!value)
	{
		/* Look for CEntity handlers */
		value = FindFactoryInTrie(&pInternalFactoryTrie, pEnt, pClassName);
	}

	if (!value)
	{
		/* No handler for this entity (not an entity?) */
		//value = pFactoryTrie.retrieve("CBaseEntity");
		g_pSM->LogError(myself, "No handler found for %d/%s", gamehelpers->EntityToBCompatRef(pEnt), pClassName);
		//PrintTypeTree(pEnt);
		//_asm int 3;
		return;
	}

	IEntityFactory *pFactory = *value;
	assert(pFactory);

	char vtable[20];
	_snprintf(vtable, sizeof(vtable), "%x", (unsigned int) *(void **)pEnt);

	CEntity *pEntity = pFactory->Create(pEdict, pEnt);

	pEntity->ClearFlags();

	pEntity->InitProps();

	if (!pHookedTrie.retrieve(vtable))
	{
		pEntity->InitDataMap();
		pEntity->InitHooks();

		pHookedTrie.insert(vtable, true);
	}

	// This causes issues with other things reading the classname, 
	// likely due to not implementing string_t correctly.
	// 
	// Just completely ignore for now, this means that custom ents
	// will have the wrong classname.
	// 
	// ^ Scratch that, it broke tfdb_nukeskin
	// 
	// Only set it if custom, for now.
	if (strcmp(pEntity->GetClassname(), pClassName) != 0)
		pEntity->SetClassname(pClassName);
}

IServerNetworkable *CEntityManager::Create_Hook(const char *pClassName)
{
	IServerNetworkable *pNetworkable = META_RESULT_ORIG_RET(IServerNetworkable *);

	if (!pNetworkable)
	{
		return NULL;
	}

	HookEntity(pNetworkable, pClassName);

	RETURN_META_VALUE(MRES_IGNORED, NULL);
}

void CEntityManager::RemoveEdict_Hook(edict_t *e)
{
	CEntity *pEnt = CEntity::Instance(e);
	if (pEnt)
	{
		//g_pSM->LogMessage(myself, "Edict Removed, removing CEntity");
		pEnt->Destroy();
	}

	RETURN_META(MRES_IGNORED);
}

IEntityFactory **CEntityManager::FindFactoryInTrie(KTrie<IEntityFactory *> *pTrie, CBaseEntity *pEntity, const char *pClassName)
{
	IEntityFactory **value = NULL;
	value = pTrie->retrieve(pClassName); // Old style match to entity classname (also used for custom classes).

	if (!value)
	{
		/* Attempt to do an RTTI lookup for C++ class links */
		IType *pType = GetType(pEntity);
		IBaseType *pBase = pType->GetBaseType();

		do 
		{
			const char *classname = GetTypeName(pBase->GetTypeInfo());
			value = pTrie->retrieve(classname);

			if (value)
			{
				break;
			}

		} while (pBase->GetNumBaseClasses() && (pBase = pBase->GetBaseClass(0)));

		pType->Destroy();
	}

	return value;
}
