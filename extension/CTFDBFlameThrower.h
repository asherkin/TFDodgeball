#ifndef _INCLUDE_CTFDBFlameThrower_H_
#define _INCLUDE_CTFDBFlameThrower_H_

#include "CEntityManager.h"
#include "CEntity.h"
#include "CPlayer.h"
#include "CTFWeaponBase.h"

class CTFDBFlameThrower : public CTFWeaponBase
{
public:
	DECLARE_CLASS(CTFDBFlameThrower, CTFWeaponBase);

	virtual bool DeflectPlayer(CBaseEntity *pTarget, CBaseEntity *pOwner, Vector &vecForward, Vector &vecCenter, Vector &vecSize);
};

#endif // _INCLUDE_CTFDBFlameThrower_H_
