#include "extension.h"
#include "CSentryRocket.h"
#include "CPlayer.h"
#include "worldsize.h"

SH_DECL_MANUALEXTERN3(FVisible, bool, CBaseEntity *, int, CBaseEntity **);

ConVar RocketSpeedMul("sm_sentryrocket_speedmul", "0.5", FCVAR_NONE);

void CTrackingProjectile::Init(edict_t *pEdict, CBaseEntity *pBaseEntity, bool addHooks)
{
	//m_bCritical = false;
	m_bHasThought = false;

	BaseClass::Init(pEdict, pBaseEntity, addHooks);

	ADD_DEFAULTHANDLER_HOOK(CTrackingProjectile, FVisible);

	sm_sendprop_info_t info;
	GET_SENDPROP_POINTER(bool, m_pEdict, BaseEntity(), &info, m_bCritical);
	GET_SENDPROP_POINTER(int, m_pEdict, BaseEntity(), &info, m_iDeflected);
	m_flDamage = (float *)((char *)BaseEntity() + (info.actual_offset+sizeof(int)));
}

void CTrackingProjectile::Spawn(void)
{
 	BaseClass::Spawn();

	//SetDamage(9000);

	SetThink(&CTrackingProjectile::FindThink);
	SetNextThink(gpGlobals->curtime); 
}

void CTrackingProjectile::FindThink(void)
{
	CEntity *pBestVictim = NULL;
	float flBestVictim = MAX_TRACE_LENGTH;
	float flVictimDist;

	CBaseEntity *pBaseEntity = NULL;

	if (!m_bHasThought)
	{
		m_bHasThought = true;
	}

	for (int index = 1; index <= gpGlobals->maxClients; index++)
	{
		CEntity *pEntity = CEntity::Instance(index);

		if (!IsValidTarget(pEntity))
		{
			continue;
		}

		flVictimDist = (GetLocalOrigin() - pEntity->GetLocalOrigin()).Length();

		//Find closest
		if (flVictimDist < flBestVictim)
		{
			pBestVictim = pEntity;
			flBestVictim = flVictimDist;
		}
	}

	if (pBestVictim == NULL) 
	{
		SetThink(&CTrackingProjectile::FindThink);
		SetNextThink(gpGlobals->curtime);
		return;
	}

	TurnToTarget(pBestVictim);

	m_currentTarget = pBestVictim->entindex();
	SetThink(&CTrackingProjectile::TrackThink);
	SetNextThink(gpGlobals->curtime);
	m_lastSearch = gpGlobals->curtime;
}

void CTrackingProjectile::TrackThink(void)
{
	CEntity *pVictim = CEntity::Instance(m_currentTarget);

	if (!IsValidTarget(pVictim))
	{
		/* This finds a new target and aims at it, or starts looping find until it does */
		FindThink();
		return;
	}

	TurnToTarget(pVictim);

	if (gpGlobals->curtime > 0.1 + m_lastSearch)
	{
		SetThink(&CTrackingProjectile::FindThink);
		SetNextThink(gpGlobals->curtime);	
	}
	else
	{
		SetThink(&CTrackingProjectile::TrackThink);
		SetNextThink(gpGlobals->curtime);
	}
}

bool CTrackingProjectile::IsValidTarget(CEntity *pEntity)
{
	if(!pEntity)
	{
		return false;
	}

	if(!pEntity->IsPlayer())
	{
		return false;
	}

	CPlayer *pPlayer = static_cast<CPlayer *>(pEntity);

	if(!pPlayer->IsAlive())
	{
		return false;
	}

	if (pPlayer->GetPlayerClass() == PLAYERCLASS_SPY) 
	{
		//Cloaky
		if (pPlayer->GetPlayerCond() & PLAYERCOND_SPYCLOAK)
		{
			return false;
		}

		//Disguised
		if (pPlayer->IsDisguised() && pPlayer->GetDisguisedTeam() == GetTeam())
		{
			return false;
		}
	}

	if (pEntity->GetTeam() != GetTeam() && FVisible(pEntity->BaseEntity(), MASK_OPAQUE, NULL))
	{
			return true;
	}

	return false;
}

void CTrackingProjectile::TurnToTarget(CEntity *pEntity)
{
	Vector targetLocation = pEntity->GetLocalOrigin();
	Vector rocketLocation = GetLocalOrigin();

	Vector rocketVec = GetAbsVelocity();
	vec_t speed = 1100.0 * RocketSpeedMul.GetFloat();

	Vector locationToTarget = targetLocation;
	locationToTarget.z += 50;
	Vector newVec = locationToTarget - rocketLocation;
	newVec.NormalizeInPlace();

	QAngle angles;
	VectorAngles(newVec, angles);

	newVec *= speed;

	Teleport(NULL, &angles, &newVec);
}

bool CTrackingProjectile::IsCritical(void)
{
	return (m_bCritical) ? *m_bCritical : false;
}

void CTrackingProjectile::SetCritical(bool bCritical)
{
	if (m_bCritical)
		*m_bCritical = bCritical;
}

float CTrackingProjectile::GetDamage(void)
{
	return *m_flDamage;
}

void CTrackingProjectile::SetDamage(float flDamage)
{
	if (m_flDamage)
		*m_flDamage = flDamage;
}

DECLARE_DEFAULTHANDLER(CTrackingProjectile, FVisible, bool, (CBaseEntity *pEntity, int traceMask, CBaseEntity **ppBlocker), (pEntity, traceMask, ppBlocker));
