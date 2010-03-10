#include "CProjectileRocket.h"

LINK_ENTITY_TO_CLASS(tf_projectile_rocket, CProjectileRocket);

void CProjectileRocket::Init(edict_t *pEdict, CBaseEntity *pBaseEntity, bool addHooks)
{
	BaseClass::Init(pEdict, pBaseEntity, addHooks);
}

void CProjectileRocket::Spawn(void)
{
	BaseClass::Spawn();
	/*
	size_t i;
	META_CONPRINTF("Data in [%p..%p): ",this,this+sizeof(CProjectileRocket));
	for (i=0;i<sizeof(CProjectileRocket);i++)
		META_CONPRINTF("%02X ", ((unsigned char*)this)[i] );
	META_CONPRINTF("\n");
	*/
}
