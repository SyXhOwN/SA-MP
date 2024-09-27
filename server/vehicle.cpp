
#include "main.h"

CVehicle::CVehicle( int iModel, VECTOR *vecPos,
				    float fRotation, int iColor1,
				    int iColor2, int iRespawnDelay)
{
	// Store the spawn info.
	m_SpawnInfo.iVehicleType = iModel;
	m_SpawnInfo.fRotation = fRotation;
	m_SpawnInfo.vecPos.X = vecPos->X;
	m_SpawnInfo.vecPos.Y = vecPos->Y;
	m_SpawnInfo.vecPos.Z = vecPos->Z;
	m_SpawnInfo.iColor1 = iColor1;
	m_SpawnInfo.iColor2 = iColor2;
	m_SpawnInfo.iRespawnDelay = iRespawnDelay;
	m_SpawnInfo.iInterior = 0;

	field_0.X = vecPos->X;
	field_0.Y = vecPos->Y;
	field_0.Z = vecPos->Z;

	field_100 = 0;
	field_105 = pNetGame->GetTime();

	// Set the initial pos to spawn pos.
	memset(&m_matWorld,0,sizeof(MATRIX4X4));
	m_matWorld.pos.X = m_SpawnInfo.vecPos.X;
	m_matWorld.pos.Y = m_SpawnInfo.vecPos.Y;
	m_matWorld.pos.Z = m_SpawnInfo.vecPos.Z;

	memset(field_4C, 0, sizeof(field_4C));
	memset(field_58, 0, sizeof(field_58));
	memset(&m_CarModInfo,  0,sizeof(CAR_MOD_INFO));
	m_CarModInfo.iColor0 = -1;
	m_CarModInfo.iColor1 = -1;
	memset(field_CE, 0, sizeof(field_CE));

	field_7A = 1; // m_bIsActive ?
	field_7E = 0; // m_bIsWasted ?
	field_6A = -1; // m_byteDriverID ?
	m_fHealth = 1000.0f;

	field_AA = 0;
	field_F1 = -1;
	field_F3 = -1;
	field_F4 = -1;
	field_F2 = -1;
	field_EF = -1;
	field_F0 = -1;
	field_F5 = -1;
	field_F6 = -1;
	field_F7 = -1;
	field_F8 = -1;
	field_F9 = -1;
	field_FA = -1;
	field_FB = -1;
	field_FC = -1;
	field_FD = -1;
	field_FE = -1;
	field_AE = 0;
	field_B2 = 0;
	field_B3 = 0;
	field_FF = 0;
	field_66 = 0;
	field_B4 = 0;
	field_109 = 0;
	field_10A = 0;
	field_B5 = -1;
}
