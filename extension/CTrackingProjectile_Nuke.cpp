#include "extension.h"
#include "CTrackingProjectile_Nuke.h"
#include "CPlayer.h"
#include "CHelpers.h"
#include "worldsize.h"

SH_DECL_MANUALEXTERN3(FVisible, bool, CBaseEntity *, int, CBaseEntity **);

LINK_ENTITY_TO_CLASS(CTFProjectile_SentryRocket, CTrackingProjectile_Nuke);
//LINK_ENTITY_TO_CUSTOM_CLASS(tfdb_nuke, tf_projectile_sentryrocket, CTrackingProjectile_Nuke);

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
ConVar RocketTurnRateInc_Nuke("sm_dodgeball_turnrate_inc_nuke", "0.0", FCVAR_NONE);

ConVar NukeBeepOnTarget("sm_dodgeball_nuke_targetbeep", "0", FCVAR_NONE, "", true, 0.0, true, 1.0);
ConVar NukeRadius("sm_dodgeball_nuke_radius", "500.0", FCVAR_NONE, "", true, 0.0, false, 0.0);

DEFINE_PROP(m_bCritical, CTrackingProjectile_Nuke);
DEFINE_PROP(m_iDeflected, CTrackingProjectile_Nuke);

DECLARE_HOOK(FVisible, CTrackingProjectile_Nuke);

DECLARE_DEFAULTHANDLER(CTrackingProjectile_Nuke, FVisible, bool, (CBaseEntity *pEntity, int traceMask, CBaseEntity **ppBlocker), (pEntity, traceMask, ppBlocker));

void CTrackingProjectile_Nuke::Init(edict_t *pEdict, CBaseEntity *pBaseEntity)
{
	m_bHasThought = false;
	m_pGlowModel = NULL;

	BaseClass::Init(pEdict, pBaseEntity);

	//m_flDamage = (float *)((char *)BaseEntity() + (info.actual_offset+sizeof(int)));
}

void CTrackingProjectile_Nuke::Spawn(void)
{
 	BaseClass::Spawn();

	if (DodgeballEnabled.GetBool())
	{
		for (int i = 0; i < gpGlobals->maxEntities; i++) {
			if (CEntity::Instance(i) && CEntity::Instance(i)->GetMoveParent() == this) {
				m_pGlowModel = static_cast<CVisibleNuke *>(CEntity::Instance(i));
				break;
			}
		}

		m_iLastTeam = GetTeamNumber();
		//*m_iDeflected = 1;

		if (!GetOwner())
		{
			SetThink(&CTrackingProjectile_Nuke::FindThink);
			SetNextThink(gpGlobals->curtime);

			m_bDodgeballRocket = true;
			SetOwner(CEntity::Instance(0));
		}
	}
}

float CTrackingProjectile_Nuke::GetRadius()
{
	if (!m_bDodgeballRocket)
		return BaseClass::GetRadius();

	return NukeRadius.GetFloat();
}

void CTrackingProjectile_Nuke::FindThink(void)
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
		SetThink(&CTrackingProjectile_Nuke::FindThink);
		SetNextThink(gpGlobals->curtime);
		return;
	}

	//TurnToTarget(pBestVictim);

	if (NukeBeepOnTarget.GetBool())
		pHelpers->EmitSoundToClient(pBestVictim, "weapons/det_pack_timer.wav");

	m_pCurrentTarget = pBestVictim;
	SetThink(&CTrackingProjectile_Nuke::TrackThink);
	SetNextThink(gpGlobals->curtime);
}

void CTrackingProjectile_Nuke::TrackThink(void)
{
	if (m_iLastTeam != GetTeamNumber())
	{
		//g_pSM->LogMessage(myself, "Rocket Airblasted!");

		if (m_pGlowModel)
		{
			m_pGlowModel->ChangeTeam(GetTeamNumber());

			if (GetTeamNumber() >= 2)
			{
				m_pGlowModel->SetSkin(GetTeamNumber() - 2);
			}
		} else {
			g_pSM->LogError(myself, "Nuke %d is missing a CVisibleNuke.", entindex());
		}

		m_iLastTeam = GetTeamNumber();

		SetThink(&CTrackingProjectile_Nuke::FindThink);
		SetNextThink(gpGlobals->curtime + 10.0);
		return;
	}

	CPlayer *pVictim = static_cast<CPlayer *>(CEntity::Instance(m_pCurrentTarget->edict()));

	if (!IsValidTarget(pVictim))
	{
		/* This finds a new target and aims at it, or starts looping find until it does */
		FindThink();
		return;
	}

	//SetOwner(pVictim);

	TurnToTarget(pVictim);
}

bool CTrackingProjectile_Nuke::IsValidTarget(CPlayer *pPlayer)
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

void CTrackingProjectile_Nuke::TurnToTarget(CPlayer *pPlayer)
{
	// Retrieve rocket info.
	Vector fRocketPosition = GetLocalOrigin();
	Vector fRocketOrientation = GetAbsVelocity();
	vec_t fCurrentSpeed;

	// Calculate speed and orientation.
	fCurrentSpeed = 1100.0 * RocketSpeedMul_Nuke.GetFloat();
	fCurrentSpeed *= (ReflectSpeedInc_Nuke.GetFloat() * (*m_iDeflected)) + 1.0;
	fRocketOrientation.NormalizeInPlace();

	// Retrieve client position and calculate new orientation.
	Vector fOrientation = pPlayer->GetLocalOrigin();
	fOrientation[0] -= fRocketPosition[0];
	fOrientation[1] -= fRocketPosition[1];
	fOrientation[2] -= fRocketPosition[2] - 50.0;
	fOrientation.NormalizeInPlace();

	// Lerp from the current orientation to the new one.
	float turnRate = RocketTurnRate_Nuke.GetFloat();
	turnRate += RocketTurnRateInc_Nuke.GetFloat() * (*m_iDeflected);
	if (turnRate > 1.0) turnRate = 1.0;
	fRocketOrientation[0] = Lerp<vec_t>(turnRate, fRocketOrientation[0], fOrientation[0]);
	fRocketOrientation[1] = Lerp<vec_t>(turnRate, fRocketOrientation[1], fOrientation[1]);
	fRocketOrientation[2] = Lerp<vec_t>(turnRate, fRocketOrientation[2], fOrientation[2]);
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
