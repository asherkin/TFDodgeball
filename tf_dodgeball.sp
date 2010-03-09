#pragma semicolon 1 // Force strict semicolon mode.

#include <sourcemod>
#define REQUIRE_EXTENSIONS
#include <tf2items>
#include <sdktools>
#include <tf2>
#include <tf2_stocks>

#define PLUGIN_NAME		"[TF2Items] Dogdeball Flamethrower"
#define PLUGIN_AUTHOR		"Asherkin"
#define PLUGIN_VERSION		"3.0.0"
#define PLUGIN_CONTACT		"http://limetech.org/"

public Plugin:myinfo = {
	name			= PLUGIN_NAME,
	author			= PLUGIN_AUTHOR,
	description	= PLUGIN_NAME,
	version		= PLUGIN_VERSION,
	url				= PLUGIN_CONTACT
};

public OnPluginStart() {
	LoadTranslations("common.phrases");
	HookEvent("post_inventory_application", Event_PostInventoryApplication);
	HookEvent("player_spawn", Event_PlayerSpawn);
	HookEvent("teamplay_round_start", Event_TeamplayRoundStart);
	HookEvent("player_changeclass", Event_PlayerChangeClass, EventHookMode_Pre);
}

public OnClientPutInServer(client) {
	FakeClientCommandEx(client, "jointeam 0");
}

public Action:Event_PlayerChangeClass(Handle:event, const String:name[], bool:dontBroadcast)
{
	SetEventInt(event, "class", 3);
	return Plugin_Changed;
}

public Event_TeamplayRoundStart(Handle:event, const String:name[], bool:dontBroadcast) {
	new bool:full = GetEventBool(event, "full_reset");
	
	if (!full)
		return;
		
	new index = -1;
	while ((index = FindEntityByClassname(index, "obj_sentrygun")) != -1) {
		SetEntData(index, FindSendPropOffs("CObjectSentrygun","m_iAmmoShells"), 0, 4, true);
		SetEntData(index, FindSendPropOffs("CObjectSentrygun","m_iAmmoRockets"), 100, 4, true);
	}
	
	CreateTimer(60.0, SentryRefill, _, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}

public Action:SentryRefill(Handle:timer)
{
	new index = -1;
	while ((index = FindEntityByClassname(index, "obj_sentrygun")) != -1) {
		SetEntData(index, FindSendPropOffs("CObjectSentrygun","m_iAmmoRockets"), 100, 4, true);
	}
	return Plugin_Continue;
}

public Event_PostInventoryApplication(Handle:event, const String:name[], bool:dontBroadcast) {
	new client_id = GetEventInt(event, "userid");
	new client = GetClientOfUserId(client_id);
	CreateTimer(0.1, RemoveWeapons, client);
}

public Action:RemoveWeapons(Handle:timer, any:client)
{
	for (new slot = 1;  slot <= 5;  slot++) {
		TF2_RemoveWeaponSlot(client, slot);
	}
	return Plugin_Stop;
}

public Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast) {
	new client_id = GetEventInt(event, "userid");
	new client = GetClientOfUserId(client_id);
	new TFClassType:class = TF2_GetPlayerClass(client);
	
	if (!(class == TFClassType:TFClass_Pyro || class == TFClassType:TFClass_Unknown)) {
		TF2_SetPlayerClass(client, TFClassType:TFClass_Pyro, false, true);
		TF2_RespawnPlayer(client);
	}
}

public Action:TF2Items_OnGiveNamedItem(client, String:strClassName[], iItemDefinitionIndex, &Handle:hItemOverride) {
	if (hItemOverride != INVALID_HANDLE || !(iItemDefinitionIndex == 21 || iItemDefinitionIndex == 40))
		return Plugin_Continue;
	
	new Handle:hWeapon = TF2Items_CreateItem(OVERRIDE_CLASSNAME | OVERRIDE_ITEM_DEF | OVERRIDE_ITEM_LEVEL | OVERRIDE_ITEM_QUALITY | OVERRIDE_ATTRIBUTES);

	TF2Items_SetClassname(hWeapon, "tf_weapon_flamethrower");
	TF2Items_SetItemIndex(hWeapon, 40);
	TF2Items_SetLevel(hWeapon, 50);
	TF2Items_SetQuality(hWeapon, 2);
	
	TF2Items_SetNumAttributes(hWeapon, 8);
	
	TF2Items_SetAttribute(hWeapon, 0, 112, 0.25);
	TF2Items_SetAttribute(hWeapon, 1, 76, 4.0);
	TF2Items_SetAttribute(hWeapon, 2, 1, 0.0);
	TF2Items_SetAttribute(hWeapon, 3, 134, 3.0);
	TF2Items_SetAttribute(hWeapon, 4, 60, 0.0);
	TF2Items_SetAttribute(hWeapon, 5, 66, 0.0);
	TF2Items_SetAttribute(hWeapon, 6, 72, 0.0);
	TF2Items_SetAttribute(hWeapon, 7, 74, 0.0);
	
	hItemOverride = hWeapon;
	return Plugin_Changed;
}

public Action:TF2_CalcIsAttackCritical(client, weapon, String:weaponname[], &bool:result)
{
    if (StrEqual(weaponname, "tf_weapon_rocketlauncher")) {
        result = true;
    } else {
        result = false;
	}
	return Plugin_Handled;
}