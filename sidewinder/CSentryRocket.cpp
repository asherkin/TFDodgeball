#include "CSentryRocket.h"

ConVar SentryCrit("sm_sentryrocket_crit", "1", FCVAR_NONE);

LINK_ENTITY_TO_CLASS(tf_projectile_sentryrocket, CSentryRocket);

void CSentryRocket::Init(edict_t *pEdict, CBaseEntity *pBaseEntity, bool addHooks)
{
	BaseClass::Init(pEdict, pBaseEntity, addHooks);
}

void CSentryRocket::Spawn(void)
{
	BaseClass::Spawn();

	if (SentryCrit.GetBool())
	{
		SetCritical(true);
	}

	
}
