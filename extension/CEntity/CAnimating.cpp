#include "CAnimating.h"

LINK_ENTITY_TO_CLASS(CBaseAnimating, CAnimating);


SH_DECL_MANUALHOOK0_void(StudioFrameAdvance, 0, 0, 0);

DECLARE_HOOK(StudioFrameAdvance, CAnimating);
DECLARE_DETOUR(Dissolve, CAnimating);

DECLARE_DEFAULTHANDLER_void(CAnimating, StudioFrameAdvance, (), ());
DECLARE_DEFAULTHANDLER_DETOUR(CAnimating, Dissolve, bool, (const char *pMaterialName, float flStartTime, bool bNPCOnly, int nDissolveType, Vector vDissolverOrigin, int iMagnitude), (pMaterialName, flStartTime, bNPCOnly, nDissolveType, vDissolverOrigin, iMagnitude));
