#ifndef _INCLUDE_CTRACKINGPROJECTILE_H_
#define _INCLUDE_CTRACKINGPROJECTILE_H_

#include "CEntityManager.h"
#include "CEntity.h"

class CTrackingProjectile : public CEntity
{
public:
	DECLARE_CLASS(CTrackingProjectile, CEntity);

	virtual void Init(edict_t *pEdict, CBaseEntity *pBaseEntity);
	virtual void Spawn(void);

public:
	virtual void TrackThink(void);
	virtual void FindThink(void);
	bool IsValidTarget(CEntity *pEntity);
	void TurnToTarget(CEntity *pEntity);
	void TurnToLocaton(Vector targetLocation);

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
	bool   m_bHasThought;

protected: //Sendprops
	DECLARE_SENDPROP(int, m_iDeflected);
	DECLARE_SENDPROP(bool, m_bCritical);
};

#endif // _INCLUDE_CTRACKINGPROJECTILE_H_
