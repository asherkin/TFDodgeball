#include "CEntityBase.h"
#include "CEntity.h"

CEntity *pEntityData[MAX_EDICTS+1] = {NULL};

CEntity *CEntityLookup::Instance(CBaseEntity *pEnt)
{
	if (!pEnt)
	{
		return NULL;
	}

	IServerNetworkable *pNetwork = pEnt->GetNetworkable();

	if  (!pNetwork)
	{
		return NULL;
	}

	edict_t *pEdict = pNetwork->GetEdict();

	if (!pEdict)
	{
		return NULL;
	}

	return Instance(pEdict);
}

CEntity *CEntityLookup::Instance(int iEnt)
{
	return pEntityData[iEnt];
}

CEntity *CEntityLookup::Instance(const edict_t *pEnt)
{
	return Instance(engine->IndexOfEdict(pEnt));
}

CEntity *CEntityLookup::Instance(const CBaseHandle &hEnt)
{
	if (!hEnt.IsValid())
	{
		return NULL;
	}

	int index = hEnt.GetEntryIndex();

	edict_t *pStoredEdict;
	CBaseEntity *pStoredEntity;

	pStoredEdict = engine->PEntityOfEntIndex(index);
	if (!pStoredEdict || pStoredEdict->IsFree())
	{
		return NULL;
	}

	IServerUnknown *pUnk;
	if ((pUnk = pStoredEdict->GetUnknown()) == NULL)
	{
		return NULL;
	}

	pStoredEntity = pUnk->GetBaseEntity();

	if (pStoredEntity == NULL)
	{
		return NULL;
	}

	IServerEntity *pSE = pStoredEdict->GetIServerEntity();

	if (pSE == NULL)
	{
		return NULL;
	}

	if (pSE->GetRefEHandle() != hEnt)
	{
		return NULL;
	}

	return Instance(index);
}

CEntity *CEntityLookup::Instance(edict_t *pEnt)
{
	return Instance(engine->IndexOfEdict(pEnt));
}