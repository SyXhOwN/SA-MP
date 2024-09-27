
#include "../main.h"
#include "util.h"

extern CGame		*pGame;
extern CNetGame		*pNetGame;
extern WORD			wVehicleComponentDebug;

BOOL bDetachingVehicleTrailer;

//-----------------------------------------------------------
// CONSTRUCTOR

CVehicle::CVehicle(int iType, float fPosX, float fPosY,
				   float fPosZ, float fRotation, BOOL bKeepModelLoaded,
				   int a8)
{
	// TODO: CVehicle::CVehicle
}

//-----------------------------------------------------------
// DESTRUCTOR

CVehicle::~CVehicle()
{
	m_pVehicle = GamePool_Vehicle_GetAt(m_dwGTAId);

	if(m_pVehicle) {
		if(m_dwMarkerID) {
			ScriptCommand(&disable_marker, m_dwMarkerID);
			m_dwMarkerID = 0;
		}

		RemoveEveryoneFromVehicle();

		if(m_pTrailer) {
			DetachTrailer();
			m_pTrailer = NULL;
		}

		FUNC_100B81F0();

		if( m_pVehicle->entity.nModelIndex == TRAIN_PASSENGER_LOCO ||
			m_pVehicle->entity.nModelIndex == TRAIN_FREIGHT_LOCO )
		{
			ScriptCommand(&destroy_train,m_dwGTAId);
		} else {
			// TODO: CVehicle::~CVehicle()

			//int nModelIndex = m_pVehicle->entity.nModelIndex;
			ScriptCommand(&destroy_car,m_dwGTAId);
			//pChatWindow->AddDebugMessage("VehicleModelRefs: %d",GetModelReferenceCount(nModelIndex));
			//if(!GetModelReferenceCount(nModelIndex) && !m_bKeepModelLoaded) {
			//	// No references and we've not been instructed to keep this
			//	// model loaded, so it should be safe to remove it.
			//	pGame->RemoveModel(nModelIndex);
			//}
		}
	}
}

//-----------------------------------------------------------
// Add override

void CVehicle::Add()
{
	if (!IsAdded()) {
		// Call underlying Add
		CEntity::Add();

		// Process stuff for trailers
		CVehicle *pTrailer = this->GetTrailer();
		if(pTrailer) pTrailer->Add();
	}
}

//-----------------------------------------------------------
// Remove override

void CVehicle::Remove()
{
	if (IsAdded()) {
		// Process stuff for trailers
		CVehicle *pTrailer = this->GetTrailer();
		if(pTrailer) pTrailer->Remove();

		// Call underlying Remove
		CEntity::Remove();
	}
}

//-----------------------------------------------------------

void CVehicle::LinkToInterior(int iInterior)
{
	if(GamePool_Vehicle_GetAt(m_dwGTAId)) {
		ScriptCommand(&link_vehicle_to_interior, m_dwGTAId, iInterior);
	}
}

//-----------------------------------------------------------
// If the game has internally destroyed the vehicle
// during this frame, the vehicle pointer should become 0

void CVehicle::ResetPointers()
{
	if(!m_pVehicle) return;

	m_pVehicle = GamePool_Vehicle_GetAt(m_dwGTAId);
	m_pEntity = (ENTITY_TYPE *)m_pVehicle;
}

//-----------------------------------------------------------

BOOL CVehicle::HasADriver()
{
	if(!m_pVehicle) return FALSE;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return FALSE;

	if(m_pVehicle) {
		if(m_pVehicle->pDriver && IN_VEHICLE(m_pVehicle->pDriver) && m_pVehicle->pDriver->dwPedType == 0)
			return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CVehicle::IsOccupied()
{
	if(m_pVehicle) {
		if(m_pVehicle->pDriver) return TRUE;
		if(m_pVehicle->pPassengers[0]) return TRUE;
		if(m_pVehicle->pPassengers[1]) return TRUE;
		if(m_pVehicle->pPassengers[2]) return TRUE;
		if(m_pVehicle->pPassengers[3]) return TRUE;
		if(m_pVehicle->pPassengers[4]) return TRUE;
		if(m_pVehicle->pPassengers[5]) return TRUE;
		if(m_pVehicle->pPassengers[6]) return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

void CVehicle::SetInvulnerable(BOOL bInv)
{
	if(!m_pVehicle) return;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return;
	if(m_pVehicle->entity.vtable == 0x863C40) return;

	if(bInv) {
		ScriptCommand(&set_car_immunities,m_dwGTAId,1,1,1,1,1);
		ScriptCommand(&toggle_car_tires_vulnerable,m_dwGTAId,0);
		m_bIsInvulnerable = TRUE;
	} else { 
		ScriptCommand(&set_car_immunities,m_dwGTAId,0,0,0,0,0);
		ScriptCommand(&toggle_car_tires_vulnerable,m_dwGTAId,1);

		m_bIsInvulnerable = FALSE;
	}
}
//-----------------------------------------------------------

void CVehicle::SetLockedState(int iLocked)
{
	if(!m_pVehicle) return;

	if(iLocked) {
		ScriptCommand(&lock_car,m_dwGTAId,1);
	} else {
		ScriptCommand(&lock_car,m_dwGTAId,0);
	}
}

//-----------------------------------------------------------

void CVehicle::SetEngineState(BOOL bState)
{
	if(!m_pVehicle) return;

	if(!bState) {
		m_pVehicle->byteFlags &= 0xDF;
	} else {
		m_pVehicle->byteFlags |= 0x20;
	}
}

//-----------------------------------------------------------

float CVehicle::GetHealth()
{
	if(m_pVehicle) return m_pVehicle->fHealth;
	else return 0.0f;
}

//-----------------------------------------------------------

void CVehicle::SetHealth(float fHealth)
{
	if(m_pVehicle) {
		m_pVehicle->fHealth = fHealth;
	}
}

//-----------------------------------------------------------

void CVehicle::SetColor(BYTE byteColor1, BYTE byteColor2)
{
	if(m_pVehicle && GamePool_Vehicle_GetAt(m_dwGTAId))  {
		m_pVehicle->byteColor1 = byteColor1;
		m_pVehicle->byteColor2 = byteColor2;
	}
	m_byteColor2 = byteColor2;
	m_byteColor1 = byteColor1;
	m_bHasNewColor = TRUE;
}

//-----------------------------------------------------------

void CVehicle::UpdateColor()
{
	if(!m_pVehicle) return;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return;

	if(m_bHasNewColor) {
		if(!field_71) {
			if(m_byteColor1 != m_pVehicle->byteColor1 || m_byteColor2 != m_pVehicle->byteColor2) {
				m_pVehicle->byteColor1 = m_byteColor1;
				m_pVehicle->byteColor2 = m_byteColor2;
			}
		}
	}
}

//-----------------------------------------------------------

UINT CVehicle::GetVehicleSubtype()
{
	if(m_pVehicle) {
		return GetVehicleSubtypeFromVehiclePtr(m_pVehicle);
	}
	return 0;
}

//-----------------------------------------------------------

BOOL CVehicle::HasSunk()
{
	if(!m_pVehicle) return FALSE;

	return ScriptCommand(&has_car_sunk,m_dwGTAId);
}

//-----------------------------------------------------------

BOOL CVehicle::IsWrecked()
{
	if(!m_pVehicle) return FALSE;

	return ScriptCommand(&is_car_wrecked,m_dwGTAId);
}

//-----------------------------------------------------------

BOOL CVehicle::IsDriverLocalPlayer()
{
	if(m_pVehicle) {
		if((PED_TYPE *)m_pVehicle->pDriver == GamePool_FindPlayerPed()) {
			return TRUE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CVehicle::IsLocalPlayerInThisVehicle()
{
	if(m_pVehicle) {
		PED_TYPE *pPed = GamePool_FindPlayerPed();
		if(pPed && IN_VEHICLE(pPed) && (DWORD)m_pVehicle == pPed->pVehicle) {
			return TRUE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CVehicle::IsATrainPart()
{
	int nModel;
	if(m_pVehicle) {
		nModel = m_pVehicle->entity.nModelIndex;
		if(nModel == TRAIN_PASSENGER_LOCO) return TRUE;
		if(nModel == TRAIN_PASSENGER) return TRUE;
		if(nModel == TRAIN_FREIGHT_LOCO) return TRUE;
		if(nModel == TRAIN_FREIGHT) return TRUE;
		if(nModel == TRAIN_TRAM) return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CVehicle::HasTurret()
{
	int nModel = GetModelIndex();
	return (nModel == 432 ||	// Tank
			nModel == 564 ||	// RC Tank
			nModel == 407 ||	// Firetruck
			nModel == 601		// Swatvan
			);
}

//-----------------------------------------------------------
// This can currently only be used for setting the alternate
// siren. The way it's coded internally doesn't seem to allow
// us to modify the horn alone.

void CVehicle::SetHornState(BYTE byteState)
{
	if(!m_pVehicle) return;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return;

	if( (GetVehicleSubtype() != VEHICLE_SUBTYPE_BOAT ) &&
		(GetVehicleSubtype() != VEHICLE_SUBTYPE_PLANE) &&
		(GetVehicleSubtype() != VEHICLE_SUBTYPE_HELI ) )
	{
		m_pVehicle->byteHorn = byteState;
		m_pVehicle->byteHorn2 = byteState;
	}
}

//-----------------------------------------------------------

void CVehicle::SetSirenOn(BYTE byteState)
{
	m_pVehicle->bSirenOn = byteState;
}

//-----------------------------------------------------------

BOOL CVehicle::IsSirenOn()
{
	return (m_pVehicle->bSirenOn == 1);
}

//-----------------------------------------------------------

void CVehicle::SetAlarmState(WORD wState)
{
	m_pVehicle->wAlarmState = wState;
}

//-----------------------------------------------------------

void CVehicle::SetLandingGearState(eLandingGearState state)
{
	//if(!m_pVehicle) return;
	//if(GetVehicleSubtypeFromVehiclePtr(m_pVehicle) != VEHICLE_SUBTYPE_PLANE) return;
	if(GetVehicleSubtype() != VEHICLE_SUBTYPE_PLANE) return;

	DWORD dwVehiclePtr = (DWORD)m_pVehicle;
	float fPlaneLandingGear = 0.0f;

	_asm mov eax, dwVehiclePtr
	_asm mov edx, [eax+0x9CC]
	_asm mov fPlaneLandingGear, edx

	if (state == LGS_DOWN && fPlaneLandingGear == 0.0f)
	{
		_asm mov ecx, dwVehiclePtr
		_asm mov edx, 0x6CAC20
		_asm call edx
	}
	else if(state == LGS_UP && fPlaneLandingGear == 1.0f)
	{
		_asm mov ecx, dwVehiclePtr
		_asm mov edx, 0x6CAC70
		_asm call edx		
	}
}

//-----------------------------------------------------------

eLandingGearState CVehicle::GetLandingGearState()
{
	if(GetVehicleSubtype() != VEHICLE_SUBTYPE_PLANE) return LGS_UP;

	DWORD dwVehiclePtr = (DWORD)m_pVehicle;
	float fPlaneLandingGear = 0.0f;

	_asm mov eax, dwVehiclePtr
	_asm mov edx, [eax+0x9CC]
	_asm mov fPlaneLandingGear, edx

	if(fPlaneLandingGear == 0.0f) return LGS_UP;
	return LGS_DOWN;
}

//-----------------------------------------------------------

UINT CVehicle::GetPassengersMax()
{
	return 0;
}

//-----------------------------------------------------------

void CVehicle::SetHydraThrusters(DWORD dwDirection)
{
	if(m_pVehicle) m_pVehicle->dwHydraThrusters = dwDirection; // 0x00 - 0x80 // byte
}

//-----------------------------------------------------------

DWORD CVehicle::GetHydraThrusters()
{
	if(m_pVehicle) return m_pVehicle->dwHydraThrusters;
	return 0UL;
}

//-----------------------------------------------------------

void CVehicle::ProcessMarkers()
{
	if(!m_pVehicle) return;

	if(m_byteObjectiveVehicle) {
		// SHOW ALWAYS
		if(!m_bSpecialMarkerEnabled) {
			if(m_dwMarkerID) {
				ScriptCommand(&disable_marker, m_dwMarkerID);
				m_dwMarkerID = 0;
			}
			ScriptCommand(&tie_marker_to_car, m_dwGTAId, 1, 3, &m_dwMarkerID);
			ScriptCommand(&set_marker_color,m_dwMarkerID,1006);
			ScriptCommand(&show_on_radar,m_dwMarkerID,3);
			m_bSpecialMarkerEnabled = TRUE;
		}
		return;
	}

	// Disable the special marker if it has been deactivated
	if(!m_byteObjectiveVehicle && m_bSpecialMarkerEnabled) {
		if(m_dwMarkerID) {
			ScriptCommand(&disable_marker, m_dwMarkerID);
			m_bSpecialMarkerEnabled = FALSE;
			m_dwMarkerID = 0;
		}
	}

	// Add or remove car scanning markers.
	if(GetDistanceFromLocalPlayerPed() < CSCANNER_DISTANCE && !IsOccupied()) {
		// SHOW IT
		if(!m_dwMarkerID)  {
			ScriptCommand(&tie_marker_to_car, m_dwGTAId, 1, 2, &m_dwMarkerID);
			ScriptCommand(&set_marker_color,m_dwMarkerID,1004);
		}	
	} 
	else if(IsOccupied() || GetDistanceFromLocalPlayerPed() >= CSCANNER_DISTANCE) {
		// REMOVE IT	
		if(m_dwMarkerID) {
			ScriptCommand(&disable_marker, m_dwMarkerID);
			m_dwMarkerID = 0;
		}
	}
}

//-----------------------------------------------------------

void CVehicle::SetDoorState(int iState)
{
	if(iState) {
		m_pVehicle->dwDoorsLocked = 2;
		m_bDoorsLocked = TRUE;
	} else {
		m_pVehicle->dwDoorsLocked = 0;
		m_bDoorsLocked = FALSE;
	}
}

//-----------------------------------------------------------

BOOL CVehicle::UpdateLastDrivenTime()
{
	if(m_pVehicle) {
		if(m_pVehicle->pDriver) {
			m_bHasBeenDriven = TRUE;
			m_dwTimeSinceLastDriven = GetTickCount();
			return TRUE;
		}
	}
	return FALSE;
	// Tell the system this vehicle has been used so it can reset the timer to not be based on remaining delay
}

//-----------------------------------------------------------

void CVehicle::SetTankRot(float X, float Y)
{
	m_pVehicle->fTankRotX = X;
	m_pVehicle->fTankRotY = Y;
}

//-----------------------------------------------------------

float CVehicle::GetTankRotX()
{
	return m_pVehicle->fTankRotX;
}

//-----------------------------------------------------------

float CVehicle::GetTankRotY()
{
	return m_pVehicle->fTankRotY;
}

//-----------------------------------------------------------

float CVehicle::GetTrainSpeed()
{
	if(!m_pVehicle) return 0.0f;

	float fResult = m_pVehicle->fTrainSpeed;
	return fResult;
}

//-----------------------------------------------------------

void CVehicle::SetTrainSpeed(float fSpeed)
{
	if(fSpeed > 100.0f || fSpeed < -100.0f) return;
	if(!m_pVehicle) return;

	m_pVehicle->fTrainSpeed = fSpeed;
}

//-----------------------------------------------------------

void CVehicle::SetWheelPopped(BYTE byteFlags)
{
	if(GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR)
	{
		if(byteFlags & 1)
			m_pVehicle->bCarWheelPopped[3] = 1;
		else
			m_pVehicle->bCarWheelPopped[3] = 0;

		byteFlags >>= 1;

		if(byteFlags & 1)
			m_pVehicle->bCarWheelPopped[2] = 1;
		else
			m_pVehicle->bCarWheelPopped[2] = 0;

		byteFlags >>= 1;

		if(byteFlags & 1)
			m_pVehicle->bCarWheelPopped[1] = 1;
		else
			m_pVehicle->bCarWheelPopped[1] = 0;

		byteFlags >>= 1;

		if (byteFlags & 1)
			m_pVehicle->bCarWheelPopped[0] = 1;
		else
			m_pVehicle->bCarWheelPopped[0] = 0;
	}
	else if(GetVehicleSubtype() == VEHICLE_SUBTYPE_BIKE)
	{
		if(byteFlags & 1)
			m_pVehicle->bBikeWheelPopped[1] = 1;
		else
			m_pVehicle->bBikeWheelPopped[1] = 0;

		byteFlags >>= 1;

		if(byteFlags & 1)
			m_pVehicle->bBikeWheelPopped[0] = 1;
		else
			m_pVehicle->bBikeWheelPopped[0] = 0;
	}
}

//-----------------------------------------------------------

BYTE CVehicle::GetWheelPopped()
{
	BYTE byteRet = 0;

	if(!m_pVehicle) return 0;

	if(GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR)
	{
		if(m_pVehicle->bCarWheelPopped[0]) byteRet |= 1;
		byteRet <<= 1;

		if(m_pVehicle->bCarWheelPopped[1]) byteRet |= 1;
		byteRet <<= 1;

		if(m_pVehicle->bCarWheelPopped[2]) byteRet |= 1;
		byteRet <<= 1;

		if(m_pVehicle->bCarWheelPopped[3]) byteRet |= 1;
	}
	else if(GetVehicleSubtype() == VEHICLE_SUBTYPE_BIKE)
	{
		if(m_pVehicle->bBikeWheelPopped[0]) byteRet |= 1;
		byteRet <<= 1;

		if(m_pVehicle->bBikeWheelPopped[1]) byteRet |= 1;
	}
	return byteRet;
}

//-----------------------------------------------------------

void CVehicle::UpdateDamageStatus(DWORD dwPanelDamage, DWORD dwDoorDamage, BYTE byteLightDamage)
{
	if(m_pVehicle && (GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR ||
		GetVehicleSubtype() == VEHICLE_SUBTYPE_PLANE)) {

		if(!dwPanelDamage && !dwDoorDamage && !byteLightDamage) {
			if(m_pVehicle->dwPanelStatus || m_pVehicle->dwDoorStatus1 || m_pVehicle->dwLightStatus) {
				// The ingame car is damaged in some way although the update
				// says the car should be repaired. So repair it and exit.
				DWORD dwVehiclePtr = (DWORD)m_pVehicle;
				_asm mov ecx, dwVehiclePtr
				_asm mov edx, 0x6A3440 // CAutomobile::RepairDamageModel
				_asm call edx

				return;
			}
		}

		m_pVehicle->dwPanelStatus = dwPanelDamage;
		m_pVehicle->dwDoorStatus1 = dwDoorDamage;
		m_pVehicle->dwLightStatus = (DWORD)byteLightDamage;

		DWORD dwVehiclePtr = (DWORD)m_pVehicle;
		_asm mov ecx, dwVehiclePtr
		_asm mov edx, 0x6B3E90 // CAutomobile::UpdateDamageModel
		_asm call edx		
	}
}

//-----------------------------------------------------------

DWORD CVehicle::GetPanelDamageStatus()
{
	if(m_pVehicle && GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR) {
		return m_pVehicle->dwPanelStatus;
	}
	return 0;
}

//-----------------------------------------------------------

DWORD CVehicle::GetDoorDamageStatus()
{
	if(m_pVehicle && GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR) {
		return m_pVehicle->dwDoorStatus1;
	}
	return 0;
}

//-----------------------------------------------------------

BYTE CVehicle::GetLightDamageStatus()
{
	if(m_pVehicle && GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR) {
		return (BYTE)m_pVehicle->dwLightStatus;
	}
	return 0;
}

//-----------------------------------------------------------

void CVehicle::AttachTrailer()
{
	if (m_pTrailer)
		ScriptCommand(&put_trailer_on_cab, m_pTrailer->m_dwGTAId, m_dwGTAId);
}

//-----------------------------------------------------------

void CVehicle::DetachTrailer()
{
	bDetachingVehicleTrailer = TRUE;

	if (m_pTrailer && m_pTrailer->m_dwGTAId && GamePool_Vehicle_GetAt(m_pTrailer->m_dwGTAId)) {
		if(m_pTrailer->m_pVehicle) {
			ScriptCommand(&detach_trailer_from_cab, m_pTrailer->m_dwGTAId, m_dwGTAId);
		}
	}
	bDetachingVehicleTrailer = FALSE;
}

//-----------------------------------------------------------

void CVehicle::SetTrailer(CVehicle *pTrailer)
{
	m_pTrailer = pTrailer;
}

//-----------------------------------------------------------

CVehicle* CVehicle::GetTrailer()
{
	if (!m_pVehicle) return NULL;

	// Try to find associated trailer
	DWORD dwTrailerGTAPtr = m_pVehicle->dwTrailer;

	if(dwTrailerGTAPtr && pNetGame) {
		CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
		VEHICLEID TrailerID = (VEHICLEID)pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE*)dwTrailerGTAPtr);
		if(TrailerID != INVALID_VEHICLE_ID && pVehiclePool->GetSlotState(TrailerID)) {
			return pVehiclePool->GetAt(TrailerID);
		}
	}

	return NULL;
}

//-----------------------------------------------------------

CVehicle* CVehicle::GetTractor()
{
	if(!m_pVehicle) return NULL;

	int iSubType = GetVehicleSubtype();
	if( iSubType == VEHICLE_SUBTYPE_BIKE ||
		iSubType == VEHICLE_SUBTYPE_BOAT ||
		iSubType == VEHICLE_SUBTYPE_TRAIN ||
		iSubType == VEHICLE_SUBTYPE_PUSHBIKE ) {
		
		return NULL;
	}

	// Try to find associated trailer
	DWORD dwTractorGTAPtr = m_pVehicle->dwTractor;

	if(dwTractorGTAPtr && pNetGame) {
		CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
		VEHICLEID TractorID = (VEHICLEID)pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE*)dwTractorGTAPtr);
		if(TractorID != INVALID_VEHICLE_ID && pVehiclePool->GetSlotState(TractorID)) {
			return pVehiclePool->GetAt(TractorID);
		}
	}

	return NULL;
}

//-----------------------------------------------------------

BOOL CVehicle::IsATrailer()
{
	if(!m_pVehicle) return FALSE;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return FALSE;

	if( m_pVehicle->entity.nModelIndex == 435 ||
		m_pVehicle->entity.nModelIndex == 450 ||
		m_pVehicle->entity.nModelIndex == 584 ||
		m_pVehicle->entity.nModelIndex == 591 ||
		m_pVehicle->entity.nModelIndex == 606 ||
		m_pVehicle->entity.nModelIndex == 607 ||
		m_pVehicle->entity.nModelIndex == 608 ||
		m_pVehicle->entity.nModelIndex == 610 ||
		m_pVehicle->entity.nModelIndex == 611 )
	{
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------

BOOL CVehicle::IsTowTruck()
{
	if(!m_pVehicle) return FALSE;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return FALSE;

	if( m_pVehicle->entity.nModelIndex == 525 )
	{
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------
//441, 	rcbandit
//464, 	rcbaron
//465, 	rcraider
//594, 	rccam
//564, 	rctiger 
//501, 	rcgoblin

BOOL CVehicle::IsRCVehicle()
{
	if (!m_pVehicle) return FALSE;
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) return FALSE;

	if( m_pVehicle->entity.nModelIndex == 441 ||
		m_pVehicle->entity.nModelIndex == 464 ||
		m_pVehicle->entity.nModelIndex == 465 ||
		m_pVehicle->entity.nModelIndex == 594 ||
		m_pVehicle->entity.nModelIndex == 501 ||
		m_pVehicle->entity.nModelIndex == 564 )
	{
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------

void CVehicle::ToggleLights(BYTE bEnable)
{
	if (!m_pVehicle) return;
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) return;

	if (!bEnable) {
		m_pVehicle->byteMoreFlags |= 8;
		m_pVehicle->byteFlags &= 0xBF;
	} else {
		m_pVehicle->byteMoreFlags &= ~8;
		m_pVehicle->byteFlags |= 0x40;
	}
}

//-----------------------------------------------------------

bool CVehicle::AreLightsEnabled()
{
	if (!m_pVehicle) return true;
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) return true;

	if (m_pVehicle->byteMoreFlags & 8)
		return true;

	return false;
}

//-----------------------------------------------------------

void CVehicle::RemoveEveryoneFromVehicle()
{
	if (!m_pVehicle) return;
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) return;

	float fPosX = m_pVehicle->entity.mat->pos.X;
	float fPosY = m_pVehicle->entity.mat->pos.Y;
	float fPosZ = m_pVehicle->entity.mat->pos.Z;

	int iPlayerID = 0;
	if (m_pVehicle->pDriver) {
		iPlayerID = GamePool_Ped_GetIndex( m_pVehicle->pDriver );
		ScriptCommand( &remove_actor_from_car_and_put_at, iPlayerID, fPosX, fPosY, fPosZ + 2 );
	}

	for (int i = 0; i < 7; i++) {
		if (m_pVehicle->pPassengers[i] != NULL) {
			iPlayerID = GamePool_Ped_GetIndex( m_pVehicle->pPassengers[i] );
			ScriptCommand( &remove_actor_from_car_and_put_at, iPlayerID, fPosX, fPosY, fPosZ + 2 );
		}
	}
}

//-----------------------------------------------------------

BOOL CVehicle::AddComponent(WORD wComponentID)
{
	if (!m_pVehicle) return FALSE;
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) return FALSE;
	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR) return FALSE;

	DWORD v;
	int iWait;

	wVehicleComponentDebug = wComponentID;

	if(!ScriptCommand(&is_component_available,wComponentID))
	{
		pGame->RequestModel(wComponentID);
		pGame->LoadRequestedModels();
		ScriptCommand(&request_car_component,wComponentID);

		iWait = 20;
		while(iWait && !ScriptCommand(&is_component_available,wComponentID)) {
			Sleep(1);
			iWait--;
		}
	}

	if(ScriptCommand(&is_component_available,wComponentID))
	{
		ScriptCommand(&add_car_component,m_dwGTAId,wComponentID,&v);
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CVehicle::RemoveComponent(WORD wComponentID)
{
	if (!m_pVehicle) return FALSE;
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) return FALSE;

	ScriptCommand(&remove_component,m_dwGTAId,wComponentID);
	return TRUE;
}

//-----------------------------------------------------------

void CVehicle::SetPaintJob(BYTE bytePaintJob)
{
	if (!m_pVehicle) return;
	if (!GamePool_Vehicle_GetAt(m_dwGTAId)) return;
	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR) return;

	if(bytePaintJob <= 3) {
		ScriptCommand(&change_car_skin,m_dwGTAId,bytePaintJob);
	}
}

//-----------------------------------------------------------

BOOL CVehicle::VerifyInstance()
{
	if(GamePool_Vehicle_GetAt(m_dwGTAId)) {
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

void CVehicle::FUNC_100B8150(char *a2)
{
	strncpy(field_93,a2,32);
}

//-----------------------------------------------------------

void CVehicle::SetRotation(float fRotation)
{
	if(GamePool_Vehicle_GetAt(m_dwGTAId)) {
		ScriptCommand(&set_car_z_angle,m_dwGTAId,fRotation);
	}
}

//-----------------------------------------------------------

void CVehicle::FUNC_100B81A0()
{
	// TODO: CVehicle::FUNC_100B81A0() .text:100B81A0
}

//-----------------------------------------------------------

void CVehicle::FUNC_100B81F0()
{
	SAFE_RELEASE(field_8F);
}

//-----------------------------------------------------------

void CVehicle::SetEngineStatus(BOOL bOn)
{
	m_bEngineStatus = bOn;
}

//-----------------------------------------------------------

BOOL CVehicle::GetEngineStatus()
{
	return m_bEngineStatus;
}

//-----------------------------------------------------------

void CVehicle::SetLightStatus(BOOL bOn)
{
	m_bLightStatus = bOn;
}

//-----------------------------------------------------------

BOOL CVehicle::GetLightStatus()
{
	return m_bLightStatus;
}

//-----------------------------------------------------------

void CVehicle::OpenDoor(int iDoor, int iNodeIndex, float fAngle)
{
	if (GetVehicleSubtype() != VEHICLE_SUBTYPE_CAR) return;

	DWORD dwVehiclePtr = (DWORD)m_pVehicle;
	BOOL bPlaySound = TRUE;
	DWORD dwPedPtr = 0;

	_asm push bPlaySound
	_asm push fAngle
	_asm push iDoor
	_asm push iNodeIndex
	_asm push dwPedPtr
	_asm mov ecx, dwVehiclePtr
	_asm mov ebx, 0x6A6AE0
	_asm call ebx
}

//-----------------------------------------------------------

float CVehicle::GetBikeBankingAngle()
{
	if(m_pVehicle && GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		return m_pVehicle->fBikeBankingAngle1;
	}
	return 0.0f;
}

//-----------------------------------------------------------

void CVehicle::SetBikeBankingAngle(float fAngle)
{
	if(!m_pVehicle) return;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return;
	if(fAngle > 1.0f || fAngle < -1.0f) return;

	m_pVehicle->fBikeBankingAngle2 = fAngle;
}

//-----------------------------------------------------------

BOOL CVehicle::FUNC_100B8330()
{
	if(field_8B) return TRUE;
	return FALSE;
}

//-----------------------------------------------------------

BYTE CVehicle::GetMaxPassengers()
{
	if(m_pVehicle && GamePool_Vehicle_GetAt(m_dwGTAId))
	{
		return m_pVehicle->byteMaxPassengers;
	}
	return 0;
}

//-----------------------------------------------------------

void CVehicle::SetWindowOpenFlag(BYTE byteDoorID)
{
	if(m_pVehicle) {
		DWORD dwVehiclePtr = (DWORD)m_pVehicle;

		_asm mov ecx, dwVehiclePtr
		_asm xor edx, edx
		_asm mov dl, byteDoorID
		_asm push edx
		_asm mov edx, 0x6D3080
		_asm call edx
	}
}

//-----------------------------------------------------------

void CVehicle::ClearWindowOpenFlag(BYTE byteDoorID)
{
	if(m_pVehicle) {
		DWORD dwVehiclePtr = (DWORD)m_pVehicle;

		_asm mov ecx, dwVehiclePtr
		_asm xor edx, edx
		_asm mov dl, byteDoorID
		_asm push edx
		_asm mov edx, 0x6D30B0
		_asm call edx
	}
}

//-----------------------------------------------------------

void CVehicle::FUNC_100B8A70(BOOL a2)
{
	// TODO: CVehicle::FUNC_100B8A70 .text:100B8A70
}

//-----------------------------------------------------------
