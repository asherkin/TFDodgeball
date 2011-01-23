#ifndef _INCLUDE_CTrackingProjectile_Nuke_H_
#define _INCLUDE_CTrackingProjectile_Nuke_H_

#include "CEntityManager.h"
#include "CEntity.h"
#include "CAnimating.h"
#include "CVisibleNuke.h"

class CTrackingProjectile_Nuke : public CAnimating
{
public:
	DECLARE_CLASS(CTrackingProjectile_Nuke, CAnimating);
	//DECLARE_DATADESC();

	virtual void Init(edict_t *pEdict, CBaseEntity *pBaseEntity);
	virtual void Spawn(void);

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
	int m_lastTeam;
	CVisibleNuke *m_pGlowModel;

protected:
	bool   m_bHasThought;

protected: //Sendprops
	DECLARE_SENDPROP(int, m_iDeflected);
	DECLARE_SENDPROP(bool, m_bCritical);
};

#endif // _INCLUDE_CTrackingProjectile_Nuke_H_
