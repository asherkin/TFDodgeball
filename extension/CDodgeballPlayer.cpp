#include "extension.h"
#include "CDodgeballPlayer.h"
#include "CPlayer.h"

LINK_ENTITY_TO_CLASS(CTFPlayer, CDodgeballPlayer);

ConVar WeaponParticle("sm_dodgeball_weaponparticle", "0.0", FCVAR_NONE, "", true, 0.0, true, 5.0);

void CDodgeballPlayer::HandleCommand_JoinClass(const char *pClass, bool unk)
{
	if (DodgeballEnabled.GetBool())
	{
		BaseClass::HandleCommand_JoinClass("pyro", unk);
	} else {
		BaseClass::HandleCommand_JoinClass(pClass, unk);
	}
}

CBaseEntity *CDodgeballPlayer::GiveNamedItem(char const *szName, int iSubType, CScriptCreatedItem *item, bool bUnknown)
{
	if (!DodgeballEnabled.GetBool() || !item)
	{
		return BaseClass::GiveNamedItem(szName, iSubType, item, bUnknown);
	} else if (GetPlayerClass() != PLAYERCLASS_PYRO) {
		g_pSM->LogError(myself, "WARNING: Server tried to give a weapon to a player that isn't a pyro.");
		return BaseClass::GiveNamedItem(szName, iSubType, item, bUnknown);
	} else if (!(item->m_iItemDefinitionIndex == 21 || item->m_iItemDefinitionIndex == 40)) {
		return NULL;
	}
	
	CScriptCreatedItem newitem;
	CSCICopy(item, &newitem);

	newitem.m_Attributes.Purge();

	CScriptCreatedAttribute tempAttrib;

	tempAttrib.m_iAttributeDefinitionIndex = 112;
	tempAttrib.m_flValue = 0.25;
	newitem.m_Attributes.AddToTail(tempAttrib);

	tempAttrib.m_iAttributeDefinitionIndex = 74;
	tempAttrib.m_flValue = 4.0;
	newitem.m_Attributes.AddToTail(tempAttrib);

	tempAttrib.m_iAttributeDefinitionIndex = 1;
	tempAttrib.m_flValue = 0.0;
	newitem.m_Attributes.AddToTail(tempAttrib);

	tempAttrib.m_iAttributeDefinitionIndex = 60;
	tempAttrib.m_flValue = 0.0;
	newitem.m_Attributes.AddToTail(tempAttrib);

	tempAttrib.m_iAttributeDefinitionIndex = 66;
	tempAttrib.m_flValue = 0.0;
	newitem.m_Attributes.AddToTail(tempAttrib);

	tempAttrib.m_iAttributeDefinitionIndex = 72;
	tempAttrib.m_flValue = 0.0;
	newitem.m_Attributes.AddToTail(tempAttrib);

	tempAttrib.m_iAttributeDefinitionIndex = 74;
	tempAttrib.m_flValue = 0.0;
	newitem.m_Attributes.AddToTail(tempAttrib);

	if (WeaponParticle.GetInt() > 0)
	{
		tempAttrib.m_iAttributeDefinitionIndex = 134;
		tempAttrib.m_flValue = WeaponParticle.GetFloat();
		newitem.m_Attributes.AddToTail(tempAttrib);
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

	copymember(m_bInitialized);

#ifdef _WIN32
	copymember(m_Padding3);
#endif

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

