#pragma semicolon 1

#include <sourcemod>
#define REQUIRE_EXTENSIONS
#include <sdktools>
#include <sdkhooks>
#include <tf2>
#include <tf2_stocks>

#define TEAM_SPEC 1
#define TEAM_RED 2
#define TEAM_BLUE 3

#define PROJECTILE_ROCKET 1
#define PROJECTILE_ROCKET_SENTRY 2

#define CONVAR_COUNT 9
#define MAX_FAILED_LAUNCHER_SEARCHES 2

public Plugin:myinfo = {
	name = "TFDodgeball",
	author = "Asherkin",
	description	= "An open-source version of the popular 'Dodgeball' gameplay modification.",
	version = "1.2.3",
	url = "http://limetech.org/"
};

public Extension:__ext_tfdodgeball = 
{
	name = "TFDodgeball",
	file = "tfdodgeball.ext",
	autoload = 1,
	required = 1,
}

new Handle:g_hRocketSpawnTimer = INVALID_HANDLE;
new Handle:g_hConVars[CONVAR_COUNT] = {INVALID_HANDLE, ...};

new bool:g_config_bEnabled;
new g_config_iMaxRockets;
new Float:g_config_flBaseDamage;
new bool:g_config_bSpawnCriticals;
new Float:g_config_flSpeedMul;
new bool:g_config_bAutoJoin;
new bool:g_config_bGameDesc;
new bool:g_config_bManiFix;

new g_iRocketCount;
new bool:g_bMapRoaded;

new currentlauncherIndex_Red = -1;
new currentlauncherIndex_Blue = -1;

public OnPluginStart()
{
	RegAdminCmd("sm_dodgeball_rocket", Command_ForceRocket, ADMFLAG_SLAY);
	RegAdminCmd("sm_dodgeball_headrocket", Command_HeadRocket, ADMFLAG_SLAY);
	
	g_hConVars[0] = FindConVar("sm_dodgeball_enabled");
	g_hConVars[1] = CreateConVar("sm_dodgeball_spawninterval", "1.0", "", FCVAR_NONE, true, 0.0, false);
	g_hConVars[2] = CreateConVar("sm_dodgeball_maxrockets", "10", "", FCVAR_NONE, true, 0.0, false);
	g_hConVars[3] = CreateConVar("sm_dodgeball_basedamage", "15.0", "", FCVAR_NONE, true, 0.0, false);
	g_hConVars[4] = CreateConVar("sm_dodgeball_criticals", "1", "", FCVAR_NONE, true, 0.0, true, 1.0);
	g_hConVars[5] = FindConVar("sm_dodgeball_speedmul");
	g_hConVars[6] = CreateConVar("sm_dodgeball_autojoin", "1", "", FCVAR_NONE, true, 0.0, true, 1.0);
	g_hConVars[7] = CreateConVar("sm_dodgeball_gamedesc", "1", "", FCVAR_NONE, true, 0.0, true, 1.0);
	g_hConVars[8] = CreateConVar("sm_dodgeball_gamedesc_manifix", "0", "", FCVAR_NONE, true, 0.0, true, 1.0);
	
	g_config_bEnabled = true;
	g_config_iMaxRockets = 10;
	g_config_flBaseDamage = 15.0;
	g_config_bSpawnCriticals = true;
	g_config_flSpeedMul = 0.5;
	g_config_bAutoJoin = true;
	g_config_bGameDesc = false;
	g_config_bManiFix = false;
	
	HookConVarChange(g_hConVars[0], config_bEnabled_changed);
	HookConVarChange(g_hConVars[1], config_flSpawnInterval_changed);
	HookConVarChange(g_hConVars[2], config_iMaxRockets_changed);
	HookConVarChange(g_hConVars[3], config_flBaseDamage_changed);
	HookConVarChange(g_hConVars[4], config_bSpawnCriticals_changed);
	HookConVarChange(g_hConVars[5], config_flSpeedMul_changed);
	HookConVarChange(g_hConVars[6], config_bAutoJoin_changed);
	HookConVarChange(g_hConVars[7], config_bGameDesc_changed);
	HookConVarChange(g_hConVars[8], config_bManiFix_changed);
	
	HookEvent("teamplay_round_start", Event_TeamplayRoundStart);
	HookEvent("teamplay_setup_finished", Event_TeamplaySetupFinished);
}

public OnClientPutInServer(client) {
	if (g_config_bEnabled)
	{
		if (g_config_bAutoJoin)
		{
			FakeClientCommandEx(client, "jointeam 0");
		}
	}
}

public OnMapStart()
{
	g_bMapRoaded = true;
}

public OnMapEnd()
{
	g_bMapRoaded = false;
	
	currentlauncherIndex_Red = -1;
	currentlauncherIndex_Blue = -1;
}

public Action:OnGetGameDescription(String:gameDesc[64])
{
	if (g_config_bGameDesc && (g_bMapRoaded || !g_config_bManiFix))
	{
		strcopy(gameDesc, sizeof(gameDesc), "TFDodgeball");
		return Plugin_Changed;
	}
	
	return Plugin_Continue;
}

public Event_TeamplayRoundStart(Handle:event, const String:name[], bool:dontBroadcast) {
	new bool:full = GetEventBool(event, "full_reset");
	if (!full)
		return;
	
	if (g_hRocketSpawnTimer != INVALID_HANDLE)
	{
		CloseHandle(g_hRocketSpawnTimer); // Check Handle
		g_hRocketSpawnTimer = INVALID_HANDLE;
	}
}

public Event_TeamplaySetupFinished(Handle:event, const String:name[], bool:dontBroadcast) {
	g_hRocketSpawnTimer = CreateTimer(GetConVarFloat(g_hConVars[1]), SpawnRockets, _, TIMER_REPEAT);
}

public config_bEnabled_changed(Handle:convar, const String:oldValue[], const String:newValue[]) { g_config_bEnabled = bool:StringToInt(newValue); }
public config_iMaxRockets_changed(Handle:convar, const String:oldValue[], const String:newValue[]) { g_config_iMaxRockets = StringToInt(newValue); }
public config_flBaseDamage_changed(Handle:convar, const String:oldValue[], const String:newValue[]) { g_config_flBaseDamage = StringToFloat(newValue); }
public config_bSpawnCriticals_changed(Handle:convar, const String:oldValue[], const String:newValue[]) { g_config_bSpawnCriticals = bool:StringToInt(newValue); }
public config_flSpeedMul_changed(Handle:convar, const String:oldValue[], const String:newValue[]) { g_config_flSpeedMul = StringToFloat(newValue); }
public config_bAutoJoin_changed(Handle:convar, const String:oldValue[], const String:newValue[]) { g_config_bAutoJoin = bool:StringToInt(newValue); }
public config_bGameDesc_changed(Handle:convar, const String:oldValue[], const String:newValue[]) { g_config_bGameDesc = bool:StringToInt(newValue); }
public config_bManiFix_changed(Handle:convar, const String:oldValue[], const String:newValue[]) { g_config_bManiFix = bool:StringToInt(newValue); }

public config_flSpawnInterval_changed(Handle:convar, const String:oldValue[], const String:newValue[])
{
	if (g_hRocketSpawnTimer != INVALID_HANDLE)
	{
		CloseHandle(g_hRocketSpawnTimer); // Check Handle
		g_hRocketSpawnTimer = CreateTimer(StringToFloat(newValue), SpawnRockets, _, TIMER_REPEAT);
	}
}

public Action:SpawnRockets(Handle:timer)
{
	if (!g_config_bEnabled || (g_config_iMaxRockets && (g_iRocketCount >= g_config_iMaxRockets)))
		return Plugin_Continue;
	
	static iRocketLastTeam = TEAM_RED;
	
	new rocketEnt;
	if (iRocketLastTeam == TEAM_BLUE) {
		rocketEnt = fireTeamProjectile(TEAM_RED, PROJECTILE_ROCKET);
		iRocketLastTeam = TEAM_RED;
	} else if (iRocketLastTeam == TEAM_RED) {
		rocketEnt = fireTeamProjectile(TEAM_BLUE, PROJECTILE_ROCKET);
		iRocketLastTeam = TEAM_BLUE;
	}
	//SDKHook(rocketEnt, SDKHook_StartTouch, OnRocketDestroyed);
	g_iRocketCount++;
	return Plugin_Continue;
}

/*public OnRocketDestroyed(entity, other)
{
	g_iRocketCount--;
	
	if (g_iRocketCount < 0)
		g_iRocketCount = 0;
}*/

public OnEntityDestroyed(entity)
{
	if (!g_config_bEnabled) return;
	
	new String:netClassName[32];
	GetEntityNetClass(entity, netClassName, 32);
	if (StrEqual(netClassName, "CTFProjectile_Rocket", false) || StrEqual(netClassName, "CTFProjectile_SentryRocket", false))
	{
		g_iRocketCount--;
	
		if (g_iRocketCount < 0)
			g_iRocketCount = 0;
	}
}

public Action:Command_ForceRocket(client, args)
{
	if (!g_config_bEnabled) return Plugin_Continue;
	
	new String:arg1[32];
	GetCmdArg(1, arg1, 32);
	fireTeamProjectile(StringToInt(arg1), PROJECTILE_ROCKET);
	return Plugin_Handled;
}

public Action:Command_HeadRocket(client, args)
{
	if (!g_config_bEnabled) return Plugin_Continue;
	
	new Float:vAngles[3];
	new Float:vPosition[3];

	GetClientEyeAngles(client, vAngles);
	GetClientEyePosition(client, vPosition);
	
	fireProjectile(vPosition, vAngles, (1100.0*g_config_flSpeedMul), g_config_flBaseDamage, GetClientTeam(client), PROJECTILE_ROCKET, g_config_bSpawnCriticals);
	
	return Plugin_Handled;
}

fireTeamProjectile(iTeam, iType = PROJECTILE_ROCKET) {
	decl Float:vPosition[3];
	decl Float:vAngles[3];
	
	new launcherIndex = findNextTeamLaunchPosition(iTeam);
	
	GetEntPropVector(launcherIndex, Prop_Data, "m_vecOrigin", vPosition);
	GetEntPropVector(launcherIndex, Prop_Data, "m_angRotation", vAngles);
	
	return fireProjectile(vPosition, vAngles, (1100.0*g_config_flSpeedMul), g_config_flBaseDamage, iTeam, iType, g_config_bSpawnCriticals);
}

findNextTeamLaunchPosition(iTeam)
{
	static iFailedAttemptsToFindLauncher = 0;
	
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
	
	if (iFailedAttemptsToFindLauncher >= MAX_FAILED_LAUNCHER_SEARCHES)
	{
		SetFailState("No launchers found in map for team %d.", iTeam);
	}
	
	if (!spawnFound) {
		if (iTeam == TEAM_RED)
			currentlauncherIndex_Red = -1;
		else if (iTeam == TEAM_BLUE)
			currentlauncherIndex_Blue = -1;
		iFailedAttemptsToFindLauncher += 1;
		launcherIndex = findNextTeamLaunchPosition(iTeam);
	}
	
	iFailedAttemptsToFindLauncher = 0;
	return launcherIndex;
}

fireProjectile(Float:vPosition[3], Float:vAngles[3] = NULL_VECTOR, Float:flSpeed = 1100.0, Float:flDamage = 90.0, iTeam = TEAM_SPEC, iType = PROJECTILE_ROCKET, bool:bCritical = false)
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
	
	vVelocity[0] = vBuffer[0]*flSpeed;
	vVelocity[1] = vBuffer[1]*flSpeed;
	vVelocity[2] = vBuffer[2]*flSpeed;
	
	TeleportEntity(iRocket, vPosition, vAngles, vVelocity);
	
	SetEntData(iRocket, FindSendPropOffs(strClassname, "m_nSkin"), (iTeam-2), 1, true);
	SetEntData(iRocket, FindSendPropOffs(strClassname, "m_bCritical"), bCritical, true);
	SetEntDataFloat(iRocket, FindSendPropOffs(strClassname, "m_iDeflected") + 4, flDamage, true); // Credit to voogru
	
	SetVariantInt(iTeam);
	AcceptEntityInput(iRocket, "TeamNum", -1, -1, 0);

	SetVariantInt(iTeam);
	AcceptEntityInput(iRocket, "SetTeam", -1, -1, 0); 
	
	DispatchSpawn(iRocket);
	
	return iRocket;
}
