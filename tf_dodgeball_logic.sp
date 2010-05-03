#pragma semicolon 1 // Force strict semicolon mode.

#include <sourcemod>
#define REQUIRE_EXTENSIONS
#include <tf2items>
#include <sdktools>
#include <tf2>
#include <tf2_stocks>

#define PLUGIN_NAME		"[TF2] Dogdeball (Weapon / Player Management)"
#define PLUGIN_AUTHOR		"Asherkin"
#define PLUGIN_VERSION		"1.0.0"
#define PLUGIN_CONTACT		"http://limetech.org/"

public Plugin:myinfo = {
	name			= PLUGIN_NAME,
	author			= PLUGIN_AUTHOR,
	description	= PLUGIN_NAME,
	version		= PLUGIN_VERSION,
	url				= PLUGIN_CONTACT
};

public OnPluginStart() {
	CreateConVar("dodgeball_version", PLUGIN_VERSION, PLUGIN_NAME, FCVAR_SPONLY|FCVAR_REPLICATED|FCVAR_NOTIFY);

	LoadTranslations("common.phrases");
	HookEvent("post_inventory_application", Event_PostInventoryApplication);
	HookEvent("player_spawn", Event_PlayerSpawn);
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

public Event_PostInventoryApplication(Handle:event, const String:name[], bool:dontBroadcast) {
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
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
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	new TFClassType:class = TF2_GetPlayerClass(client);
	
	if (!(class == TFClassType:TFClass_Pyro || class == TFClassType:TFClass_Unknown)) {
		TF2_SetPlayerClass(client, TFClassType:TFClass_Pyro, false, true);
		TF2_RespawnPlayer(client);
	}
}

public Action:TF2Items_OnGiveNamedItem(client, String:strClassName[], iItemDefinitionIndex, &Handle:hItemOverride) {
	if (hItemOverride != INVALID_HANDLE || !(iItemDefinitionIndex == 21 || iItemDefinitionIndex == 40))
		return Plugin_Continue;
	
	new Handle:hWeapon = TF2Items_CreateItem(OVERRIDE_ATTRIBUTES);

	TF2Items_SetNumAttributes(hWeapon, 7);
	
	TF2Items_SetAttribute(hWeapon, 0, 112, 0.25);
	TF2Items_SetAttribute(hWeapon, 1, 76, 4.0);
	TF2Items_SetAttribute(hWeapon, 2, 1, 0.0);
	TF2Items_SetAttribute(hWeapon, 3, 60, 0.0);
	TF2Items_SetAttribute(hWeapon, 4, 66, 0.0);
	TF2Items_SetAttribute(hWeapon, 5, 72, 0.0);
	TF2Items_SetAttribute(hWeapon, 6, 74, 0.0);
	//TF2Items_SetAttribute(hWeapon, 7, 134, 3.0);
	
	hItemOverride = hWeapon;
	return Plugin_Changed;
}