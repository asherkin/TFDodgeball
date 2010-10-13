#include "extension.h"
#include "CDodgeballPlayer.h"
#include "CPlayer.h"

LINK_ENTITY_TO_CLASS(CTFPlayer, CDodgeballPlayer);

ConVar WeaponParticle("sm_dodgeball_weaponparticle", "0.0", FCVAR_NONE, "", true, 0.0, true, 19.0);

void CDodgeballPlayer::HandleCommand_JoinClass(const char *pClass, bool unk)
{
	if (DodgeballEnabled.GetBool())
	{
		BaseClass::HandleCommand_JoinClass("pyro", unk);
	} else {
		BaseClass::HandleCommand_JoinClass(pClass, unk);
	}
}

bool IsFlamethrower(int index) {
	return (index == 21 ||	// TF_WEAPON_FLAMETHROWER
			index == 40 ||	// The Backburner
			index == 215);	// The Degreaser
}

bool IsPyroHat(int index) {
	return (index == 51 ||	// Pyro's Beanie
			index == 102 ||	// Respectless Rubber Glove
			index == 105 ||	// Brigade Helm
			index == 151 ||	// Pyro Brain Sucker
			index == 175 ||	// Pyro Monocle
			index == 182 ||	// Pyro Helm
			index == 213 ||	// The Attendant
			index == 247 ||	// Old Guadalajara
			index == 248 ||	// Napper's Respite
			index == 253);	// Handyman's Handle
}

bool IsAllClassHat(int index) {
	return (index == 115 ||	// Mildly Disturbing Halloween Mask
			index == 116 ||	// Ghastly Gibus
			index == 125 ||	// Honest Halo
			index == 126 ||	// L4D Hat
			index == 134 ||	// Propaganda Contest First Place
			index == 135 ||	// Towering Pillar of Hats
			index == 136 ||	// Propaganda Contest Second Place
			index == 137 ||	// Noble Amassment of Hats
			index == 138 ||	// Propaganda Contest Third Place
			index == 139 ||	// Modest Pile of Hat
			index == 143 ||	// OSX Item
			index == 162 ||	// TTG Max Hat
			index == 164 ||	// Employee Badge A
			index == 165 ||	// Employee Badge B
			index == 166 ||	// Employee Badge C
			index == 170 ||	// Employee Badge Plat
			index == 189 ||	// Parasite Hat
			// Unknown if action items need to be here.
			index == 242 ||	// Duel Medal Bronze
			index == 243 ||	// Duel Medal Silver
			index == 244 ||	// Duel Medal Gold
			index == 245 ||	// Duel Medal Plat
			index == 260 ||	// Wiki Cap
			index == 261 ||	// Mann Co. Cap
			index == 262 ||	// Polycount Pin
			index == 263);	// Ellis Hat
}

CBaseEntity *CDodgeballPlayer::GiveNamedItem(char const *szName, int iSubType, CScriptCreatedItem *item, bool bUnknown)
{
	if (!DodgeballEnabled.GetBool() || !item)
	{
		return BaseClass::GiveNamedItem(szName, iSubType, item, bUnknown);
	} else if (GetPlayerClass() != PLAYERCLASS_PYRO) {
		g_pSM->LogError(myself, "WARNING: Server tried to give a weapon to a player that isn't a pyro.");
		return BaseClass::GiveNamedItem(szName, iSubType, item, bUnknown);
	} else if (IsPyroHat(item->m_iItemDefinitionIndex) || IsAllClassHat(item->m_iItemDefinitionIndex)) {
		return BaseClass::GiveNamedItem(szName, iSubType, item, bUnknown);
	} else if (!IsFlamethrower(item->m_iItemDefinitionIndex)) {
		return NULL;
	}
	
	CScriptCreatedItem newitem;
	CSCICopy(item, &newitem);

	newitem.m_Attributes.Purge();

	newitem.m_Attributes.AddToTail(CScriptCreatedAttribute(112,	0.25)); // +%s1% ammo regenerated every 5 seconds on wearer
	newitem.m_Attributes.AddToTail(CScriptCreatedAttribute(76,	4.00)); // +%s1% max primary ammo on wearer
	////newitem.m_Attributes.AddToTail(CScriptCreatedAttribute(1,	0.00)); // %s1% damage done
	newitem.m_Attributes.AddToTail(CScriptCreatedAttribute(60,	0.00)); // +%s1% fire damage resistance on wearer
	//newitem.m_Attributes.AddToTail(CScriptCreatedAttribute(66,	0.00)); // +%s1% bullet damage resistance on wearer
	////newitem.m_Attributes.AddToTail(CScriptCreatedAttribute(72,	0.00)); // %s1% burn damage
	////newitem.m_Attributes.AddToTail(CScriptCreatedAttribute(74,	0.00)); // %s1% burn duration

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

	copymember(m_pUnknown);

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

