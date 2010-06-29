#ifndef _INCLUDE_CTFDB_MAPCONFIG_H_
#define _INCLUDE_CTFDB_MAPCONFIG_H_

#include "CEntityManager.h"
#include "CEntity.h"

class CTFDB_MapConfig : public CEntity
{
public:
	DECLARE_CLASS(CTFDB_MapConfig, CEntity);
	DECLARE_DATADESC();

	void Spawn(void);
	void DumpDataFunc(inputdata_t &inputData);

//	void InputSetMaxRockets( inputdata_t &data );
//	void InputSetSpawnInterval( inputdata_t &data );
//	void InputSetSpeedMul( inputdata_t &data );
//	void InputSetReflectInc( inputdata_t &data );
//	void InputSetTurnRate( inputdata_t &data );

protected: // Datamaps
	DECLARE_DATAMAP(string_t, m_iGlobalname);
	DECLARE_DATAMAP(int, m_iHammerID);
	DECLARE_DATAMAP(string_t, m_iName);
	DECLARE_DATAMAP(Vector, m_vecAbsOrigin);
	DECLARE_DATAMAP(int, m_spawnflags);
	DECLARE_DATAMAP(QAngle, m_angAbsRotation);
	DECLARE_DATAMAP(QAngle, m_angRotation);

protected: // My Datamaps
	int m_iMaxRockets;
	float m_fSpawnInterval;
	float m_fSpeedMul;
	float m_fReflectInc;
	float m_fTurnRate;

};

#endif // _INCLUDE_CTFDB_MAPCONFIG_H_