#ifndef CTFDBCreatedItem_h__
#define CTFDBCreatedItem_h__

#include "CScriptCreatedItem.h"

class CTFDBCreatedItem: public CScriptCreatedItem
{
public:
	int GetLoadoutSlot(int playerClass);
};
#endif // CTFDBCreatedItem_h__
