/**
* =============================================================================
* CEntity Entity Handling Framework
* Copyright (C) 2010 Matt Woodrow.  All rights reserved.
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

#ifndef _INCLUDE_CPLAYER_H_
#define _INCLUDE_CPLAYER_H_

#include "CEntity.h"
#include "CTakeDamageInfo.h"
#include "sh_list.h"
#include "shareddefs.h"
#include "usercmd.h"
#include "vehicles.h"
#include "mathlib.h"
#include "CScriptCreatedItem.h"


class CPlayer : public CEntity
{
public:
	DECLARE_CLASS(CPlayer, CEntity);
	DECLARE_DATADESC();
	virtual bool IsPlayer();

	bool IsAlive();
	int GetPlayerClass();
	int GetPlayerCond();
	bool IsDisguised();
	int GetDisguisedTeam();
	int GetButtons();
	IServerVehicle *GetVehicle();
	virtual void Spawn();
	virtual int OnTakeDamage(CEntityTakeDamageInfo &info)
	{
		return BaseClass::OnTakeDamage(info);
	}

public:
	int GetHealth();
	void SetHealth(int health);
	float GetMovementSpeed();
	void SetMovementSpeed(float speed);

	void GetClientEyePosition(Vector &pos);
	virtual QAngle *GetClientEyeAngles();
	int GetObserverMode();
	CEntity* GetObserverTarget();
	CEntity* GetAimTarget(bool playersOnly);

public: // CBasePlayer virtuals
	virtual	bool FVisible(CEntity *pEntity, int traceMask = MASK_BLOCKLOS, CEntity **ppBlocker = NULL);
	virtual void PlayerRunCmd(CUserCmd *, IMoveHelper *);
	virtual void LeaveVehicle( const Vector &vecExitPoint = vec3_origin, const QAngle &vecExitAngles = vec3_angle );
	virtual void ProcessUserCmds(CUserCmd *cmds, int numcmds, int totalcmds, int dropped_packets, bool paused);
	virtual void PreThink(void);
	virtual void PostThink(void);
	virtual void Jump(void);
	virtual int OnTakeDamage_Alive(CEntityTakeDamageInfo &info);
	virtual bool WeaponSwitch(CBaseEntity /*CBaseCombatWeapon*/ *pWeapon, int viewmodelindex);
	virtual bool IsReadyToSpawn(void);
	virtual bool CanSpeakVoiceCommand(void);

public: //Virtual calls
	virtual CBaseEntity *GiveNamedItem(char const *szName, int iSubType, CScriptCreatedItem *item, bool bUnknown);
	virtual bool RemovePlayerItem(CBaseEntity *pWeapon);
	virtual void Weapon_Equip(CBaseEntity *pWeapon);
	virtual CBaseEntity *Weapon_GetSlot(int slot);

public: //Autohandlers
	DECLARE_DEFAULTHEADER(FVisible, bool, (CBaseEntity *pEntity, int traceMask, CBaseEntity **ppBlocker));
	DECLARE_DEFAULTHEADER(PlayerRunCmd, void, (CUserCmd *pCmd, IMoveHelper *pHelper));
	DECLARE_DEFAULTHEADER(LeaveVehicle, void, (const Vector &vecExitPoint, const QAngle &vecExitAngles));
	DECLARE_DEFAULTHEADER(ProcessUserCmds, void, (CUserCmd *cmds, int numcmds, int totalcmds, int dropped_packets, bool paused));
	DECLARE_DEFAULTHEADER(PreThink, void, ());
	DECLARE_DEFAULTHEADER(PostThink, void, ());
	DECLARE_DEFAULTHEADER(Jump, void, ());
	DECLARE_DEFAULTHEADER(OnTakeDamage_Alive, int, (CEntityTakeDamageInfo &info));
	DECLARE_DEFAULTHEADER(WeaponSwitch, bool, (CBaseEntity /*CBaseCombatWeapon*/ *pWeapon, int viewmodelindex));
	DECLARE_DEFAULTHEADER(IsReadyToSpawn, bool, ());
	DECLARE_DEFAULTHEADER(CanSpeakVoiceCommand, bool, ());
	DECLARE_DEFAULTHEADER(GiveNamedItem, CBaseEntity *, (char const *szName, int iSubType, CScriptCreatedItem *item, bool bUnknown));
	DECLARE_DEFAULTHEADER(RemovePlayerItem, bool, (CBaseEntity *pItem));
	DECLARE_DEFAULTHEADER(Weapon_Equip, void, (CBaseEntity *pWeapon));
	DECLARE_DEFAULTHEADER(Weapon_GetSlot, CBaseEntity *, (int slot));
	DECLARE_DEFAULTHEADER(GetClientEyeAngles, QAngle *, ());

protected: // Sendprops
	DECLARE_SENDPROP(float, m_flNextAttack);
	DECLARE_SENDPROP(CBaseHandle, m_hActiveWeapon);
	DECLARE_SENDPROP(CBaseHandle, m_hMyWeapons);
	DECLARE_SENDPROP(CBaseHandle, m_hVehicle);
	DECLARE_SENDPROP(uint16_t, m_iHealth);
	//DECLARE_SENDPROP(uint16_t, m_i);
	DECLARE_SENDPROP(uint8_t, m_lifeState);
	DECLARE_SENDPROP(uint8_t, m_iClass);
	DECLARE_SENDPROP(uint16_t, m_nPlayerCond);
	DECLARE_SENDPROP(bool, m_bJumping);
	DECLARE_SENDPROP(uint8_t, m_nNumHealers);
	DECLARE_SENDPROP(uint8_t, m_nPlayerState);
	DECLARE_SENDPROP(uint8_t, m_nDisguiseTeam);
	DECLARE_SENDPROP(uint8_t, m_nDisguiseClass);
	DECLARE_SENDPROP(uint8_t, m_iDisguiseTargetIndex);
	DECLARE_SENDPROP(uint16_t, m_iDisguiseHealth);
	DECLARE_SENDPROP(float, m_flMaxspeed);
	DECLARE_SENDPROP(uint16_t, m_iObserverMode);
	DECLARE_SENDPROP(CBaseHandle, m_hObserverTarget);

protected:
	DECLARE_DATAMAP(int, m_nButtons);
};

#define PLAYERCLASS_SCOUT 1
#define PLAYERCLASS_SNIPER 2
#define PLAYERCLASS_SOLDIER 3
#define PLAYERCLASS_DEMOMAN 4
#define PLAYERCLASS_MEDIC 5
#define PLAYERCLASS_HEAVY 6
#define PLAYERCLASS_PYRO 7
#define PLAYERCLASS_SPY 8
#define PLAYERCLASS_ENGINEER 9

#define TEAM_UNASSIGNED 0
#define TEAM_SPEC 1
#define TEAM_RED 2
#define TEAM_BLUE 3

#define PLAYERCOND_SLOWED (1<<0) //1
#define PLAYERCOND_ZOOMED (1<<1) //2
#define PLAYERCOND_DISGUISING (1<<2) //4
#define PLAYERCOND_DISGUISED (1<<3) //8
#define PLAYERCOND_SPYCLOAK (1<<4) //16
#define PLAYERCOND_UBERED (1<<5) //32
#define PLAYERCOND_TELEPORTTRAIL (1<<6) //64
#define PLAYERCOND_TAUNT (1<<7) //128
// (1<<8) //256
// (1<<9) //512
#define PLAYERCOND_TELEPORTFLASH (1<<10) //1024
#define PLAYERCOND_KRUBER (1<<11) //2048
// (1<<12) //4096
// (1<<13) //8192
#define PLAYERCOND_BONKED (1<<14) //16384 (blame Neph if it doesn't work)
#define PLAYERCOND_BONKEDORDRINKSLOWDOWN (1<<15) //32768
#define PLAYERCOND_HEALING (1<<16) //65536
#define PLAYERCOND_BURNING (1<<17) //131072
#define PLAYERCOND_FULLYCHARGEDBYMEDIC (1<<18) //262144

#endif // _INCLUDE_CPLAYER_H_
