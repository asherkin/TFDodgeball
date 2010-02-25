#include "CProjectileRocket.h"

LINK_ENTITY_TO_CLASS(tf_projectile_rocket, CProjectileRocket);

void CProjectileRocket::Init(edict_t *pEdict, CBaseEntity *pBaseEntity, bool addHooks)
{
	BaseClass::Init(pEdict, pBaseEntity, addHooks);
}

void CProjectileRocket::Spawn(void)
{
	BaseClass::Spawn();

	if (SentryCrit.GetBool())
	{
		SetCritical(true);
	}
}
