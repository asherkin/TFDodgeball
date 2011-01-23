#ifndef _INCLUDE_CANIMATING_H_
#define _INCLUDE_CANIMATING_H_

#include "CEntity.h"

class CAnimating : public CEntity
{
public:
	DECLARE_CLASS(CAnimating, CEntity);

	virtual void StudioFrameAdvance();
	virtual bool Dissolve(const char *pMaterialName, float flStartTime, bool bNPCOnly, int nDissolveType, Vector vDissolverOrigin, int iMagnitude);

public:
	DECLARE_DEFAULTHEADER(StudioFrameAdvance, void, ());
	DECLARE_DEFAULTHEADER_DETOUR(Dissolve, bool, (const char *pMaterialName, float flStartTime, bool bNPCOnly, int nDissolveType, Vector vDissolverOrigin, int iMagnitude));
};

#endif // _INCLUDE_CANIMATING_H_