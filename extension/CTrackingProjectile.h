#ifndef _INCLUDE_CTRACKINGPROJECTILE_H_
#define _INCLUDE_CTRACKINGPROJECTILE_H_

#include "CEntityManager.h"
#include "CEntity.h"
#include "CTFRocket.h"

class CTrackingProjectile : public CTFRocket
{
public:
	DECLARE_CLASS(CTrackingProjectile, CTFRocket);
	//DECLARE_DATADESC();

	virtual void Init(edict_t *pEdict, CBaseEntity *pBaseEntity);
	virtual void Spawn(void);

public:
	virtual void TrackThink(void);
	virtual void FindThink(void);
	bool IsValidTarget(CPlayer *pEntity);
	void TurnToTarget(CPlayer *pEntity);

	bool IsCritical(void);
	void SetCritical(bool bCritical);

public: // CBasePlayer virtuals
	virtual	bool FVisible(CBaseEntity *pEntity, int traceMask = MASK_BLOCKLOS, CBaseEntity **ppBlocker = NULL);

public: //Autohandlers
	DECLARE_DEFAULTHEADER(FVisible, bool, (CBaseEntity *pEntity, int traceMask, CBaseEntity **ppBlocker));

private:
	CPlayer *m_pCurrentTarget;
	int m_iLastTeam;
	bool m_bDodgeballRocket;

protected:
	bool m_bHasThought;

protected: //Sendprops
	DECLARE_SENDPROP(int, m_iDeflected);
	DECLARE_SENDPROP(bool, m_bCritical);
};

#endif // _INCLUDE_CTRACKINGPROJECTILE_H_
