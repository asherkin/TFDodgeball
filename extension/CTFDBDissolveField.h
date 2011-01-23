#ifndef _INCLUDE_CTFDBDissolveField_H_
#define _INCLUDE_CTFDBDissolveField_H_

#include "CEntityManager.h"
#include "CEntity.h"

class CTFDBDissolveField : public CEntity
{
public:
	DECLARE_CLASS(CTFDBDissolveField, CEntity);

	virtual void Spawn(void);

	virtual void StartTouch(CEntity *pOther);
};

#endif // _INCLUDE_CTFDBDissolveField_H_
