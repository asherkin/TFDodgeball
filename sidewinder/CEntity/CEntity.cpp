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

#include "CEntity.h"
#include "../game/shared/shareddefs.h"
#include "CEntityManager.h"

SH_DECL_MANUALHOOK3_void(Teleport, 0, 0, 0, const Vector *, const QAngle *, const Vector *);
SH_DECL_MANUALHOOK0_void(UpdateOnRemove, 0, 0, 0);
SH_DECL_MANUALHOOK0_void(Spawn, 0, 0, 0);
SH_DECL_MANUALHOOK1(OnTakeDamage, 0, 0, 0, int, const CTakeDamageInfo &);
SH_DECL_MANUALHOOK0_void(Think, 0, 0, 0);
SH_DECL_MANUALHOOK1_void(StartTouch, 0, 0, 0, CBaseEntity *);
SH_DECL_MANUALHOOK1_void(Touch, 0, 0, 0, CBaseEntity *);
SH_DECL_MANUALHOOK1_void(EndTouch, 0, 0, 0, CBaseEntity *);

CEntity *pEntityData[MAX_EDICTS+1] = {NULL};

LINK_ENTITY_TO_CLASS(baseentity, CEntity);

void CEntity::Init(edict_t *pEdict, CBaseEntity *pBaseEntity, bool addHooks)
{
	m_pEntity = pBaseEntity;
	m_pEdict = pEdict;

	assert(!pEntityData[entindex()]);

	pEntityData[entindex()] = this;

	if(!m_pEntity || !m_pEdict)
		return;

	sm_sendprop_info_t info;
	GET_SENDPROP_POINTER(uint8_t, m_pEdict, BaseEntity(), &info, m_iTeamNum);
	GET_SENDPROP_POINTER(Vector, m_pEdict, BaseEntity(), &info, m_vecOrigin);
	GET_SENDPROP_POINTER(uint8_t, m_pEdict, BaseEntity(), &info, m_CollisionGroup);
	GET_SENDPROP_POINTER(CBaseHandle, m_pEdict, BaseEntity(), &info, m_hOwnerEntity);
	GET_SENDPROP_POINTER(uint16_t, m_pEdict, BaseEntity(), &info, m_fFlags);

	datamap_t *pMap = NULL;
	GET_DATAMAP_POINTER(Vector, BaseEntity(), pMap, m_vecAbsVelocity);
	//GET_DATAMAP_POINTER(BASEPTR, BaseEntity(), pMap, m_pfnThink);
	m_pfnThink = NULL;
	GET_DATAMAP_POINTER(int, BaseEntity(), pMap, m_nNextThinkTick);
	GET_DATAMAP_POINTER(string_t, BaseEntity(), pMap, m_iClassname);
	GET_DATAMAP_POINTER(matrix3x4_t, BaseEntity(), pMap, m_rgflCoordinateFrame);
	GET_DATAMAP_POINTER(Vector, BaseEntity(), pMap, m_vecVelocity);
	GET_DATAMAP_POINTER(CBaseHandle, BaseEntity(), pMap, m_hMoveParent);
	GET_DATAMAP_POINTER(int, BaseEntity(), pMap, m_iEFlags);

	ADD_DEFAULTHANDLER_HOOK(CEntity, Teleport);
	ADD_DEFAULTHANDLER_HOOK(CEntity, UpdateOnRemove);
	ADD_DEFAULTHANDLER_HOOK(CEntity, Spawn);
	ADD_DEFAULTHANDLER_HOOK(CEntity, OnTakeDamage);
	ADD_DEFAULTHANDLER_HOOK(CEntity, Think);
	ADD_DEFAULTHANDLER_HOOK(CEntity, StartTouch);
	ADD_DEFAULTHANDLER_HOOK(CEntity, Touch);
	ADD_DEFAULTHANDLER_HOOK(CEntity, EndTouch);
}

void CEntity::Destroy()
{
	pEntityData[entindex()] = NULL;
	delete this;
}

CBaseEntity * CEntity::BaseEntity()
{
	return m_pEntity;
}

/* Expanded handler for readability and since this one actually does something */
void CEntity::UpdateOnRemove()
{
	if (!m_bInUpdateOnRemove)
	{
		SH_MCALL(BaseEntity(), UpdateOnRemove);
		return;
	}

	SET_META_RESULT(MRES_IGNORED);

	SH_GLOB_SHPTR->DoRecall();
	SourceHook::EmptyClass *thisptr = reinterpret_cast<SourceHook::EmptyClass*>(SH_GLOB_SHPTR->GetIfacePtr());
	(thisptr->*(__SoureceHook_FHM_GetRecallMFPUpdateOnRemove(thisptr)))();

	SET_META_RESULT(MRES_SUPERCEDE);
}

void CEntity::InternalUpdateOnRemove()
{
	SET_META_RESULT(MRES_SUPERCEDE);

	CEntity *pEnt = CEntity::Instance(META_IFACEPTR(CBaseEntity));
	if (!pEnt)
	{
		RETURN_META(MRES_IGNORED);
	}

	pEnt->m_bInUpdateOnRemove = true;
	pEnt->UpdateOnRemove();
	pEnt->m_bInUpdateOnRemove = false;

	pEnt->Destroy();
}

DECLARE_DEFAULTHANDLER_void(CEntity, Teleport, (const Vector *origin, const QAngle* angles, const Vector *velocity), (origin, angles, velocity));
DECLARE_DEFAULTHANDLER_void(CEntity, Spawn, (), ());
DECLARE_DEFAULTHANDLER(CEntity, OnTakeDamage, int, (const CTakeDamageInfo &info), (info));
DECLARE_DEFAULTHANDLER_void(CEntity, StartTouch, (CBaseEntity *entity), (entity));
DECLARE_DEFAULTHANDLER_void(CEntity, Touch, (CBaseEntity *entity), (entity));
DECLARE_DEFAULTHANDLER_void(CEntity, EndTouch, (CBaseEntity *entity), (entity));

void CEntity::Think()
{
	if (m_pfnThink)
	{
		(this->*m_pfnThink)();
	}

	if (!m_bInThink)
	{
		SH_MCALL(BaseEntity(), Think)();
		return;
	}

	SET_META_RESULT(MRES_IGNORED);
	SH_GLOB_SHPTR->DoRecall();
	SourceHook::EmptyClass *thisptr = reinterpret_cast<SourceHook::EmptyClass*>(SH_GLOB_SHPTR->GetIfacePtr());
	(thisptr->*(__SoureceHook_FHM_GetRecallMFPThink(thisptr)))();
	SET_META_RESULT(MRES_SUPERCEDE);
}

void CEntity::InternalThink()
{
	SET_META_RESULT(MRES_SUPERCEDE);

	CEntity *pEnt = CEntity::Instance(META_IFACEPTR(CBaseEntity));
	if (!pEnt)
	{
		RETURN_META(MRES_IGNORED);
	}

	pEnt->m_bInThink = true;
	pEnt->Think();
	pEnt->m_bInThink = false;
}


BASEPTR	CEntity::ThinkSet(BASEPTR func, float thinkTime, const char *szContext)
{
	return m_pfnThink = func;
}

void CEntity::SetNextThink(float thinkTime, const char *szContext)
{
	int thinkTick = ( thinkTime == TICK_NEVER_THINK ) ? TICK_NEVER_THINK : TIME_TO_TICKS(thinkTime);

	// Are we currently in a think function with a context?
	if ( !szContext )
	{
		// Old system
		*m_nNextThinkTick = thinkTick;
		CheckHasThinkFunction( thinkTick == TICK_NEVER_THINK ? false : true );
		return;
	}
}

void CEntity::AddEFlags(int nEFlagMask)
{
	*m_iEFlags |= nEFlagMask;
}

void CEntity::RemoveEFlags(int nEFlagMask)
{
	*m_iEFlags &= ~nEFlagMask;
}

bool CEntity::IsEFlagSet(int nEFlagMask) const
{
	return (*m_iEFlags & nEFlagMask) != 0;
}

void CEntity::CheckHasThinkFunction(bool isThinking)
{
	if ( IsEFlagSet( EFL_NO_THINK_FUNCTION ) && isThinking )
	{
		RemoveEFlags( EFL_NO_THINK_FUNCTION );
	}
	else if ( !isThinking && !IsEFlagSet( EFL_NO_THINK_FUNCTION ) && !WillThink() )
	{
		AddEFlags( EFL_NO_THINK_FUNCTION );
	}
}

bool CEntity::WillThink()
{
	if (*m_nNextThinkTick > 0)
		return true;

	return false;
}

const char* CEntity::GetClassname()
{
	return STRING(*m_iClassname);
}

void CEntity::ChangeTeam(int iTeamNum)
{
	*m_iTeamNum = iTeamNum;
}

int CEntity::GetTeamNumber(void) const
{
	return *m_iTeamNum;
}

bool CEntity::InSameTeam(CEntity *pEntity) const
{
	if (!pEntity)
		return false;

	return (pEntity->GetTeamNumber() == GetTeamNumber());
}

const Vector& CEntity::GetLocalOrigin(void) const
{
	return *m_vecOrigin;
}

const Vector &CEntity::GetAbsVelocity() const
{
	if (IsEFlagSet(EFL_DIRTY_ABSVELOCITY))
	{
		//const_cast<CEntity*>(this)->CalcAbsoluteVelocity();
	}
	return *m_vecAbsVelocity;
}

CEntity *CEntity::GetMoveParent(void)
{
	return Instance(*m_hMoveParent); 
}

edict_t *CEntity::edict()
{
	return m_pEdict;
}

int CEntity::entindex()
{
	return engine->IndexOfEdict(edict());
}

CEntity *CEntity::Instance(CBaseEntity *pEnt)
{
	edict_t *pEdict = gameents->BaseEntityToEdict(META_IFACEPTR(CBaseEntity));
	
	if (!pEdict)
	{
		return NULL;
	}

	return Instance(pEdict);
}

CEntity *CEntity::Instance(int iEnt)
{
	return pEntityData[iEnt];
}

CEntity *CEntity::Instance(const edict_t *pEnt)
{
	return Instance(engine->IndexOfEdict(pEnt));
}

CEntity *CEntity::Instance(const CBaseHandle &hEnt)
{
	if (!hEnt.IsValid())
	{
		return NULL;
	}

	int index = hEnt.GetEntryIndex();

	edict_t *pStoredEdict;
	CBaseEntity *pStoredEntity;

	pStoredEdict = engine->PEntityOfEntIndex(index);
	if (!pStoredEdict || pStoredEdict->IsFree())
	{
		return NULL;
	}

	IServerUnknown *pUnk;
	if ((pUnk = pStoredEdict->GetUnknown()) == NULL)
	{
		return NULL;
	}

	pStoredEntity = pUnk->GetBaseEntity();

	if (pStoredEntity == NULL)
	{
		return NULL;
	}

	IServerEntity *pSE = pStoredEdict->GetIServerEntity();

	if (pSE == NULL)
	{
		return NULL;
	}

	if (pSE->GetRefEHandle() != hEnt)
	{
		return NULL;
	}

	return Instance(index);
}

CEntity *CEntity::Instance(edict_t *pEnt)
{
	return Instance(engine->IndexOfEdict(pEnt));
}

bool CEntity::IsPlayer()
{
	return false;
}

int CEntity::GetTeam()
{
	return *m_iTeamNum;
}
