#include "extension.h"
#include "CTFDBServerCommand.h"
#include "CEntity.h"

LINK_ENTITY_TO_CLASS(CPointServerCommand, CTFDBServerCommand);

ConVar BlockPointServerCommand("sm_dodgeball_block_point_servercommand", "1", FCVAR_NONE, "", true, 0.0, true, 1.0);

void CTFDBServerCommand::Spawn(void)
{
	BaseClass::Spawn();

	if (DodgeballEnabled.GetBool() && BlockPointServerCommand.GetBool())
		g_pSM->LogError(myself, "Warning: Server spawned a point_servercommand entity, while TFDB will try and filter it from changing TFDB settings, you should stop using this map.");
}

void CTFDBServerCommand::InputCommand(inputdata_t *inputdata)
{
	if (!DodgeballEnabled.GetBool() || !BlockPointServerCommand.GetBool())
		return BaseClass::InputCommand(inputdata);

	if (V_stristr(inputdata->value.String(), "sm_dodgeball_") == NULL)
		return BaseClass::InputCommand(inputdata);

	g_pSM->LogMessage(myself, "Warning: point_servercommand entity tried to execute \"%s\" which contains a TFDB convar.", inputdata->value.String());
}
