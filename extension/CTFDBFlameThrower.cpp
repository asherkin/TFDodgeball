#include "extension.h"
#include "CTFDBFlameThrower.h"
#include "CAnimating.h"
#include "CTFDBPlayer.h"

LINK_ENTITY_TO_CLASS(CTFFlameThrower, CTFDBFlameThrower);

ConVar DeflectPlayers("sm_dodgeball_deflect_players", "1", FCVAR_NONE, "", true, 0.0, true, 1.0);
ConVar DeflectPlayersDamage("sm_dodgeball_deflect_players_damage", "0.0", FCVAR_NONE, "", true, 0.0, false, 0.0);

bool CTFDBFlameThrower::DeflectPlayer(CBaseEntity *pTarget, CBaseEntity *pOwner, Vector &vecForward, Vector &vecCenter, Vector &vecSize)
{
	if (!DodgeballEnabled.GetBool() || (!pTarget || !pOwner))
		return BaseClass::DeflectPlayer(pTarget, pOwner, vecForward, vecCenter, vecSize);

	CPlayer *pTargetPlayer = static_cast<CPlayer *>(CEntity::Instance(pTarget));
	CPlayer *pOwnerPlayer = static_cast<CPlayer *>(CEntity::Instance(pOwner));

	if (pTargetPlayer->InSameTeam(pOwnerPlayer))
		return BaseClass::DeflectPlayer(pTarget, pOwner, vecForward, vecCenter, vecSize);

	if (pTargetPlayer->IsAlive() && DeflectPlayersDamage.GetFloat() > 0.0)
	{
		CEntityTakeDamageInfo damageInfo;
		damageInfo.m_hAttacker = pTarget;
		damageInfo.m_hInflictor = pTarget;
		damageInfo.m_flDamage = DeflectPlayersDamage.GetFloat();
		damageInfo.m_bitsDamageType = DMG_GENERIC;

		pOwnerPlayer->OnTakeDamage(damageInfo);
	}

	if (!DeflectPlayers.GetBool())
		return false;

	return BaseClass::DeflectPlayer(pTarget, pOwner, vecForward, vecCenter, vecSize);
}


