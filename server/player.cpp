
#include "main.h"

extern CNetGame *pNetGame;

//----------------------------------------------------

CPlayer::CPlayer()
{
	m_PlayerID = INVALID_PLAYER_ID;
	field_2963 = 0;
	field_290D = 0;
	field_295F = 0;
	field_2CDA = 0;
	field_2CD1 = 0;
	memset(&m_aimSync,0,sizeof(AIM_SYNC_DATA));
	memset(&m_icSync,0,sizeof(INCAR_SYNC_DATA));
	memset(&m_psSync,0,sizeof(PASSENGER_SYNC_DATA));
	memset(&m_ofSync,0,sizeof(ONFOOT_SYNC_DATA));
	memset(&m_spSync,0,sizeof(SPECTATOR_SYNC_DATA));
	memset(&m_trSync,0,sizeof(TRAILER_SYNC_DATA));
	field_2C85 = 0;
	ResetAllSyncAttributes();
}

//----------------------------------------------------

void CPlayer::ResetAllSyncAttributes()
{
	field_2969 = 0;
	m_VehicleID = 0;
	field_2B9D = 0;
	field_2BA1 = 0;
	field_2951 = 0;
	field_2915 = 0;
	field_2919 = 0;
	field_291D = 0;
	field_292D = 0;
	field_2931 = 0;
	field_2935 = 0;
	field_2929 = 0;
	field_2939 = 0;
	field_2921 = 0;
	field_2925 = 0;
	field_2CB8 = 0.0f;
	m_bCheckpointEnabled = FALSE;
	m_bRaceCheckpointEnabled = FALSE;
	m_iInteriorId = 0;
	m_dwColor = 0;
	field_2949 = 0;
	field_294B = 0;
	field_294D = 0;
	m_byteState = PLAYER_STATE_NONE;
	field_2CB7 = 0;
	field_2C2F = 0;
	field_2C30 = 4;
	field_2CBC = 0;
	field_2CBD = -1;
	m_byteSeatID = 0;
	field_28F5 = 0;
	field_28F9 = 0;
	field_2BDF = 0;
	field_2CC1 = pNetGame->GetTime();

	for (int i = 0; i < 13; i++)
	{
		m_byteSlotWeapon[i] = 0;
		m_dwSlotAmmo[i] = 0;
	}
}

//----------------------------------------------------

void CPlayer::SetSpawnInfo(PLAYER_SPAWN_INFO *pSpawn)
{
	memcpy(&m_SpawnInfo,pSpawn,sizeof(PLAYER_SPAWN_INFO));
	m_bHasSpawnInfo = TRUE;
}

//----------------------------------------------------




//----------------------------------------------------

void CPlayer::SetPlayerColor(DWORD dwColor)
{
	RakNet::BitStream bsColor;

	m_dwColor = dwColor;

	bsColor.Write(m_PlayerID);
	bsColor.Write(dwColor);

	pNetGame->BroadcastData(RPC_ScrSetPlayerColor, &bsColor, INVALID_PLAYER_ID, 2);
}

//----------------------------------------------------

void CPlayer::SetCheckpoint(float fX, float fY, float fZ, float fSize)
{
	m_vecCheckpoint.X = fX;
	m_vecCheckpoint.Y = fY;
	m_vecCheckpoint.Z = fZ;
	m_fCheckpointSize = fSize;
	ToggleCheckpoint(TRUE);
}

//----------------------------------------------------

void CPlayer::ToggleCheckpoint(BOOL bEnabled)
{
	m_bCheckpointEnabled = bEnabled;
	m_bInCheckpoint = FALSE;

	RakNet::BitStream bsParams;

	if (bEnabled)
	{
		bsParams.Write(m_vecCheckpoint.X);
		bsParams.Write(m_vecCheckpoint.Y);
		bsParams.Write(m_vecCheckpoint.Z);
		bsParams.Write(m_fCheckpointSize);
		pNetGame->SendToPlayer(RPC_SetCheckpoint, &bsParams, m_PlayerID, 2);
	} else {
		pNetGame->SendToPlayer(RPC_DisableCheckpoint, &bsParams, m_PlayerID, 2);
	}
}

//----------------------------------------------------

// TODO: sub_486D30
// TODO: sub_486DE0

//----------------------------------------------------

void CPlayer::SetRaceCheckpoint(int iType, float fX, float fY, float fZ, float fNX, float fNY, float fNZ, float fSize)
{
	m_vecRaceCheckpoint.X = fX;
	m_vecRaceCheckpoint.Y = fY;
	m_vecRaceCheckpoint.Z = fZ;
	m_vecRaceNextCheckpoint.X = fNX;
	m_vecRaceNextCheckpoint.Y = fNY;
	m_vecRaceNextCheckpoint.Z = fNZ;
	m_fRaceCheckpointSize = fSize;
	m_byteRaceCheckpointType = iType;
	ToggleRaceCheckpoint(TRUE);
}

//----------------------------------------------------

void CPlayer::ToggleRaceCheckpoint(BOOL bEnabled)
{
	m_bRaceCheckpointEnabled = bEnabled;
	m_bInRaceCheckpoint = FALSE;

	RakNet::BitStream bsParams;

	if (bEnabled)
	{
		bsParams.Write(m_byteRaceCheckpointType);
		bsParams.Write(m_vecRaceCheckpoint.X);
		bsParams.Write(m_vecRaceCheckpoint.Y);
		bsParams.Write(m_vecRaceCheckpoint.Z);
		bsParams.Write(m_vecRaceNextCheckpoint.X);
		bsParams.Write(m_vecRaceNextCheckpoint.Y);
		bsParams.Write(m_vecRaceNextCheckpoint.Z);
		bsParams.Write(m_fRaceCheckpointSize);
		pNetGame->SendToPlayer(RPC_SetRaceCheckpoint, &bsParams, m_PlayerID, 2);
	} else {
		pNetGame->SendToPlayer(RPC_DisableRaceCheckpoint, &bsParams, m_PlayerID, 2);
	}
}

//----------------------------------------------------
