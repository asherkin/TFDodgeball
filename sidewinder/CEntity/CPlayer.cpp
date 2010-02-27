/**
* vim: set ts=4 :
* =============================================================================
* CEntity Entity Handling Framework
* Copyright (C) 2009 Matt Woodrow.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CPlayer.h"

SH_DECL_MANUALHOOK3(FVisible, 0, 0, 0, bool, CBaseEntity *, int, CBaseEntity **);

LINK_ENTITY_TO_CLASS(player, CPlayer);
LINK_ENTITY_TO_CLASS(tf_bot, CPlayer);

void CPlayer::Init(edict_t *pEdict, CBaseEntity *pBaseEntity, bool addHooks)
{
	BaseClass::Init(pEdict, pBaseEntity, addHooks);

	sm_sendprop_info_t info;
	GET_SENDPROP_POINTER(float, m_pEdict, BaseEntity(), &info, m_flNextAttack);
	GET_SENDPROP_POINTER(CBaseHandle, m_pEdict, BaseEntity(), &info, m_hActiveWeapon);
	GET_SENDPROP_POINTER(CBaseHandle, m_pEdict, BaseEntity(), &info, m_hMyWeapons);
	GET_SENDPROP_POINTER(uint16_t, m_pEdict, BaseEntity(), &info, m_iHealth);
	GET_SENDPROP_POINTER(uint8_t, m_pEdict, BaseEntity(), &info, m_lifeState);

	GET_SENDPROP_POINTER(uint8_t, m_pEdict, BaseEntity(), &info, m_iClass);
	GET_SENDPROP_POINTER(uint16_t, m_pEdict, BaseEntity(), &info, m_nPlayerCond);
	GET_SENDPROP_POINTER(bool, m_pEdict, BaseEntity(), &info, m_bJumping);
	GET_SENDPROP_POINTER(uint8_t, m_pEdict, BaseEntity(), &info, m_nPlayerState);
	GET_SENDPROP_POINTER(uint8_t, m_pEdict, BaseEntity(), &info, m_nDisguiseTeam);
	GET_SENDPROP_POINTER(uint8_t, m_pEdict, BaseEntity(), &info, m_nDisguiseClass);
	GET_SENDPROP_POINTER(uint8_t, m_pEdict, BaseEntity(), &info, m_iDisguiseTargetIndex);
	GET_SENDPROP_POINTER(uint16_t, m_pEdict, BaseEntity(), &info, m_iDisguiseHealth);

	ADD_DEFAULTHANDLER_HOOK(CPlayer, FVisible);
}

void CPlayer::Destroy()
{
	BaseClass::Destroy();
}

DECLARE_DEFAULTHANDLER(CPlayer, FVisible, bool, (CBaseEntity *pEntity, int traceMask, CBaseEntity **ppBlocker), (pEntity, traceMask, ppBlocker));

bool CPlayer::IsPlayer()
{
	return true;
}

bool CPlayer::IsAlive()
{
	return *m_lifeState == LIFE_ALIVE;
}

int CPlayer::GetPlayerClass()
{
	return *m_iClass;
}

int CPlayer::GetPlayerCond()
{
	return *m_nPlayerCond;
}

bool CPlayer::IsDisguised()
{
	return (*m_nPlayerCond & PLAYERCOND_DISGUISED) == PLAYERCOND_DISGUISED;
}

int CPlayer::GetDisguisedTeam()
{
	return *m_nDisguiseTeam;
}
