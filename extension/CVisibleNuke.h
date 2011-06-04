#ifndef _INCLUDE_CVisibleNuke_H_
#define _INCLUDE_CVisibleNuke_H_

#include "CEntityManager.h"
#include "CEntity.h"
#include "CAnimating.h"

class CVisibleNuke : public CAnimating
{
public:
	DECLARE_CLASS(CVisibleNuke, CAnimating);
	//DECLARE_DATADESC();

	virtual void Init(edict_t *pEdict, CBaseEntity *pBaseEntity);
	virtual void Spawn(void);
	virtual ServerClass *GetServerClass(void);
	virtual void Touch(CEntity *pOther);
public:
	void SetSkin(int nSkin);

protected: //Sendprops
	DECLARE_SENDPROP(int, m_nSkin);
};

#endif // _INCLUDE_CVisibleNuke_H_
