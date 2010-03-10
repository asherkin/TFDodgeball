#include <sourcemod>
#include <sdktools>

#define VERSION "9001"

#define TEAM_SPEC 1
#define TEAM_RED 2
#define TEAM_BLUE 3

#define PROJECTILE_ROCKET 1
#define PROJECTILE_ROCKET_SENTRY 2

public Plugin:myinfo = 
{
	name = "[TF2] Rockets",
	author = "Asherkin",
	description = "Spawns rockets.",
	version = VERSION,
	url = "http://www.limetech.org"
}

new Handle:g_SentryRocketCreate = INVALID_HANDLE;

public OnPluginStart()
{
	CreateConVar("sm_rocket_version", VERSION, "Rocket Version", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_REPLICATED|FCVAR_NOTIFY);
	
	RegAdminCmd("sm_sdkrocket", Command_SDKRocket, ADMFLAG_SLAY);
	
	StartPrepSDKCall(SDKCall_Static);
	PrepSDKCall_SetSignature(SDKLibrary_Server, "\x8B\x44\x24\x0C\x8B\x4C\x24\x08\x8B\x54\x24\x04\x56\x50\x51\x52", 16); // CTFProjectile_SentryRocket::Create
	PrepSDKCall_AddParameter(SDKType_Vector, SDKPass_ByRef); // Position
	PrepSDKCall_AddParameter(SDKType_QAngle, SDKPass_ByRef); // Angles
	PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Pointer, VDECODE_FLAG_ALLOWWORLD); // Client? Appears to be used for the team atleast.
	PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Pointer, VDECODE_FLAG_ALLOWWORLD); // Unknown
	//PrepSDKCall_SetReturnInfo(SDKType_CBaseEntity, SDKPass_ByRef);
	g_SentryRocketCreate = EndPrepSDKCall();
}

public Action:Command_SDKRocket(client, args)
{
	if(GetEntityCount() >= GetMaxEntities()-32)
	{
		PrintToChat(client, "[SM] Entity limit is reached. Can't spawn anymore rockets. Change maps.");
		return Plugin_Handled;
	}
	
	decl Float:vPosition[3];
	decl Float:vAngles[3];
	
	GetClientEyePosition(client, vPosition);
	GetClientEyeAngles(client, vAngles);
	
	/*new iRocket = */SDKCall(g_SentryRocketCreate, vPosition, vAngles, client, 0);
	//SetEntData(iRocket, FindSendPropOffs("CTFProjectile_SentryRocket", "m_bCritical"), true, true);
	
	return Plugin_Handled;
}