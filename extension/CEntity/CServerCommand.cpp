#include "extension.h"
#include "CServerCommand.h"
#include "CEntity.h"

LINK_ENTITY_TO_INTERNAL_CLASS(CPointServerCommand, CServerCommand);

DECLARE_DETOUR(InputCommand, CServerCommand);

DECLARE_DEFAULTHANDLER_DETOUR_void(CServerCommand, InputCommand, (inputdata_t *inputdata), (inputdata));
