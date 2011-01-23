#ifndef _INCLUDE_CDODGEBALLPLAYER_H_
#define _INCLUDE_CDODGEBALLPLAYER_H_

#include "CEntityManager.h"
#include "CEntity.h"
#include "CPlayer.h"
#include "CTFDBCreatedItem.h"

class CTFDBPlayer : public CPlayer
{
public:
	DECLARE_CLASS(CTFDBPlayer, CPlayer);

	virtual CBaseEntity *GiveNamedItem(char const *szName, int iSubType, CScriptCreatedItem *item, bool bUnknown);
	virtual void HandleCommand_JoinClass(const char *pClass, bool unk);
	virtual int OnTakeDamage(CEntityTakeDamageInfo &info);
	virtual bool ShouldGib(const CEntityTakeDamageInfo &info, bool unk);
};

void CSCICopy(CScriptCreatedItem *olditem, CScriptCreatedItem *newitem);

extern ConVar DissolveDelay;

#endif // _INCLUDE_CDODGEBALLPLAYER_H_
