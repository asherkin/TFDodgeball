#ifndef _INCLUDE_CTRACKINGPROJECTILE_H_
#define _INCLUDE_CTRACKINGPROJECTILE_H_

#include "CEntityManager.h"
#include "CEntity.h"

class CTrackingProjectile : public CEntity
{
public:
	DECLARE_CLASS(CTrackingProjectile, CEntity);

	virtual void Init(edict_t *pEdict, CBaseEntity *pBaseEntity, bool addHooks);
	virtual void Spawn(void);
	virtual void StartTouch(CBaseEntity *entity);

public:
	virtual void TrackThink(void);
	virtual void FindThink(void);
	bool IsValidTarget(CEntity *pEntity);
	void TurnToTarget(CEntity *pEntity);

	bool IsCritical(void);
	void SetCritical(bool bCritical);

public: // CBasePlayer virtuals
	virtual	bool FVisible(CBaseEntity *pEntity, int traceMask = MASK_BLOCKLOS, CBaseEntity **ppBlocker = NULL);

public: //Autohandlers
	DECLARE_DEFAULTHEADER(FVisible, bool, (CBaseEntity *pEntity, int traceMask, CBaseEntity **ppBlocker));

private:
	int m_currentTarget;
	float m_lastSearch;

protected:	
	bool  *m_bCritical;
	bool   m_bHasThought;
};

#endif // _INCLUDE_CTRACKINGPROJECTILE_H_
