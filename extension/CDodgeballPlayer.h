#ifndef _INCLUDE_CDODGEBALLPLAYER_H_
#define _INCLUDE_CDODGEBALLPLAYER_H_

#include "CEntityManager.h"
#include "CEntity.h"
#include "CPlayer.h"
#include "CTFDBCreatedItem.h"

class CDodgeballPlayer : public CPlayer
{
public:
	DECLARE_CLASS(CDodgeballPlayer, CPlayer);

	virtual CBaseEntity *GiveNamedItem(char const *szName, int iSubType, CScriptCreatedItem *item, bool bUnknown);
	virtual void HandleCommand_JoinClass(const char *pClass, bool unk);
	void TakeDamage(const CEntityTakeDamageInfo &inputInfo);
};

void CSCICopy(CScriptCreatedItem *olditem, CScriptCreatedItem *newitem);

#endif // _INCLUDE_CDODGEBALLPLAYER_H_
