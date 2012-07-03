#ifndef _INCLUDE_CTFDBServerCommand_H_
#define _INCLUDE_CTFDBServerCommand_H_

#include "CEntityManager.h"
#include "CServerCommand.h"

class CTFDBServerCommand : public CServerCommand
{
public:
	DECLARE_CLASS(CTFDBServerCommand, CServerCommand);

	virtual void Spawn(void);
	virtual void InputCommand(inputdata_t *inputdata);
};

#endif // _INCLUDE_CTFDBServerCommand_H_
