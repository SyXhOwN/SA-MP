
#include "../main.h"
#include "../game/util.h"

extern CGame		 *pGame;
extern CNetGame		 *pNetGame;

BOOL bFirstSpawn = TRUE;

int iNetModeNormalOnfootSendRate	= NETMODE_NORMAL_ONFOOT_SENDRATE;
int iNetModeNormalIncarSendRate		= NETMODE_NORMAL_INCAR_SENDRATE;
int iNetModeFiringSendRate			= NETMODE_FIRING_SENDRATE;
int iNetModeSendMultiplier			= NETMODE_SEND_MULTIPLIER;

//----------------------------------------------------------

CLocalPlayer::CLocalPlayer()
{
	field_147 = GetTickCount();
	field_137 = GetTickCount();
	field_13B = GetTickCount();
	m_bInRCMode = FALSE;
	field_2E2 = 0;
	field_2D6 = GetTickCount();
	field_2F6 = 0;
	m_pPlayerPed = pGame->FindPlayerPed();
	m_bIsActive = FALSE;
	m_bIsWasted = FALSE;
	field_10D = 0;
	field_30A = 0;
	field_10F = GetTickCount();
	field_113 = GetTickCount();
	field_13F = GetTickCount();
	field_117 = field_10F;
	field_11B = field_10F;
	m_bIsSpectating = FALSE;
	field_30F = 0;
	m_SpectateID = 0xFFFFFFFF;
	field_306 = 0;

	ResetAllSyncAttributes();

	DWORD x=0;
	while(x!=13) {
		field_18A[x] = 0;
		field_197[x] = 0;
		x++;
	}

	m_byteTeam = NO_TEAM;
}

//----------------------------------------------------------

void CLocalPlayer::ResetAllSyncAttributes()
{
	m_bHasSpawnInfo = FALSE;
	field_2FA = 0;
	field_302 = 0;
	field_2FE = 0;
	field_1CF = 0;
	field_FA = -1;
	m_bInRCMode = FALSE;
	field_318 = -1;
	field_11F = -1;
	field_121 = -1;
	field_123 = -1;
	memset(field_12B, 0, sizeof(field_12B));
	memset(field_3F, 0, sizeof(field_3F));
	memset(field_5E, 0, sizeof(field_5E));
	field_92 = 0;
	memset(field_94, 0, sizeof(field_94));
	memset(field_0, 0, sizeof(field_0));
	memset(field_3C, 0, sizeof(field_3C));
	memset(field_D8, 0, sizeof(field_D8));
	field_FC = 0;
	field_100 = 0;
	field_185 = -1;
	field_187 = -1;
	field_181 = GetTickCount();
	field_189 = 0;
	field_2DA = 0;
	field_2DE = 0;
	field_127 = GetTickCount();
}

//----------------------------------------------------------

void CLocalPlayer::Say(PCHAR szText)
{
	// TODO: CLocalPlayer::Say .text:10005A10
}

//----------------------------------------------------------



//----------------------------------------------------------

BOOL CLocalPlayer::Spawn()
{
	if(!m_bHasSpawnInfo) return FALSE;

	CCamera	*pGameCamera;
	pGameCamera = pGame->GetCamera();
	pGameCamera->Restore();
	pGameCamera->SetBehindPlayer();
	pGame->DisplayHud(TRUE);
	m_pPlayerPed->TogglePlayerControllable(1);

	if(!bFirstSpawn) {
		m_pPlayerPed->SetInitialState();
	} else {
		bFirstSpawn = FALSE;
	}

	// TODO: CLocalPlayer::Spawn

	return TRUE;
}

//----------------------------------------------------------



//----------------------------------------------------

void CLocalPlayer::SetPlayerColor(DWORD dwColor)
{
	SetRadarColor(pNetGame->GetPlayerPool()->GetLocalPlayerID(),dwColor);
}

//----------------------------------------------------

DWORD CLocalPlayer::GetPlayerColorAsRGBA()
{
	return TranslateColorCodeToRGBA(pNetGame->GetPlayerPool()->GetLocalPlayerID());
}

//----------------------------------------------------

DWORD CLocalPlayer::GetPlayerColorAsARGB()
{
	return (TranslateColorCodeToRGBA(pNetGame->GetPlayerPool()->GetLocalPlayerID()) >> 8) | 0xFF000000;
}

//----------------------------------------------------

void CLocalPlayer::ProcessOnFootWorldBounds()
{
	if(pGame->GetActiveInterior() != 0) return; // can't enforce inside interior

	if( m_pPlayerPed->EnforceWorldBoundries(
		pNetGame->field_3D5->WorldBounds[0],
		pNetGame->field_3D5->WorldBounds[1],
		pNetGame->field_3D5->WorldBounds[2],
		pNetGame->field_3D5->WorldBounds[3]) )
	{
		m_pPlayerPed->SetArmedWeapon(0);
		pGame->DisplayGameText("Stay within the ~r~world boundries",1000,5);
	}
}

//----------------------------------------------------

void CLocalPlayer::ProcessInCarWorldBounds()
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	VEHICLEID VehicleID = (VEHICLEID)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
	CVehicle *pGameVehicle;

	if(pGame->GetActiveInterior() != 0) return; // can't enforce inside interior

	if(VehicleID != INVALID_VEHICLE_ID) {
		pGameVehicle = pVehiclePool->GetAt(VehicleID);
		if(!pGameVehicle) return;

		if( pGameVehicle->EnforceWorldBoundries(
			pNetGame->field_3D5->WorldBounds[0],
			pNetGame->field_3D5->WorldBounds[1],
			pNetGame->field_3D5->WorldBounds[2],
			pNetGame->field_3D5->WorldBounds[3]) )
		{
			pGame->DisplayGameText("Stay within the ~r~world boundries",1000,5);
		}
	}
}

//-----------------------------------------------------








//-----------------------------------------------------------

void CLocalPlayer::ToggleSpectating(BOOL bToggle)
{
	// TODO: CLocalPlayer::ToggleSpectating

	m_bIsSpectating = bToggle;
	m_SpectateID = 0xFFFFFFFF;
}

//-----------------------------------------------------------

