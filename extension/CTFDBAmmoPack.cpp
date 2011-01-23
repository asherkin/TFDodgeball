#include "extension.h"
#include "CTFDBAmmoPack.h"
#include "CAnimating.h"
#include "CTFDBPlayer.h"

LINK_ENTITY_TO_CLASS(CTFAmmoPack, CTFDBAmmoPack);

ConVar DissolveWeapons("sm_dodgeball_dissolve_weapons", "1", FCVAR_NONE, "", true, 0.0, true, 1.0);

void CTFDBAmmoPack::Spawn(void)
{
	BaseClass::Spawn();

	if (DodgeballEnabled.GetBool() && DissolveWeapons.GetBool())
		Dissolve(NULL, gpGlobals->curtime + DissolveDelay.GetFloat(), false, ENTITY_DISSOLVE_NORMAL, GetAbsOrigin(), 250);
}
