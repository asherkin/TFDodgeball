#include "extension.h"
#include "CTFDB_MapConfig.h"

LINK_ENTITY_TO_CUSTOM_CLASS(tfdb_mapconfig, info_target, CTFDB_MapConfig);
//LINK_ENTITY_TO_CLASS(info_target, CTFDB_MapConfig);

BEGIN_DATADESC_NO_BASE(CTFDB_MapConfig)
	// Custom Stuff
	DEFINE_KEYFIELD(m_iMaxRockets, FIELD_INTEGER, "maxrockets"),
	DEFINE_KEYFIELD(m_fSpawnInterval, FIELD_FLOAT, "spawninterval"),
	DEFINE_KEYFIELD(m_fSpeedMul, FIELD_FLOAT, "speedmul"),
	DEFINE_KEYFIELD(m_fReflectInc, FIELD_FLOAT, "reflectinc"),
	DEFINE_KEYFIELD(m_fTurnRate, FIELD_FLOAT, "turnrate"),

	DEFINE_INPUT(m_iMaxRockets, FIELD_INTEGER, "SetMaxRockets"),
	DEFINE_INPUT(m_fSpawnInterval, FIELD_FLOAT, "SetSpawnInterval"),
	DEFINE_INPUT(m_fSpeedMul, FIELD_FLOAT, "SetSpeedMul"),
	DEFINE_INPUT(m_fReflectInc, FIELD_FLOAT, "SetReflectInc"),
	DEFINE_INPUT(m_fTurnRate, FIELD_FLOAT, "SetTurnRate"),

	DEFINE_INPUTFUNC(FIELD_VOID, "DumpData", DumpDataFunc),

	// Base Stuff
	DEFINE_KEYFIELD( m_iClassname, FIELD_STRING, "classname" ),
	DEFINE_GLOBAL_KEYFIELD( m_iGlobalname, FIELD_STRING, "globalname" ),
	DEFINE_KEYFIELD( m_iHammerID, FIELD_INTEGER, "hammerid" ), // save ID numbers so that entities can be tracked between save/restore and vmf
	DEFINE_KEYFIELD( m_nNextThinkTick, FIELD_TICK, "nextthink" ),
	DEFINE_FIELD( m_iEFlags, FIELD_INTEGER ),
	DEFINE_FIELD( m_iName, FIELD_STRING ),
	DEFINE_FIELD( m_hOwnerEntity, FIELD_EHANDLE ),

	DEFINE_FIELD( m_vecAbsOrigin, FIELD_POSITION_VECTOR ),
	DEFINE_KEYFIELD( m_spawnflags, FIELD_INTEGER, "spawnflags" ),
	DEFINE_FIELD( m_angAbsRotation, FIELD_VECTOR ),
	DEFINE_FIELD( m_vecOrigin, FIELD_VECTOR ),			// NOTE: MUST BE IN LOCAL SPACE, NOT POSITION_VECTOR!!! (see CBaseEntity::Restore)
	DEFINE_FIELD( m_angRotation, FIELD_VECTOR ),

	DEFINE_FIELD( m_fFlags, FIELD_INTEGER ),
END_DATADESC()

void CTFDB_MapConfig::Spawn(void)
{
	m_iMaxRockets = 0;
	m_fSpawnInterval = 0.0;
	m_fSpeedMul = 0.0;
	m_fReflectInc = 0.0;
	m_fTurnRate = 0.0;

	BaseClass::Spawn();

	inputdata_t p;
	DumpDataFunc(p);
}

void CTFDB_MapConfig::DumpDataFunc(inputdata_t &inputData)
{
	g_pSM->LogMessage(myself, "Datatable Dump:");
	g_pSM->LogMessage(myself, ">>> m_iMaxRockets = %d", m_iMaxRockets);
	g_pSM->LogMessage(myself, ">>> m_fSpawnInterval = %f", m_fSpawnInterval);
	g_pSM->LogMessage(myself, ">>> m_fSpeedMul = %f", m_fSpeedMul);
	g_pSM->LogMessage(myself, ">>> m_fReflectInc = %f", m_fReflectInc);
	g_pSM->LogMessage(myself, ">>> m_fTurnRate = %f", m_fTurnRate);
	g_pSM->LogMessage(myself, ">>> m_iClassname = %s", m_iClassname);
	g_pSM->LogMessage(myself, ">>> m_iGlobalname = %s", m_iGlobalname);
	g_pSM->LogMessage(myself, ">>> m_nNextThinkTick = %d", m_nNextThinkTick);
	g_pSM->LogMessage(myself, ">>> m_iEFlags = %d", m_iEFlags);
	g_pSM->LogMessage(myself, ">>> m_iName = %s", m_iName);
	g_pSM->LogMessage(myself, ">>> m_spawnflags = %d", m_spawnflags);
	g_pSM->LogMessage(myself, ">>> m_fFlags = %u", m_fFlags);
}