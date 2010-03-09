#include <sourcemod>
#include <sdktools>

#define VERSION "0.4"

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

new g_iRocketLastTeam = TEAM_RED;

new Handle:g_hSpeedMul = INVALID_HANDLE;
//new Handle:g_SentryRocketCreate = INVALID_HANDLE;
new Handle:g_RocketRespawnTimer = INVALID_HANDLE;

public OnPluginStart()
{
	CreateConVar("sm_rocket_version", VERSION, "Rocket Version", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_REPLICATED|FCVAR_NOTIFY);
	
	/*
	RegAdminCmd("sm_sdkrocket", Command_SDKRocket, ADMFLAG_SLAY);
	
	RegAdminCmd("sm_rocket", Command_ForceRocket, ADMFLAG_SLAY);
	RegAdminCmd("sm_rocket_red", Command_ForceRocketRed, ADMFLAG_SLAY);
	RegAdminCmd("sm_rocket_blue", Command_ForceRocketBlue, ADMFLAG_SLAY);
	*/
	
	g_hSpeedMul = FindConVar("sm_sentryrocket_speedmul");
	
	/*
	StartPrepSDKCall(SDKCall_Static);
	PrepSDKCall_SetSignature(SDKLibrary_Server, "\x8B\x44\x24\x0C\x8B\x4C\x24\x08\x8B\x54\x24\x04\x56\x50\x51\x52", 16);
	PrepSDKCall_AddParameter(SDKType_Vector, SDKPass_ByRef);
	PrepSDKCall_AddParameter(SDKType_QAngle, SDKPass_ByRef);
	PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Pointer);
	PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Pointer);
	g_SentryRocketCreate = EndPrepSDKCall();
	*/
	
	HookEvent("teamplay_setup_finished", Event_teamplay_setup_finished);
}

public Action:Event_teamplay_setup_finished(Handle:event, const String:name[], bool:dontBroadcast) {
	if (g_RocketRespawnTimer != INVALID_HANDLE) {
		CloseHandle(g_RocketRespawnTimer);
		g_RocketRespawnTimer = INVALID_HANDLE;
	}
	g_RocketRespawnTimer = CreateTimer(1.0, SpawnRockets, _, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
	
	return Plugin_Continue;
}

public Action:SpawnRockets(Handle:timer)
{
	if (g_iRocketLastTeam == TEAM_BLUE) {
		fireTeamProjectile(TEAM_RED, PROJECTILE_ROCKET);
		g_iRocketLastTeam = TEAM_RED;
	}
	if (g_iRocketLastTeam == TEAM_RED) {
		fireTeamProjectile(TEAM_BLUE, PROJECTILE_ROCKET);
		g_iRocketLastTeam = TEAM_BLUE;
	}
	return Plugin_Continue;
}

/*
public Action:Command_SDKRocket(client, args)
{
	if(GetEntityCount() >= GetMaxEntities()-32)
	{
		PrintToChat(client, "[SM] Entity limit is reached. Can't spawn anymore rockets. Change maps.");
		return Plugin_Handled;
	}
	
	new iTeam = GetClientTeam(client);
	decl Float:vPosition[3];
	decl Float:vAngles[3];
	
	new launcherIndex = findNextTeamLaunchPosition(iTeam);
	
	GetEntPropVector(launcherIndex, Prop_Data, "m_vecOrigin", vPosition);
	GetEntPropVector(launcherIndex, Prop_Data, "m_angRotation", vAngles);
	
	SDKCall(g_SentryRocketCreate, vPosition, vAngles, 0, 0);
	
	return Plugin_Handled;
}


public Action:Command_ForceRocket(client, args)
{
	if(GetEntityCount() >= GetMaxEntities()-32)
	{
		PrintToChat(client, "[SM] Entity limit is reached. Can't spawn anymore rockets. Change maps.");
		return Plugin_Handled;
	}
	
	new iTeam = GetClientTeam(client);
	fireTeamProjectile(iTeam, PROJECTILE_ROCKET);
	
	return Plugin_Handled;
}

public Action:Command_ForceRocketRed(client, args)
{
	if(GetEntityCount() >= GetMaxEntities()-32)
	{
		PrintToChat(client, "[SM] Entity limit is reached. Can't spawn anymore rockets. Change maps.");
		return Plugin_Handled;
	}
	
	fireTeamProjectile(TEAM_RED, PROJECTILE_ROCKET);
	
	return Plugin_Handled;
}

public Action:Command_ForceRocketBlue(client, args)
{
	if(GetEntityCount() >= GetMaxEntities()-32)
	{
		PrintToChat(client, "[SM] Entity limit is reached. Can't spawn anymore rockets. Change maps.");
		return Plugin_Handled;
	}
	
	fireTeamProjectile(TEAM_BLUE, PROJECTILE_ROCKET);
	
	return Plugin_Handled;
}
*/

fireTeamProjectile(iTeam, iType = PROJECTILE_ROCKET) {
	decl Float:vPosition[3];
	decl Float:vAngles[3];
	
	new launcherIndex = findNextTeamLaunchPosition(iTeam);
	
	GetEntPropVector(launcherIndex, Prop_Data, "m_vecOrigin", vPosition);
	GetEntPropVector(launcherIndex, Prop_Data, "m_angRotation", vAngles);
	
	return fireProjectile(vPosition, vAngles, (1100.0*GetConVarFloat(g_hSpeedMul)), iTeam, iType, true)
}

findNextTeamLaunchPosition(iTeam)
{
	static currentlauncherIndex_Red = -1;
	static currentlauncherIndex_Blue = -1;
	
	new launcherIndex = -1;
	new String:targetName[32] = "";
	new bool:spawnFound = false;	
	
	if (iTeam == TEAM_RED)
		launcherIndex = currentlauncherIndex_Red;
	else if (iTeam == TEAM_BLUE)
		launcherIndex = currentlauncherIndex_Blue;
	
	while ((launcherIndex = FindEntityByClassname(launcherIndex, "info_target")) != -1) {
		GetEntPropString(launcherIndex, Prop_Data, "m_iName", targetName, 32);
		if (iTeam == TEAM_RED && StrEqual(targetName, "rocket_spawn_red", false)) {
			spawnFound = true;
			currentlauncherIndex_Red = launcherIndex;
			break;
		} else if (iTeam == TEAM_BLUE && StrEqual(targetName, "rocket_spawn_blue", false)) {
			spawnFound = true;
			currentlauncherIndex_Blue = launcherIndex;
			break;
		}
	}
	
	if (!spawnFound) {
		if (iTeam == TEAM_RED)
			currentlauncherIndex_Red = -1;
		else if (iTeam == TEAM_BLUE)
			currentlauncherIndex_Blue = -1;
		launcherIndex = findNextTeamLaunchPosition(iTeam);
	}
	
	return launcherIndex
}

fireProjectile(Float:vPosition[3], Float:vAngles[3] = NULL_VECTOR, Float:fSpeed = 1100.0, iTeam = TEAM_SPEC, iType = PROJECTILE_ROCKET, bool:bCritical = false)
{
	new String:strClassname[32] = "";
	new String:strEntname[32] = "";
	
	switch (iType) {
		case PROJECTILE_ROCKET:
		{
			strClassname = "CTFProjectile_Rocket";
			strEntname = "tf_projectile_rocket";
		}
		case PROJECTILE_ROCKET_SENTRY:
		{
			strClassname = "CTFProjectile_SentryRocket";
			strEntname = "tf_projectile_sentryrocket";
		}
		default:
		{
			return -1;
		}
	}

	new iRocket = CreateEntityByName(strEntname);
	
	if(!IsValidEntity(iRocket))
		return -1;
	
	decl Float:vVelocity[3];
	decl Float:vBuffer[3];
	
	GetAngleVectors(vAngles, vBuffer, NULL_VECTOR, NULL_VECTOR);
	
	vVelocity[0] = vBuffer[0]*fSpeed;
	vVelocity[1] = vBuffer[1]*fSpeed;
	vVelocity[2] = vBuffer[2]*fSpeed;
	
	TeleportEntity(iRocket, vPosition, vAngles, vVelocity);
	
	SetEntData(iRocket, FindSendPropOffs(strClassname, "m_nSkin"), (iTeam-2), 1, true);
	SetEntData(iRocket, FindSendPropOffs(strClassname, "m_bCritical"), bCritical, true);
	
	SetVariantInt(iTeam);
	AcceptEntityInput(iRocket, "TeamNum", -1, -1, 0);

	SetVariantInt(iTeam);
	AcceptEntityInput(iRocket, "SetTeam", -1, -1, 0); 
	
	//SetRocketDamage(iRocket, someFloatProbaly); // BLAAAARGH Need to write this :'(
	
	DispatchSpawn(iRocket);
	
	return iRocket;
}