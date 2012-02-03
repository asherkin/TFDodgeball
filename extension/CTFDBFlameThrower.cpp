#include "extension.h"
#include "CTFDBFlameThrower.h"
#include "CAnimating.h"
#include "CTFDBPlayer.h"

LINK_ENTITY_TO_CLASS(CTFFlameThrower, CTFDBFlameThrower);

ConVar DeflectPlayers("sm_dodgeball_deflect_players", "1", FCVAR_NONE, "", true, 0.0, true, 1.0);
ConVar DeflectPlayersDamage("sm_dodgeball_deflect_players_damage", "0", FCVAR_NONE, "", true, 0.0, false, 0.0);

bool CTFDBFlameThrower::DeflectPlayer(CBaseEntity *pTarget, CBaseEntity *pOwner, Vector &vecForward, Vector &vecCenter, Vector &vecSize)
{
	if (!DodgeballEnabled.GetBool() || (pTarget && pOwner && (CEntity::Instance(pTarget)->GetTeamNumber() == CEntity::Instance(pOwner)->GetTeamNumber())))
	{
		return BaseClass::DeflectPlayer(pTarget, pOwner, vecForward, vecCenter, vecSize);
	} else {
		if (DeflectPlayersDamage.GetInt())
		{
			META_CONPRINTF("Implement me, please :(\n");
			//CEntity::Instance(pOwner)->OnTakeDamage(NULL);
		}

		if (DeflectPlayers.GetBool())
		{
			return BaseClass::DeflectPlayer(pTarget, pOwner, vecForward, vecCenter, vecSize);
		} else {
			return false;
		}
	}
}


