#ifndef _INCLUDE_CDODGEBALLPLAYER_H_
#define _INCLUDE_CDODGEBALLPLAYER_H_

#include "CEntityManager.h"
#include "CEntity.h"
#include "CPlayer.h"

class CDodgeballPlayer : public CPlayer
{
public:
	DECLARE_CLASS(CDodgeballPlayer, CPlayer);

	virtual void Spawn(void);
	virtual CBaseEntity *GiveNamedItem(char const *szName, int iSubType, CScriptCreatedItem *item, bool bUnknown);

public: // CBasePlayer virtuals
	virtual	void ForceRespawn(void);

public: //Autohandlers
	DECLARE_DEFAULTHEADER(ForceRespawn, void, (void));
};

#endif // _INCLUDE_CDODGEBALLPLAYER_H_
