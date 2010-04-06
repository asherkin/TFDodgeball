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

#ifndef _INCLUDE_CPLAYER_H_
#define _INCLUDE_CPLAYER_H_

#include "CEntity.h"

class CPlayer : public CEntity
{
public:
	DECLARE_CLASS(CPlayer, CEntity);

	virtual void Init(edict_t *pEdict, CBaseEntity *pBaseEntity, bool addHooks);
	virtual void Destroy();
	virtual bool IsPlayer();

	bool IsAlive();
	int GetPlayerClass();
	int GetPlayerCond();
	bool IsDisguised();
	int GetDisguisedTeam();

	int GetHealth();
	void SetHealth(int iHealth);


public: // CBasePlayer virtuals
	virtual	bool FVisible(CBaseEntity *pEntity, int traceMask = MASK_BLOCKLOS, CBaseEntity **ppBlocker = NULL);

public: //Autohandlers
	DECLARE_DEFAULTHEADER(FVisible, bool, (CBaseEntity *pEntity, int traceMask, CBaseEntity **ppBlocker));

protected: // Sendprops
	float *m_flNextAttack;
	CBaseHandle *m_hActiveWeapon;
	CBaseHandle *m_hMyWeapons;
	uint16_t *m_iHealth;
	uint8_t *m_lifeState;

	uint8_t *m_iClass;
	uint16_t *m_nPlayerCond;
	bool *m_bJumping;
	uint8_t *m_nNumHealers;
	uint8_t *m_nPlayerState;
	uint8_t *m_nDisguiseTeam;
	uint8_t *m_nDisguiseClass;
	uint8_t *m_iDisguiseTargetIndex;
	uint16_t *m_iDisguiseHealth;
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
