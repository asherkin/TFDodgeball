/**
* vim: set ts=4 :
* =============================================================================
* CEntity Entity Handling Framework
* Copyright (C) 2009 Matt Woodrow.  All rights reserved.
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
#include "../game/shared/ehandle.h"
class CBaseEntity;
typedef CHandle<CBaseEntity> EHANDLE;
#include "../game/shared/takedamageinfo.h"
#include "server_class.h"
#include "CEntity.h"

SH_DECL_HOOK1(IEntityFactoryDictionary, Create, SH_NOATTRIB, 0, IServerNetworkable *, const char *);
SH_DECL_HOOK1_void(IVEngineServer, RemoveEdict, SH_NOATTRIB, 0, edict_t *);

SH_DECL_MANUALEXTERN0_void(UpdateOnRemove);
SH_DECL_MANUALEXTERN3_void(Teleport, const Vector *, const QAngle *, const Vector *);
SH_DECL_MANUALEXTERN0_void(Spawn);
SH_DECL_MANUALEXTERN1(OnTakeDamage, int, const CTakeDamageInfo &);
SH_DECL_MANUALEXTERN0_void(Think);
SH_DECL_MANUALEXTERN1_void(StartTouch, CBaseEntity *);
SH_DECL_MANUALEXTERN1_void(Touch, CBaseEntity *);
SH_DECL_MANUALEXTERN1_void(EndTouch, CBaseEntity *);

SH_DECL_MANUALEXTERN3(FVisible, bool, CBaseEntity *, int, CBaseEntity **);

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
		return false;
	}

	typedef IEntityFactoryDictionary *(*EntityFactoryDictionaryCall)();
	EntityFactoryDictionaryCall EntityFactoryDictionary = (EntityFactoryDictionaryCall)addr;
	pDict = EntityFactoryDictionary();

	/* Reconfigure all the hooks */
	RECONFIGURE_HOOK(Teleport);
	RECONFIGURE_HOOK(UpdateOnRemove);
	RECONFIGURE_HOOK(Spawn);
	RECONFIGURE_HOOK(OnTakeDamage);
	RECONFIGURE_HOOK(Think);
	RECONFIGURE_HOOK(StartTouch);
	RECONFIGURE_HOOK(Touch);
	RECONFIGURE_HOOK(EndTouch);
	RECONFIGURE_HOOK(FVisible);

	SH_DECL_MANUALEXTERN3(FVisible, bool, CBaseEntity *, int, CBaseEntity **);
	
	/* Start the creation hooks! */
	SH_ADD_HOOK(IEntityFactoryDictionary, Create, pDict, SH_MEMBER(this, &CEntityManager::Create), true);
	SH_ADD_HOOK(IVEngineServer, RemoveEdict, engine, SH_MEMBER(this, &CEntityManager::RemoveEdict), true);

	srand(time(NULL));

	m_bEnabled = true;
	return true;
}

void CEntityManager::Shutdown()
{
	SH_REMOVE_HOOK(IEntityFactoryDictionary, Create, pDict, SH_MEMBER(this, &CEntityManager::Create), true);
	SH_REMOVE_HOOK(IVEngineServer, RemoveEdict, engine, SH_MEMBER(this, &CEntityManager::RemoveEdict), true);
}

void CEntityManager::LinkEntityToClass(IEntityFactory *pFactory, const char *className)
{
	if (!pFactory)
	{
		return;
	}

	pFactoryTrie.insert(className, pFactory);
}

IServerNetworkable *CEntityManager::Create(const char *pClassName)
{
	IEntityFactory **value = pFactoryTrie.retrieve(pClassName);
	
	if (!value)
	{
		/* No specific handler for this entity */
		value = pFactoryTrie.retrieve("baseentity");
		assert(value);
	}

	IEntityFactory *pFactory = *value;
	assert(pFactory);

	IServerNetworkable *pNetworkable = META_RESULT_ORIG_RET(IServerNetworkable *);

	if (pNetworkable != NULL)
	{
		//const char *serverName = pNetworkable->GetServerClass()->GetName();
		edict_t *pEdict = pNetworkable->GetEdict();
		CBaseEntity *pEnt = pNetworkable->GetBaseEntity();

		if (!pEdict || !pEnt)
		{
			return NULL;
		}
	
		char vtable[20];
		_snprintf(vtable, sizeof(vtable), "%x", (unsigned int) *(void **)pEnt);

		if (pHookedTrie.retrieve(vtable))
		{
			pFactory->Create(pEdict, pEnt, false);
		}
		else
		{
			pFactory->Create(pEdict, pEnt, true);
			pHookedTrie.insert(vtable, true);
		}
	}

	return NULL;
}

void CEntityManager::RemoveEdict(edict_t *e)
{
	CEntity *pEnt = CEntity::Instance(e);
	if (pEnt)
	{
		pEnt->Destroy();
	}
}
