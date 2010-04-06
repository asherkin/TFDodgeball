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
*
* CEntity Entity Handling Framework version 1.0 by Matt 'pRED*' Woodrow
*
* - Credits:
*		- This is largely (or entirely) based on a concept by voogru - http://voogru.com/
*		- The virtual function hooking is powered by the SourceHook library by Pavol "PM OnoTo" Marko.
*
* - About:
*		- CEntity is (and its derived classes are) designed to emulate the CBaseEntity class from the HL2 SDK.
*		- Valve code (like entire class definitions and CBaseEntity functions) from the SDK should almost just work when copied into this.
*			- References to CBaseEntity need to be changed to CEntity.
*			- Sendprops and datamaps are pointers to the actual values so references to these need to be dereferenced.
*				- Code that uses unexposed data members won't work - Though you could reimplement these manually.
*		- Virtual functions handle identically to ones in a real derived class.
*			- Calls from valve code to a virtual in CEntity (with no derived versions) fall back directly to the valve code.
*			- Calls from valve code to a virtual (with a derived version) will call that code, and the valve code can be optionally run using BaseClass::Function().
*
*			- Calls from your code to a virtual in CEntity (with no derived versions) will make a call to the valve code.
*			- Calls from your code to a virtual (with a derived version) will call that code, and the valve code can be optionally run using BaseClass::Function().
*			
*
* - Notes:
*		- If you inherit Init() or Destroy() in a derived class, I would highly recommend calling the BaseClass equivalent.
* 
* - TODO (in no particular order):
*		- Add handling of custom keyvalues commands
*			- Add datamapping to class values so keyvalues can parse to them
*		- Add handling of Inputs
*		- Include more CEntity virtuals and props/datamaps
*		- Create more derived classes
*		- Include more Think/Touch etc handlers
*			- Can we access the actual valve internal m_pfnThink somehow
*			- Valve code now has lists of thinks, can we access this?
*		- Forcibly deleting entities?
*		- Handling of custom entity names in Create
*			- Requires a pre-hook to switch out the custom string with one it can actually handle
*				- Probably need a new LINK_ENTITY_TO_CUSTOM_CLASS to define which real entity name to use instead
*			- Need to hook FindFactory and return the matched real entity factory so CanCreate (sp?) will succeed.
*		- Support mods other than TF2 (CPlayer should only contain CBasePlayer sdk stuff and create optional CTFPlayer/CCSPlayer derives)
*
*	- Change log
*		- 1.0
*			- Initial import of basic CEntity and CPlayer
*/

#ifndef _INCLUDE_CENTITY_H_
#define _INCLUDE_CENTITY_H_

#include "extension.h"
#include "IEntityFactory.h"
#include "vector.h"
#include "server_class.h"

#include "../game/shared/ehandle.h"
class CBaseEntity;
typedef CHandle<CBaseEntity> EHANDLE;
#include "../game/shared/takedamageinfo.h"

class CEntity;

#define DECLARE_DEFAULTHEADER(name, ret, params) \
	ret Internal##name params; \
	bool m_bIn##name;

#define SetThink(a) ThinkSet(static_cast <void (CEntity::*)(void)> (a), 0, NULL)
typedef void (CEntity::*BASEPTR)(void);

class CEntity // : public CBaseEntity  - almost.
{
public: // CEntity
	DECLARE_CLASS_NOBASE(CEntity);

	virtual void Init(edict_t *pEdict, CBaseEntity *pBaseEntity, bool addHooks);
	virtual void Destroy();
	CBaseEntity *BaseEntity();

public: // CBaseEntity virtuals
	virtual void Teleport(const Vector *origin, const QAngle* angles, const Vector *velocity);
	virtual void UpdateOnRemove();
	virtual void Spawn();
	virtual int OnTakeDamage(const CTakeDamageInfo &info);
	virtual void Think();
	virtual void StartTouch(CBaseEntity *entity);
	virtual void Touch(CBaseEntity *entity);
	virtual void EndTouch(CBaseEntity *entity);

public: // CBaseEntity non virtual helpers
	BASEPTR	ThinkSet(BASEPTR func, float thinkTime, const char *szContext);
	void SetNextThink(float thinkTime, const char *szContext = NULL);
	void CheckHasThinkFunction(bool isThinking);
	bool WillThink();

	void AddEFlags(int nEFlagMask);
	void RemoveEFlags(int nEFlagMask);
	bool IsEFlagSet(int nEFlagMask) const;

	const char* GetClassname();

	int GetTeamNumber()  const;
	virtual void ChangeTeam(int iTeamNum);
	bool InSameTeam(CEntity *pEntity) const;

	const Vector &GetLocalOrigin() const;
	const Vector &GetAbsVelocity() const;

	CEntity *GetMoveParent();

	edict_t *edict();
	int entindex();

	static CEntity *Instance(const CBaseHandle &hEnt);
	static CEntity *Instance(const edict_t *pEnt);
	static CEntity *Instance(edict_t *pEnt);
	static CEntity* Instance(int iEnt);
	static CEntity* Instance(CBaseEntity *pEnt);

	virtual	bool IsPlayer();
	int GetTeam();

private: // All the internal hook implementations for the above virtuals
	DECLARE_DEFAULTHEADER(Teleport, void, (const Vector *origin, const QAngle* angles, const Vector *velocity));
	DECLARE_DEFAULTHEADER(UpdateOnRemove, void, ());
	DECLARE_DEFAULTHEADER(Spawn, void, ());
	DECLARE_DEFAULTHEADER(OnTakeDamage, int, (const CTakeDamageInfo &info));
	DECLARE_DEFAULTHEADER(Think, void, ());
	DECLARE_DEFAULTHEADER(StartTouch, void, (CBaseEntity*));
	DECLARE_DEFAULTHEADER(Touch, void, (CBaseEntity*));
	DECLARE_DEFAULTHEADER(EndTouch, void, (CBaseEntity*));

protected: // CEntity
	CBaseEntity *m_pEntity;
	edict_t *m_pEdict;

protected: //Sendprops
	uint8_t *m_iTeamNum;
	Vector *m_vecOrigin;
	uint8_t *m_CollisionGroup;
	CBaseHandle *m_hOwnerEntity;
	uint16_t *m_fFlags;

protected: //Datamaps
	Vector *m_vecAbsVelocity;
	string_t *m_iClassname;
	matrix3x4_t	*m_rgflCoordinateFrame;
	Vector *m_vecVelocity;
	CBaseHandle *m_hMoveParent;
	int *m_iEFlags;

	/* Thinking Stuff */
	void (CEntity::*m_pfnThink)(void);
	int *m_nNextThinkTick;
};

/* All the macros to make everything above smaller */

#define DECLARE_DEFAULTHANDLER(type, name, ret, params, paramscall) \
	ret type::name params \
	{ \
		if (!m_bIn##name) \
		{ \
			return SH_MCALL(BaseEntity(), name) paramscall; \
		} \
		SET_META_RESULT(MRES_IGNORED); \
		SH_GLOB_SHPTR->DoRecall(); \
		SourceHook::EmptyClass *thisptr = reinterpret_cast<SourceHook::EmptyClass*>(SH_GLOB_SHPTR->GetIfacePtr()); \
		RETURN_META_VALUE(MRES_SUPERCEDE, (thisptr->*(__SoureceHook_FHM_GetRecallMFP##name(thisptr))) paramscall); \
	} \
	ret type::Internal##name params \
	{ \
		SET_META_RESULT(MRES_SUPERCEDE); \
		type *pEnt = (type *)CEntity::Instance(META_IFACEPTR(CBaseEntity)); \
		if (!pEnt) \
		{ \
			RETURN_META_VALUE(MRES_IGNORED, (ret)0); \
		} \
		int index = pEnt->entindex(); \
		pEnt->m_bIn##name = true; \
		ret retvalue = pEnt->name paramscall; \
		pEnt = (type *)CEntity::Instance(index); \
		if (pEnt) \
			pEnt->m_bIn##name = false; \
		return retvalue; \
	}

#define DECLARE_DEFAULTHANDLER_void(type, name, params, paramscall) \
	void type::name params \
	{ \
		if (!m_bIn##name) \
		{ \
			SH_MCALL(BaseEntity(), name) paramscall; \
			return; \
		} \
		SET_META_RESULT(MRES_IGNORED); \
		SH_GLOB_SHPTR->DoRecall(); \
		SourceHook::EmptyClass *thisptr = reinterpret_cast<SourceHook::EmptyClass*>(SH_GLOB_SHPTR->GetIfacePtr()); \
		(thisptr->*(__SoureceHook_FHM_GetRecallMFP##name(thisptr))) paramscall; \
		SET_META_RESULT(MRES_SUPERCEDE); \
	} \
	void type::Internal##name params \
	{ \
		SET_META_RESULT(MRES_SUPERCEDE); \
		type *pEnt = (type *)CEntity::Instance(META_IFACEPTR(CBaseEntity)); \
		if (!pEnt) \
		{ \
			RETURN_META(MRES_IGNORED); \
		} \
		int index = pEnt->entindex(); \
		pEnt->m_bIn##name = true; \
		pEnt->name paramscall; \
		pEnt = (type *)CEntity::Instance(index); \
		if (pEnt) \
			pEnt->m_bIn##name = false; \
	}

#define ADD_DEFAULTHANDLER_HOOK(type,name) \
	if (addHooks) \
		SH_ADD_MANUALVPHOOK(name, BaseEntity(), SH_MEMBER(this, &type::Internal##name), false); \
	m_bIn##name = false;
#define REMOVE_DEFAULTHANDLER_HOOK(type,name) \
	SH_REMOVE_MANUALHOOK(name, BaseEntity(), SH_MEMBER(this, &type::Internal##name), false);

#define GET_SENDPROP_POINTER(type, edict, entity, info, prop) \
	if (!gamehelpers->FindSendPropInfo(edict->GetNetworkable()->GetServerClass()->GetName(), #prop, info)) \
		prop = NULL; \
	else \
		prop = (type *)((uint8_t *)entity + (info)->actual_offset);

#define GET_DATAMAP_POINTER(type, entity, map, prop) \
	map = gamehelpers->GetDataMap(entity); \
	if (!map) \
	prop = NULL; \
	else \
	{ \
		typedescription_t *typedesc = gamehelpers->FindInDataMap(map, #prop); \
		if (!typedesc) \
			prop = NULL; \
		else \
			prop = (type *)((uint8_t *)entity + typedesc->fieldOffset[TD_OFFSET_NORMAL]); \
		map = NULL; \
	}

#endif // _INCLUDE_CENTITY_H_
