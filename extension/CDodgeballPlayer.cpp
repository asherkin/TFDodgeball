#include "extension.h"
#include "CDodgeballPlayer.h"
#include "CPlayer.h"

LINK_ENTITY_TO_CLASS(CTFPlayer, CDodgeballPlayer);

ConVar WeaponParticle("sm_dodgeball_weaponparticle", "0.0", FCVAR_NONE, "", true, 0.0, true, 19.0);
ConVar DissolvePlayers("sm_dodgeball_dissolve_players", "1", FCVAR_NONE, "", true, 0.0, true, 1.0);

void CDodgeballPlayer::HandleCommand_JoinClass(const char *pClass, bool unk)
{
	if (DodgeballEnabled.GetBool())
	{
		BaseClass::HandleCommand_JoinClass("pyro", unk);
	} else {
		BaseClass::HandleCommand_JoinClass(pClass, unk);
	}
}

void CDodgeballPlayer::TakeDamage(const CEntityTakeDamageInfo &inputInfo)
{
	if (!DodgeballEnabled.GetBool() || !DissolvePlayers.GetBool())
		return BaseClass::TakeDamage(inputInfo);

	META_CONPRINTF("TakeDamage called: %d\n", inputInfo.GetDamage());

	//inputInfo.m_bitsDamageType |= DMG_NEVERGIB;
	//inputInfo.m_bitsDamageType |= DMG_PREVENT_PHYSICS_FORCE;

	BaseClass::TakeDamage(inputInfo);

	CAnimating *pRagdoll = dynamic_cast<CAnimating *>(GetRagdoll());

	if (pRagdoll)
		pRagdoll->Dissolve(NULL, gpGlobals->curtime + 1.0, false, ENTITY_DISSOLVE_NORMAL, GetAbsOrigin(), 250);

	return;
}

CBaseEntity *CDodgeballPlayer::GiveNamedItem(char const *szName, int iSubType, CScriptCreatedItem *item, bool bUnknown)
{
	CTFDBCreatedItem *pItem = (CTFDBCreatedItem *)item;
	if (!DodgeballEnabled.GetBool() || !item)
	{
		return BaseClass::GiveNamedItem(szName, iSubType, item, bUnknown);
	} else if (GetPlayerClass() != PLAYERCLASS_PYRO) {
		g_pSM->LogError(myself, "WARNING: Server tried to give a weapon to a player that isn't a pyro.");
		return BaseClass::GiveNamedItem(szName, iSubType, item, bUnknown);
	} else if (pItem->GetLoadoutSlot(PLAYERCLASS_PYRO) == LOADOUTSLOT_MISC || pItem->GetLoadoutSlot(PLAYERCLASS_PYRO) == LOADOUTSLOT_ACTION) {
		return BaseClass::GiveNamedItem(szName, iSubType, item, bUnknown);
	} else if (pItem->GetLoadoutSlot(PLAYERCLASS_PYRO) != LOADOUTSLOT_PRIMARY) {
		return NULL;
	}
	
	CScriptCreatedItem newitem;
	CSCICopy(item, &newitem);

	newitem.m_Attributes.Purge();

	newitem.m_Attributes.AddToTail(CScriptCreatedAttribute(112,	0.25)); // +%s1% ammo regenerated every 5 seconds on wearer
	newitem.m_Attributes.AddToTail(CScriptCreatedAttribute(76,	4.00)); // +%s1% max primary ammo on wearer
	newitem.m_Attributes.AddToTail(CScriptCreatedAttribute(60,	0.00)); // +%s1% fire damage resistance on wearer

	if (WeaponParticle.GetInt() > 0)
	{
		newitem.m_Attributes.AddToTail(CScriptCreatedAttribute(134, WeaponParticle.GetFloat()));
	}

	return BaseClass::GiveNamedItem(szName, iSubType, &newitem, bUnknown);
}

void CSCICopy(CScriptCreatedItem *olditem, CScriptCreatedItem *newitem)
{
	memset(newitem, 0, sizeof(CScriptCreatedItem));
	
	//#define copymember(a) newitem->a = olditem->a
	#define copymember(a) memcpy(&newitem->a, &olditem->a, sizeof(newitem->a));

	copymember(m_pVTable);
	
#ifdef _WIN32
	copymember(m_Padding);
#endif

	copymember(m_iItemDefinitionIndex);
	copymember(m_iEntityQuality);
	copymember(m_iEntityLevel);

#ifdef _WIN32
	copymember(m_Padding2);
#endif

	copymember(m_iGlobalIndex);
	copymember(m_iGlobalIndexHigh);
	copymember(m_iGlobalIndexLow);
	copymember(m_iAccountID);
	copymember(m_iPosition);
	copymember(m_szWideName);
	copymember(m_szName);

	copymember(m_szBlob);
	copymember(m_szBlob2);

	copymember(m_Unknown);
	copymember(m_Unknown2);

	copymember(m_bInitialized);

	newitem->m_Attributes = olditem->m_Attributes;
	
	/*
	META_CONPRINTF("Copying attributes...\n");
	int nCount = olditem->m_Attributes.Count();
	META_CONPRINTF("Count: %d\n", nCount);
	newitem->m_Attributes.SetSize( nCount );
	for ( int i = 0; i < nCount; i++ )
	{
		META_CONPRINTF("Copying %d...\n", i+1);
		newitem->m_Attributes[ i ] = olditem->m_Attributes[ i ];
	}
	*/
}
