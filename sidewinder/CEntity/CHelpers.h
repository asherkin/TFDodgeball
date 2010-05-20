/* ======================================================
 * TF2 RPG
 * Copyright (C) 2009 Brutal Servers.
 * All rights reserved.
 * ======================================================
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose, 
 * including commercial applications, and to alter it and redistribute it 
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not 
 * claim that you wrote the original software. If you use this software in a 
 * product, an acknowledgment in the product documentation would be 
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Version: $Id$
 */

#ifndef _INCLUDE_CHELPERS_H_
#define _INCLUDE_CHELPERS_H_

//#include "extension.h"
#include "CPlayer.h"
#include "CRecipientFilter.h"
#include "igameevents.h"

#define SOUND_FROM_PLAYER	-2

#define SNDVOL_NORMAL		1.0		/**< Normal volume */
#define SNDPITCH_NORMAL		100		/**< Normal pitch */
#define SNDPITCH_LOW		95		/**< A low pitch */
#define SNDPITCH_HIGH		120		/**< A high pitch */
#define SNDATTN_NONE		0.0		/**< No attenuation */
#define SNDATTN_NORMAL		0.8		/**< Normal attenuation */
#define SNDATTN_STATIC		1.25	/**< Static attenuation? */
#define SNDATTN_RICOCHET	1.5		/**< Ricochet effect */
#define SNDATTN_IDLE		2.0		/**< Idle attenuation? */

//Sound Levels
enum
{
	SNDLEVEL_NONE = 0,			/**< None */
	SNDLEVEL_RUSTLE = 20,		/**< Rustling leaves */
	SNDLEVEL_WHISPER = 25,		/**< Whispering */
	SNDLEVEL_LIBRARY = 30,		/**< In a library */
	SNDLEVEL_FRIDGE = 45,		/**< Refridgerator */
	SNDLEVEL_HOME = 50,			/**< Average home (3.9 attn) */
	SNDLEVEL_CONVO = 60,		/**< Normal conversation (2.0 attn) */
	SNDLEVEL_DRYER = 60,		/**< Clothes dryer */
	SNDLEVEL_DISHWASHER = 65,	/**< Dishwasher/washing machine (1.5 attn) */
	SNDLEVEL_CAR = 70,			/**< Car or vacuum cleaner (1.0 attn) */
	SNDLEVEL_NORMAL = 75,		/**< Normal sound level */
	SNDLEVEL_TRAFFIC = 75,		/**< Busy traffic (0.8 attn) */
	SNDLEVEL_MINIBIKE = 80,		/**< Mini-bike, alarm clock (0.7 attn) */
	SNDLEVEL_SCREAMING = 90,	/**< Screaming child (0.5 attn) */
	SNDLEVEL_TRAIN = 100,		/**< Subway train, pneumatic drill (0.4 attn) */
	SNDLEVEL_HELICOPTER = 105,	/**< Helicopter */
	SNDLEVEL_SNOWMOBILE = 110,	/**< Snow mobile */
	SNDLEVEL_AIRCRAFT = 120,	/**< Auto horn, aircraft */
	SNDLEVEL_RAIDSIREN = 130,	/**< Air raid siren */
	SNDLEVEL_GUNFIRE = 140,		/**< Gunshot, jet engine (0.27 attn) */
	SNDLEVEL_ROCKET = 180,		/**< Rocket launching (0.2 attn) */
};

//Sound Channels
enum
{
	SNDCHAN_REPLACE = -1,		/**< Unknown */
	SNDCHAN_AUTO = 0,			/**< Auto */
	SNDCHAN_WEAPON = 1,			/**< Weapons */
	SNDCHAN_VOICE = 2,			/**< Voices */
	SNDCHAN_ITEM = 3,			/**< Items */
	SNDCHAN_BODY = 4,			/**< Player? */
	SNDCHAN_STREAM = 5,			/**< "Stream channel from the static or dynamic area" */
	SNDCHAN_STATIC = 6,			/**< "Stream channel from the static area" */
	SNDCHAN_VOICE_BASE = 7,		/**< "Channel for network voice data" */
	SNDCHAN_USER_BASE = 135		/**< Anything >= this is allocated to game code */
};

//RayTypes
enum RayType
{
	RayType_EndPoint,
	RayType_Infinite
};

typedef struct tr_contents
{
	CBaseEntity		*entity;	//-1 if no entity hit
	Vector			endpos;
} trcontents_t;

typedef struct hudtextparms_s
{
	float		x;
	float		y;
	int			effect;
	byte		r1, g1, b1, a1;
	byte		r2, g2, b2, a2;
	float		fadeinTime;
	float		fadeoutTime;
	float		holdTime;
	float		fxTime;
	int			channel;
} hudtextparms_t;

class CHelpers
{
public: //UserMessages
	void PrintCenterText(CPlayer *pPlayer, const char *szText);
	void PrintCenterTextAll(const char *szText);
	void PrintHudText(CPlayer *pPlayer, const hudtextparms_t &textparms, const char *szText);
	void PrintHudTextAll(const hudtextparms_t &textparms, const char *szText);
	void SayText2(CPlayer *pPlayer, CPlayer *pAuthor, const char *szText);
	void SayText2All(CPlayer *pAuthor, const char *szText);

public: //Sound functions
	void EmitAmbientSound(const char *szPath, const Vector pos,
									int entity = SOUND_FROM_WORLD,
									int level = SNDLEVEL_NORMAL,
									int flags = SND_NOFLAGS,
									float volume = SNDVOL_NORMAL,
									int pitch = SNDPITCH_NORMAL,
									float delay = 0.0);
	//void EmitSound(const int clients[], unsigned int numClients, const char *szPath, int entity, int channel,
							//int level, int flags, float volume, int pitch, int speakerentity,
							//const Vector vecOrigin, const Vector vecDirection, bool updatePos, float soundtime);
	void EmitSoundToClient(CPlayer *pPlayer, const char *szPath, int entity = SOUND_FROM_LOCAL_PLAYER, int channel = SNDCHAN_AUTO,
							int level = SNDLEVEL_NORMAL, int flags = SND_NOFLAGS, float volume = SNDVOL_NORMAL,
							int pitch = SNDPITCH_NORMAL, int speakerentity = -1, const Vector *vecOrigin = NULL,
							const Vector *vecDirection = NULL, bool updatePos = true, float soundtime = 0.0);
	void EmitSoundToAll(const char *szPath, int entity, int channel, int level,
							int flags, float volume, int pitch, int speakerentity, const Vector *vecOrigin,
							const Vector *vecDirection, bool updatePos, float soundtime);

public: //traceray functions
	tr_contents *TR_TraceRayFilter(const Vector origin, const QAngle angles, int flags, RayType rtype, CPlayer *pPlayer);

public: //Utilities
	CPlayer* UTIL_PlayerByIndex(int playerIndex);

public: //String Tables
	bool AddFileToDownloadTable(const char *path);
	void AddFolderToDownloadTable(const char *path);
	int FindStringTable(const char *table);
	bool AddToStringTable(int tableidx, const char *str);
	bool LockStringTables(bool lock);

public: //Scanned Functions
	int LoadEventsFromFile(IGameEventManager2 *pThis, const char *filename);
	void KeyValuesSetString(KeyValues *pKeyValues, const char *keyName, const char *value);
	CBaseEntity *CreateEntityByName(const char *className, int iForceEdictIndex = -1);
	int DispatchSpawn(CBaseEntity *pEntity);
	CBaseEntity *GiveNamedItem(CBaseEntity *player, const char *weapon, int iSubType, CScriptCreatedItem *item, bool bUnknown);
	void WeaponEquip(CBaseEntity *player, CBaseEntity *weapon);
	//float PlayScene(CBaseEntity *player, const char *pszScene, float flDelay, AI_Response *response, IRecipientFilter *filter);
};

extern CHelpers *pHelpers;

typedef int (__fastcall *LoadEventsFromFileFunction)(IGameEventManager2 * /*this*/, int /*dummy*/, const char *);
typedef void (__fastcall *KVSetStringFunction)(KeyValues * /*this*/, int /*dummy*/, const char *, const char *);
typedef CBaseEntity* (*CreateEntityByNameFunction)(const char *, int);
typedef int (*DispatchSpawnFunction)(CBaseEntity *);
typedef CBaseEntity* (__fastcall *GiveNamedItemFunction) (void* /*this*/, int /*dummy*/, const char *, int, CScriptCreatedItem *, bool);
typedef void (__fastcall *WeaponEquipFunction) (void * /*this*/, int /*dummy*/, CBaseEntity *);
//typedef float (__fastcall *PlaySceneFunction) (void * /*this*/, int /*dummy*/, const char *, float, AI_Response *, IRecipientFilter *);

#define StrEqual(first,second) strcmp(first,second) == 0

#endif //_INCLUDE_CHELPERS_H_