#ifndef _INCLUDE_CENTITYBASE_H_
#define _INCLUDE_CENTITYBASE_H_

class CEntity;
class CBaseHandle;
class CBaseEntity;
struct edict_t;

class CEntityLookup
{
public:
	static CEntity *Instance(const CBaseHandle &hEnt);
	static CEntity *Instance(const edict_t *pEnt);
	static CEntity *Instance(edict_t *pEnt);
	static CEntity* Instance(int iEnt);
	static CEntity* Instance(CBaseEntity *pEnt);
};

#endif // _INCLUDE_CENTITYBASE_H_
