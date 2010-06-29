#include "extension.h"
#include "CDodgeballPlayer.h"
#include "CPlayer.h"

SH_DECL_MANUALHOOK0_void(ForceRespawn, 0, 0, 0);

LINK_ENTITY_TO_CLASS(CTFPlayer, CDodgeballPlayer);

ConVar WeaponParticle("sm_dodgeball_weaponparticle", "0.0", FCVAR_NONE, "", true, 0.0, true, 5.0);

DECLARE_HOOK(ForceRespawn, CDodgeballPlayer);

DECLARE_DEFAULTHANDLER_void(CDodgeballPlayer, ForceRespawn, (void), ());

void CDodgeballPlayer::Spawn(void)
{
	BaseClass::Spawn();

	/*if (GetPlayerClass() != PLAYERCLASS_PYRO && GetPlayerClass() != PLAYERCLASS_UNKNOWN)
	{
		SetPlayerClass(PLAYERCLASS_PYRO, true);
		ForceRespawn();
	}*/
}

CBaseEntity *CDodgeballPlayer::GiveNamedItem(char const *szName, int iSubType, CScriptCreatedItem *item, bool bUnknown)
{
	if (item->m_iItemDefinitionIndex != 21 && item->m_iItemDefinitionIndex != 40)
	{
		//return BaseClass::GiveNamedItem(szName, iSubType, item, bUnknown);
		return NULL;
	}

	item->m_Attributes.Purge();

	CScriptCreatedAttribute tempAttrib;

	tempAttrib.m_iAttributeDefinitionIndex = 112;
	tempAttrib.m_flValue = 0.25;
	item->m_Attributes.AddToTail(tempAttrib);

	tempAttrib.m_iAttributeDefinitionIndex = 74;
	tempAttrib.m_flValue = 4.0;
	item->m_Attributes.AddToTail(tempAttrib);

	tempAttrib.m_iAttributeDefinitionIndex = 1;
	tempAttrib.m_flValue = 0.0;
	item->m_Attributes.AddToTail(tempAttrib);

	tempAttrib.m_iAttributeDefinitionIndex = 60;
	tempAttrib.m_flValue = 0.0;
	item->m_Attributes.AddToTail(tempAttrib);

	tempAttrib.m_iAttributeDefinitionIndex = 66;
	tempAttrib.m_flValue = 0.0;
	item->m_Attributes.AddToTail(tempAttrib);

	tempAttrib.m_iAttributeDefinitionIndex = 72;
	tempAttrib.m_flValue = 0.0;
	item->m_Attributes.AddToTail(tempAttrib);

	tempAttrib.m_iAttributeDefinitionIndex = 74;
	tempAttrib.m_flValue = 0.0;
	item->m_Attributes.AddToTail(tempAttrib);

	if (WeaponParticle.GetInt() > 0)
	{
		tempAttrib.m_iAttributeDefinitionIndex = 134;
		tempAttrib.m_flValue = WeaponParticle.GetFloat();
		item->m_Attributes.AddToTail(tempAttrib);
	}

	return BaseClass::GiveNamedItem(szName, iSubType, item, bUnknown);
}