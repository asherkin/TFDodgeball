#ifndef _INCLUDE_CDODGEBALLPLAYER_H_
#define _INCLUDE_CDODGEBALLPLAYER_H_

#include "CEntityManager.h"
#include "CEntity.h"
#include "CPlayer.h"
#include "CTFDBEconItemView.h"

class CTFDBPlayer : public CPlayer
{
public:
	DECLARE_CLASS(CTFDBPlayer, CPlayer);

	virtual CBaseEntity *GiveNamedItem(char const *szName, int iSubType, CEconItemView *pScriptItem, bool bForce);
	virtual void HandleCommand_JoinClass(const char *pClass, bool bAllowSpawn);
	virtual int OnTakeDamage(CEntityTakeDamageInfo &info);
	virtual bool ShouldGib(const CEntityTakeDamageInfo &info, bool bFeignDeath);
};

void CSCICopy(CEconItemView *olditem, CEconItemView *newitem);

extern ConVar DissolveDelay;

#endif // _INCLUDE_CDODGEBALLPLAYER_H_
