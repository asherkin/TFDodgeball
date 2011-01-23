#include "extension.h"
#include "CTFDBDissolveField.h"
#include "CEntity.h"
#include "CAnimating.h"
#include "CPlayer.h"

LINK_ENTITY_TO_CUSTOM_CLASS(tfdb_dissolve_field, trigger_multiple, CTFDBDissolveField);

ConVar DissolveOffset("sm_dodgeball_dissolve_weapons", "-1.5");

void CTFDBDissolveField::Spawn(void)
{
	BaseClass::Spawn();
}

void CTFDBDissolveField::StartTouch(CEntity *pOther)
{
	BaseClass::StartTouch(pOther);

	float flStartTime = gpGlobals->curtime + DissolveOffset.GetFloat();

	if (pOther->IsPlayer())
	{
		// Simply immediately kill the player.
		CPlayer *pPlayer = assert_cast< CPlayer* >(pOther);
		CEntityTakeDamageInfo info;

		info.m_hInflictor.Set(pPlayer->BaseEntity());
		info.m_hAttacker.Set(pPlayer->BaseEntity());
		info.m_flDamage = 10000;
		info.m_bitsDamageType = (DMG_GENERIC | DMG_PREVENT_PHYSICS_FORCE);

		pPlayer->TakeDamage(info);
		
		pOther = pPlayer->GetRagdoll();
		flStartTime = gpGlobals->curtime;
	}

	CAnimating *pAnimating = dynamic_cast<CAnimating *>(pOther);
	if (pAnimating)
		pAnimating->Dissolve(NULL, flStartTime, false, ENTITY_DISSOLVE_NORMAL, GetAbsOrigin(), 250);
}
