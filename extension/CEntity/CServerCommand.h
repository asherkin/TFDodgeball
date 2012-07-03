#ifndef _INCLUDE_CServerCommand_H_
#define _INCLUDE_CServerCommand_H_

#include "CEntityManager.h"
#include "CEntity.h"

class CServerCommand : public CEntity
{
public:
	DECLARE_CLASS(CServerCommand, CEntity);
	
public:
	virtual void InputCommand(inputdata_t *inputdata);
	
public:
	DECLARE_DEFAULTHEADER_DETOUR(InputCommand, void, (inputdata_t *inputdata));
};

#endif // _INCLUDE_CServerCommand_H_
