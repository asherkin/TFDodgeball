#include "extension.h"
#include "CTrackingProjectile_Nuke.h"
#include "CPlayer.h"
#include "CHelpers.h"
#include "worldsize.h"

SH_DECL_MANUALEXTERN3(FVisible, bool, CBaseEntity *, int, CBaseEntity **);

LINK_ENTITY_TO_CLASS(CTFProjectile_SentryRocket, CTrackingProjectile_Nuke);

#if 0
BEGIN_DATADESC(CTrackingProjectile_Nuke)
	DEFINE_THINKFUNC(FindThink),
	DEFINE_THINKFUNC(TrackThink),
END_DATADESC()
#endif

//IMPLEMENT_NULL_DATADESC(CTrackingProjectile_Nuke);

ConVar RocketSpeedMul_Nuke("sm_dodgeball_speedmul_nuke", "0.2", FCVAR_NONE);
ConVar ReflectSpeedInc_Nuke("sm_dodgeball_reflectinc_nuke", "0.02", FCVAR_NONE);
ConVar RocketTurnRate_Nuke("sm_dodgeball_turnrate_nuke", "0.1", FCVAR_NONE);

ConVar NukeBeepOnTarget("sm_dodgeball_nuke_targetbeep", "1", FCVAR_NONE, "", true, 0.0, true, 1.0);

DEFINE_PROP(m_bCritical, CTrackingProjectile_Nuke);
DEFINE_PROP(m_iDeflected, CTrackingProjectile_Nuke);

DECLARE_HOOK(FVisible, CTrackingProjectile_Nuke);

DECLARE_DEFAULTHANDLER(CTrackingProjectile_Nuke, FVisible, bool, (CBaseEntity *pEntity, int traceMask, CBaseEntity **ppBlocker), (pEntity, traceMask, ppBlocker));

void CTrackingProjectile_Nuke::Init(edict_t *pEdict, CBaseEntity *pBaseEntity)
{
	m_bHasThought = false;
	m_pMyLittlePony = NULL;

	BaseClass::Init(pEdict, pBaseEntity);

	//m_flDamage = (float *)((char *)BaseEntity() + (info.actual_offset+sizeof(int)));
}

void CTrackingProjectile_Nuke::Spawn(void)
{
	for (int i = 0; i < gpGlobals->maxEntities; i++) {
		if (CEntity::Instance(i) && CEntity::Instance(i)->GetMoveParent() == this) {
			m_pMyLittlePony = static_cast<CVisibleNuke *>(CEntity::Instance(i));
			break;
		}
	}

 	BaseClass::Spawn();

	if (DodgeballEnabled.GetBool())
	{
		SetThink(&CTrackingProjectile_Nuke::FindThink);
		SetNextThink(gpGlobals->curtime);

		m_lastTeam = GetTeamNumber();
		//*m_iDeflected = 1;
		SetOwner(CEntity::Instance(0));
	}
}

void CTrackingProjectile_Nuke::FindThink(void)
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

		if (flVictimDist < 64.0)
			continue;

		//Find closest
		if (flVictimDist < flBestVictim)
		{
			pBestVictim = pEntity;
			flBestVictim = flVictimDist;
		}
	}

	if (pBestVictim == NULL) 
	{
		SetThink(&CTrackingProjectile_Nuke::FindThink);
		SetNextThink(gpGlobals->curtime);
		return;
	}

	//TurnToTarget(pBestVictim);

	if (NukeBeepOnTarget.GetBool())
		pHelpers->EmitSoundToClient(static_cast<CPlayer *>(pBestVictim), "weapons/sentry_spot_client.wav");

	m_currentTarget = pBestVictim->entindex();
	SetThink(&CTrackingProjectile_Nuke::TrackThink);
	SetNextThink(gpGlobals->curtime);
}

void CTrackingProjectile_Nuke::TrackThink(void)
{
	if (m_lastTeam != GetTeamNumber())
	{
		//g_pSM->LogMessage(myself, "Rocket Airblasted!");

		m_pMyLittlePony->ChangeTeam(GetTeamNumber());

		if (GetTeamNumber() > 2)
			m_pMyLittlePony->SetSkin(GetTeamNumber() - 2);

		m_lastTeam = GetTeamNumber();

		SetThink(&CTrackingProjectile_Nuke::FindThink);
		SetNextThink(gpGlobals->curtime + 10.0);
		return;
	}

	CEntity *pVictim = CEntity::Instance(m_currentTarget);

	if (!IsValidTarget(pVictim))
	{
		/* This finds a new target and aims at it, or starts looping find until it does */
		FindThink();
		return;
	}

	//SetOwner(pVictim);

	TurnToTarget(pVictim);
}

bool CTrackingProjectile_Nuke::IsValidTarget(CEntity *pEntity)
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
		if (pPlayer->GetPlayerCond() & PLAYERCOND_CLOAKED)
		{
			return false;
		}

		//Disguised
		if (pPlayer->IsDisguised() && pPlayer->GetDisguisedTeam() == GetTeam())
		{
			return false;
		}
	}

	if (pEntity->GetTeamNumber() == GetTeamNumber())
	{
		return false;
	}

	if (!FVisible(pEntity->BaseEntity(), MASK_SOLID, NULL))
	{
			return false;
	}

	return true;
}

#if 0
void CTrackingProjectile_Nuke::TurnToTarget(CEntity *pEntity)
{
	Vector targetLocation = pEntity->GetLocalOrigin();
	Vector rocketLocation = GetLocalOrigin();

	Vector rocketVec = GetAbsVelocity();

	Vector locationToTarget = targetLocation;
	locationToTarget.z += 50;
	Vector newVec = locationToTarget - rocketLocation;
	newVec.NormalizeInPlace();

	QAngle angles;
	VectorAngles(newVec, angles);

	vec_t speed = 1100.0 * RocketSpeedMul.GetFloat();
	speed *= (ReflectSpeedInk.GetFloat() * (*m_iDeflected/* - 1*/)) + 1.0;
	newVec *= speed;

	Teleport(NULL, &angles, &newVec);
}
#endif

void CTrackingProjectile_Nuke::TurnToTarget(CEntity *pEntity)
{
	// Retrieve rocket info.
	Vector fRocketPosition = GetLocalOrigin();
	Vector fRocketOrientation = GetAbsVelocity();
	vec_t fCurrentSpeed;

	// Calculate speed and orientation.
	fCurrentSpeed = /*fRocketOrientation.Length()*/ 1100.0 * RocketSpeedMul_Nuke.GetFloat();
	fCurrentSpeed *= (ReflectSpeedInc_Nuke.GetFloat() * (*m_iDeflected/* - 1*/)) + 1.0;
	fRocketOrientation.NormalizeInPlace();

	// Retrieve client position and calculate new orientation.
	Vector fOrientation = pEntity->GetLocalOrigin();
	fOrientation[0] -= fRocketPosition[0];
	fOrientation[1] -= fRocketPosition[1];
	fOrientation[2] -= fRocketPosition[2] - 50.0;
	fOrientation.NormalizeInPlace();

	// Lerp from the current orientation to the new one.
	fRocketOrientation[0] = Lerp<vec_t>(RocketTurnRate_Nuke.GetFloat(), fRocketOrientation[0], fOrientation[0]);
	fRocketOrientation[1] = Lerp<vec_t>(RocketTurnRate_Nuke.GetFloat(), fRocketOrientation[1], fOrientation[1]);
	fRocketOrientation[2] = Lerp<vec_t>(RocketTurnRate_Nuke.GetFloat(), fRocketOrientation[2], fOrientation[2]);
	fRocketOrientation.NormalizeInPlace();

	// Calculate angles and final speed.
	QAngle fRocketAngles;
	VectorAngles(fRocketOrientation, fRocketAngles);
	Vector fRocketSpeed;
	fRocketSpeed[0] = fRocketOrientation[0] * fCurrentSpeed;
	fRocketSpeed[1] = fRocketOrientation[1] * fCurrentSpeed;
	fRocketSpeed[2] = fRocketOrientation[2] * fCurrentSpeed;

	// Done
	Teleport(NULL, &fRocketAngles, &fRocketSpeed);
}

bool CTrackingProjectile_Nuke::IsCritical(void)
{
	return (m_bCritical) ? *m_bCritical : false;
}

void CTrackingProjectile_Nuke::SetCritical(bool bCritical)
{
	if (m_bCritical)
		*m_bCritical = bCritical;
}
