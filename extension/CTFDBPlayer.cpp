#include "extension.h"
#include "CTFDBPlayer.h"
#include "CPlayer.h"

LINK_ENTITY_TO_CLASS(CTFPlayer, CTFDBPlayer);

ConVar WeaponParticle("sm_dodgeball_weaponparticle", "0.0", FCVAR_NONE, "", true, 0.0, true, 19.0);
ConVar DissolvePlayers("sm_dodgeball_dissolve_players", "0", FCVAR_NONE, "", true, 0.0, true, 1.0);
ConVar DissolveOverride("sm_dodgeball_dissolve", "1", FCVAR_NONE, "", true, 0.0, true, 1.0);
ConVar DissolveDelay("sm_dodgeball_dissolve_delay", "1.0", FCVAR_NONE, "");
ConVar ForcePyro("sm_dodgeball_force_class", "1", FCVAR_NONE, "", true, 0.0, true, 1.0);
ConVar ForceLoadout("sm_dodgeball_force_loadout", "2", FCVAR_NONE, "", true, 0.0, true, 2.0);
ConVar NoFireDamage("sm_dodgeball_no_fire_damage", "1", FCVAR_NONE, "", true, 0.0, true, 1.0);

void CTFDBPlayer::HandleCommand_JoinClass(const char *pClass, bool bAllowSpawn)
{
	if (DodgeballEnabled.GetBool() && ForcePyro.GetBool())
	{
		BaseClass::HandleCommand_JoinClass("pyro", bAllowSpawn);
	} else {
		BaseClass::HandleCommand_JoinClass(pClass, bAllowSpawn);
	}
}

int CTFDBPlayer::OnTakeDamage(CEntityTakeDamageInfo &info)
{
	if (!DodgeballEnabled.GetBool())
		return BaseClass::OnTakeDamage(info);
	
	if (NoFireDamage.GetBool() && ((info.m_bitsDamageType & DMG_PLASMA) != 0))
		return 0;
	
	if (!DissolveOverride.GetBool() || (!DissolvePlayers.GetBool() && (info.m_bitsDamageType & DMG_ACID) != 0))
		return BaseClass::OnTakeDamage(info);

	int ret = BaseClass::OnTakeDamage(info);

	CAnimating *pRagdoll = dynamic_cast<CAnimating *>(GetRagdoll());

	if (pRagdoll)
		pRagdoll->Dissolve(NULL, gpGlobals->curtime + DissolveDelay.GetFloat(), false, ENTITY_DISSOLVE_NORMAL, GetAbsOrigin(), 250);

	return ret;
}

bool CTFDBPlayer::ShouldGib(const CEntityTakeDamageInfo &info)
{
	if (DodgeballEnabled.GetBool() && DissolveOverride.GetBool() && (DissolvePlayers.GetBool() || (info.m_bitsDamageType & DMG_ACID) == 0))
		return false;
	else
		return BaseClass::ShouldGib(info);
}

CBaseEntity *CTFDBPlayer::GiveNamedItem(char const *szName, int iSubType, CEconItemView *pScriptItem, bool bForce)
{
	CTFDBEconItemView *pItem = (CTFDBEconItemView *)pScriptItem;
	if (!DodgeballEnabled.GetBool() || !pScriptItem)
	{
		return BaseClass::GiveNamedItem(szName, iSubType, pScriptItem, bForce);
	} else if (GetPlayerClass() != PLAYERCLASS_PYRO) {
		if (ForcePyro.GetBool())
		{
			g_pSM->LogError(myself, "WARNING: Server tried to give a weapon to a player that isn't a pyro.");
		}
		return BaseClass::GiveNamedItem(szName, iSubType, pScriptItem, bForce);
	} else if (pItem->GetLoadoutSlot(PLAYERCLASS_PYRO) == LOADOUTSLOT_HEAD || pItem->GetLoadoutSlot(PLAYERCLASS_PYRO) == LOADOUTSLOT_MISC || pItem->GetLoadoutSlot(PLAYERCLASS_PYRO) == LOADOUTSLOT_ACTION) {
		return BaseClass::GiveNamedItem(szName, iSubType, pScriptItem, bForce);
	} else if (ForceLoadout.GetInt() >= 2 && (pItem->GetLoadoutSlot(PLAYERCLASS_PYRO) != LOADOUTSLOT_PRIMARY)) {
		return NULL;
	} else if (!ForceLoadout.GetBool()) {
		return BaseClass::GiveNamedItem(szName, iSubType, pScriptItem, bForce);
	}
	
	CEconItemView newitem;
	CSCICopy(pScriptItem, &newitem);

	newitem.m_Attributes.Purge();

	newitem.m_Attributes.AddToTail(CEconItemAttribute(112,	0.25)); // +%s1% ammo regenerated every 5 seconds on wearer
	newitem.m_Attributes.AddToTail(CEconItemAttribute(76,	4.00)); // +%s1% max primary ammo on wearer

	if (WeaponParticle.GetInt() > 0)
	{
		newitem.m_Attributes.AddToTail(CEconItemAttribute(134, WeaponParticle.GetFloat()));
	}
	
	newitem.m_bDoNotIterateStaticAttributes = true;

	return BaseClass::GiveNamedItem(szName, iSubType, &newitem, bForce);
}

void CSCICopy(CEconItemView *olditem, CEconItemView *newitem)
{
	memset(newitem, 0, sizeof(CEconItemView));
	
	//#define copymember(a) newitem->a = olditem->a
	#define copymember(a) memcpy(&newitem->a, &olditem->a, sizeof(newitem->a));

	copymember(m_pVTable);

	copymember(m_iItemDefinitionIndex);
	
	copymember(m_iEntityQuality);
	copymember(m_iEntityLevel);

	copymember(m_iItemID);
	copymember(m_iItemIDHigh);
	copymember(m_iItemIDLow);
	copymember(m_iAccountID);
	copymember(m_iInventoryPosition);

	copymember(m_pAlternateItemData);
	copymember(m_bInitialized);

	copymember(m_pVTable_Attributes);
	copymember(m_pAttributeManager);
	
	copymember(m_bDoNotIterateStaticAttributes);

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
