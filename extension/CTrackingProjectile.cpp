#include "extension.h"
#include "CTrackingProjectile.h"
#include "CPlayer.h"
#include "CHelpers.h"
#include "worldsize.h"

SH_DECL_MANUALEXTERN3(FVisible, bool, CBaseEntity *, int, CBaseEntity **);

LINK_ENTITY_TO_CLASS(CTFProjectile_Rocket, CTrackingProjectile);
//LINK_ENTITY_TO_CUSTOM_CLASS(tfdb_rocket, tf_projectile_rocket, CTrackingProjectile);

#if 0
BEGIN_DATADESC(CTrackingProjectile)
	DEFINE_THINKFUNC(FindThink),
	DEFINE_THINKFUNC(TrackThink),
END_DATADESC()
#endif

//IMPLEMENT_NULL_DATADESC(CTrackingProjectile);

ConVar RocketSpeedMul("sm_dodgeball_speedmul", "0.5", FCVAR_NONE);
ConVar ReflectSpeedInk("sm_dodgeball_reflectinc", "0.02", FCVAR_NONE);
ConVar RocketTurnRate("sm_dodgeball_turnrate", "0.25", FCVAR_NONE);

DEFINE_PROP(m_bCritical, CTrackingProjectile);
DEFINE_PROP(m_iDeflected, CTrackingProjectile);

DECLARE_HOOK(FVisible, CTrackingProjectile);

DECLARE_DEFAULTHANDLER(CTrackingProjectile, FVisible, bool, (CBaseEntity *pEntity, int traceMask, CBaseEntity **ppBlocker), (pEntity, traceMask, ppBlocker));

void CTrackingProjectile::Init(edict_t *pEdict, CBaseEntity *pBaseEntity)
{
	m_bHasThought = false;

	BaseClass::Init(pEdict, pBaseEntity);

	//m_flDamage = (float *)((char *)BaseEntity() + (info.actual_offset+sizeof(int)));
}

void CTrackingProjectile::Spawn(void)
{
 	BaseClass::Spawn();

	if (DodgeballEnabled.GetBool())
	{
		m_iLastTeam = GetTeamNumber();
		//*m_iDeflected = 1;

		if (!GetOwner())
		{
			SetThink(&CTrackingProjectile::FindThink);
			SetNextThink(gpGlobals->curtime);

			m_bDodgeballRocket = true;
			SetOwner(CEntity::Instance(0));
		}
	}
}

void CTrackingProjectile::FindThink(void)
{

	CPlayer *pBestVictim = NULL;
	float flBestVictim = MAX_TRACE_LENGTH;
	float flVictimDist;

	CBaseEntity *pBaseEntity = NULL;

	if (!m_bHasThought)
	{
		m_bHasThought = true;
	}

	for (int index = 1; index <= gpGlobals->maxClients; index++)
	{
		CPlayer *pPlayer = static_cast<CPlayer *>(CEntity::Instance(index));

		if (!IsValidTarget(pPlayer))
		{
			continue;
		}

		flVictimDist = (GetLocalOrigin() - pPlayer->GetLocalOrigin()).Length();

		if (flVictimDist < 64.0)
			continue;

		//Find closest
		if (flVictimDist < flBestVictim)
		{
			pBestVictim = pPlayer;
			flBestVictim = flVictimDist;
		}
	}

	if (pBestVictim == NULL) 
	{
		SetThink(&CTrackingProjectile::FindThink);
		SetNextThink(gpGlobals->curtime);
		return;
	}

	//TurnToTarget(pBestVictim);

	//pHelpers->EmitSoundToClient(pBestVictim, "weapons/sentry_spot_client.wav");

	m_pCurrentTarget = pBestVictim;
	SetThink(&CTrackingProjectile::TrackThink);
	SetNextThink(gpGlobals->curtime);
}

void CTrackingProjectile::TrackThink(void)
{
	if (m_iLastTeam != GetTeamNumber())
	{
		//g_pSM->LogMessage(myself, "Rocket Airblasted!");

		m_iLastTeam = GetTeamNumber();

		SetThink(&CTrackingProjectile::FindThink);
		SetNextThink(gpGlobals->curtime + 10.0);
		return;
	}

	CPlayer *pVictim = static_cast<CPlayer *>(CEntity::Instance(m_pCurrentTarget));

	if (!IsValidTarget(pVictim))
	{
		/* This finds a new target and aims at it, or starts looping find until it does */
		FindThink();
		return;
	}

	//SetOwner(pVictim);

	TurnToTarget(pVictim);
}

bool CTrackingProjectile::IsValidTarget(CPlayer *pPlayer)
{
	if(!pPlayer)
	{
		return false;
	}

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

	if (pPlayer->GetTeamNumber() == GetTeamNumber())
	{
		return false;
	}

	if (!FVisible(pPlayer->BaseEntity(), MASK_SOLID, NULL))
	{
			return false;
	}

	return true;
}

#if 0
void CTrackingProjectile::TurnToTarget(CEntity *pEntity)
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

void CTrackingProjectile::TurnToTarget(CPlayer *pPlayer)
{
	// Retrieve rocket info.
	Vector fRocketPosition = GetLocalOrigin();
	Vector fRocketOrientation = GetAbsVelocity();
	vec_t fCurrentSpeed;

	// Calculate speed and orientation.
	fCurrentSpeed = /*fRocketOrientation.Length()*/ 1100.0 * RocketSpeedMul.GetFloat();
	fCurrentSpeed *= (ReflectSpeedInk.GetFloat() * (*m_iDeflected/* - 1*/)) + 1.0;
	fRocketOrientation.NormalizeInPlace();

	// Retrieve client position and calculate new orientation.
	Vector fOrientation = pPlayer->GetLocalOrigin();
	fOrientation[0] -= fRocketPosition[0];
	fOrientation[1] -= fRocketPosition[1];
	fOrientation[2] -= fRocketPosition[2] - 50.0;
	fOrientation.NormalizeInPlace();

	// Lerp from the current orientation to the new one.
	fRocketOrientation[0] = Lerp<vec_t>(RocketTurnRate.GetFloat(), fRocketOrientation[0], fOrientation[0]);
	fRocketOrientation[1] = Lerp<vec_t>(RocketTurnRate.GetFloat(), fRocketOrientation[1], fOrientation[1]);
	fRocketOrientation[2] = Lerp<vec_t>(RocketTurnRate.GetFloat(), fRocketOrientation[2], fOrientation[2]);
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

bool CTrackingProjectile::IsCritical(void)
{
	return (m_bCritical) ? *m_bCritical : false;
}

void CTrackingProjectile::SetCritical(bool bCritical)
{
	if (m_bCritical)
		*m_bCritical = bCritical;
}
