#include "extension.h"
#include "CVisibleNuke.h"
#include "CPlayer.h"
#include "CHelpers.h"
#include "worldsize.h"

LINK_ENTITY_TO_CUSTOM_CLASS(tfdb_nukeskin, prop_physics, CVisibleNuke);

//IMPLEMENT_NULL_DATADESC(CVisibleNuke);

DEFINE_PROP(m_nSkin, CVisibleNuke);

ServerClass *g_pFlagClass;

void CVisibleNuke::Init(edict_t *pEdict, CBaseEntity *pBaseEntity)
{
	if (!g_pFlagClass) 
	{
		g_pFlagClass = FindServerClass("CCaptureFlag");

		if (!g_pFlagClass)
			g_pSM->LogError(myself, "CCaptureFlag ServerClass not found.");
	}

	BaseClass::Init(pEdict, pBaseEntity);
}

void CVisibleNuke::Spawn(void)
{
	BaseClass::Spawn();
}

ServerClass *CVisibleNuke::GetServerClass(void)
{
	if (g_pFlagClass)
		return g_pFlagClass;
	else
		return BaseClass::GetServerClass();
}

void CVisibleNuke::SetSkin(int nSkin)
{
	*m_nSkin = nSkin;
}

ServerClass *FindServerClass(const char *classname)
{
	ServerClass *sc = gamedll->GetAllServerClasses();
	while (sc)
	{
		if (strcmp(sc->GetName(), classname) == 0){
			return sc;
		}
		sc = sc->m_pNext;
	}
	return NULL;
}