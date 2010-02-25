#ifndef _INCLUDE_CSENTRYROCKET_H_
#define _INCLUDE_CSENTRYROCKET_H_

#include "CTrackingProjectile.h"

class CSentryRocket : public CTrackingProjectile
{
public:
	DECLARE_CLASS(CSentryRocket, CTrackingProjectile);

	virtual void Init(edict_t *pEdict, CBaseEntity *pBaseEntity, bool addHooks);
	virtual void Spawn(void);
};


#endif // _INCLUDE_CSENTRYROCKET_H_
