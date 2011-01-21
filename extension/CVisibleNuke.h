#ifndef _INCLUDE_CVisibleNuke_H_
#define _INCLUDE_CVisibleNuke_H_

#include "CEntityManager.h"
#include "CEntity.h"

class CVisibleNuke : public CEntity
{
public:
	DECLARE_CLASS(CVisibleNuke, CEntity);
	//DECLARE_DATADESC();

	virtual void Init(edict_t *pEdict, CBaseEntity *pBaseEntity);
	virtual void Spawn(void);
	virtual ServerClass *GetServerClass(void);

public:
	void SetSkin(int nSkin);

protected: //Sendprops
	DECLARE_SENDPROP(int, m_nSkin);
};

ServerClass *FindServerClass(const char *classname);

#endif // _INCLUDE_CVisibleNuke_H_
