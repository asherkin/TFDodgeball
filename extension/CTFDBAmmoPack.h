#ifndef _INCLUDE_CDODGEBALLPLAYER_H_
#define _INCLUDE_CDODGEBALLPLAYER_H_

#include "CEntityManager.h"
#include "CEntity.h"
#include "CAnimating.h"

class CTFDBAmmoPack : public CAnimating
{
public:
	DECLARE_CLASS(CTFDBAmmoPack, CAnimating);

	virtual void Spawn(void);
};

#endif // _INCLUDE_CDODGEBALLPLAYER_H_
