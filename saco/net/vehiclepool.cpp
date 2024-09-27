
#include "../main.h"
#include "../game/util.h"

extern CGame *pGame;
extern CChatWindow *pChatWindow;

//----------------------------------------------------

CVehiclePool::CVehiclePool()
{
	// loop through and initialize all vehicle properties to 0
	for(VEHICLEID VehicleID = 0; VehicleID < MAX_VEHICLES; VehicleID++) {
		m_bVehicleSlotState[VehicleID] = FALSE;
		m_pVehicles[VehicleID] = NULL;
		m_pGTAVehicles[VehicleID] = NULL;
		field_9DD4[VehicleID] = GetTickCount();
	}
	memset(&m_bWaitingSlotState[0],0,MAX_VEHICLE_WAITING_SLOTS * sizeof(BOOL));
	field_0 = 0;
	field_17894 = 1;
}

//----------------------------------------------------

CVehiclePool::~CVehiclePool()
{
	for(VEHICLEID VehicleID = 0; VehicleID < MAX_VEHICLES; VehicleID++) {
		Delete(VehicleID);
	}
}

//----------------------------------------------------

BOOL CVehiclePool::New(NEW_VEHICLE *pNewVehicle)
{
	if(m_pVehicles[pNewVehicle->VehicleId] != NULL) {
		pChatWindow->AddDebugMessage("Warning: vehicle %u was not deleted",pNewVehicle->VehicleId);
		Delete(pNewVehicle->VehicleId);
	}

	// TODO: CVehiclePool::New .text:1001F080

	return FALSE;
}

//----------------------------------------------------

BOOL CVehiclePool::Delete(VEHICLEID VehicleID)
{
	if(!GetSlotState(VehicleID) || !m_pVehicles[VehicleID])
	{
		return FALSE; // Vehicle already deleted or not used.
	}

	m_bVehicleSlotState[VehicleID] = FALSE;
	delete m_pVehicles[VehicleID];
	m_pVehicles[VehicleID] = NULL;
	m_pGTAVehicles[VehicleID] = 0;

	// TODO: CVehiclePool::Delete .text:1001EA80

	return TRUE;
}

//----------------------------------------------------



//----------------------------------------------------

VEHICLEID CVehiclePool::FindIDFromGtaPtr(VEHICLE_TYPE * pGtaVehicle)
{
	int x=1;

	while(x<=field_0) {
		if(pGtaVehicle == m_pGTAVehicles[x]) return x;
		x++;
	}

	return INVALID_VEHICLE_ID;
}

//----------------------------------------------------

int CVehiclePool::FindGtaIDFromID(int iID)
{
	if(m_pGTAVehicles[iID]) {
		return GamePool_Vehicle_GetIndex(m_pGTAVehicles[iID]);
	} else {
		return INVALID_VEHICLE_ID;
	}
}

//----------------------------------------------------

int CVehiclePool::FindGtaIDFromGtaPtr(VEHICLE_TYPE * pGtaVehicle)
{
	if(pGtaVehicle) {
		return GamePool_Vehicle_GetIndex(pGtaVehicle);
	} else {
		return INVALID_VEHICLE_ID;
	}
}

//-----------------------------------------------------------


//----------------------------------------------------

void CVehiclePool::NotifyVehicleDeath(VEHICLEID VehicleID)
{
	{
		RakNet::BitStream bsDeath;
		bsDeath.Write(VehicleID);

		// CVehiclePool::NotifyVehicleDeath .text:1001F230
	}
}

//----------------------------------------------------

int CVehiclePool::FindNearestToLocalPlayerPed()
{
	float fLeastDistance=10000.0f;
	float fThisDistance;
	VEHICLEID ClosestSoFar=INVALID_VEHICLE_ID;

	VEHICLEID x=0;
	while(x <= field_0) {
		if(GetSlotState(x) && m_bIsActive[x]) {
			fThisDistance = m_pVehicles[x]->GetDistanceFromLocalPlayerPed();
			if(fThisDistance < fLeastDistance) {
				fLeastDistance = fThisDistance;
				ClosestSoFar = x;
			}
		}
		x++;
	}

	return ClosestSoFar;
}

//----------------------------------------------------

void CVehiclePool::ProcessWaitingList()
{
	int x=0;
	while(x!=MAX_VEHICLE_WAITING_SLOTS) {
		if(m_bWaitingSlotState[x] && pGame->IsModelLoaded(m_NewVehicleWaiting[x].iVehicleType))
		{
			New(&m_NewVehicleWaiting[x]);

			// TRAIN STUFF
			if(m_NewVehicleWaiting[x].iVehicleType == TRAIN_FREIGHT_LOCO) {
				m_NewVehicleWaiting[x].iVehicleType = TRAIN_FREIGHT;

				m_NewVehicleWaiting[x].VehicleId++;
				New(&m_NewVehicleWaiting[x]);

				m_NewVehicleWaiting[x].VehicleId++;
				New(&m_NewVehicleWaiting[x]);

				m_NewVehicleWaiting[x].VehicleId++;
				New(&m_NewVehicleWaiting[x]);	
			}

			if(m_NewVehicleWaiting[x].iVehicleType == TRAIN_PASSENGER_LOCO) {
				m_NewVehicleWaiting[x].iVehicleType = TRAIN_PASSENGER;

				m_NewVehicleWaiting[x].VehicleId++;
				New(&m_NewVehicleWaiting[x]);

				m_NewVehicleWaiting[x].VehicleId++;
				New(&m_NewVehicleWaiting[x]);

				m_NewVehicleWaiting[x].VehicleId++;
				New(&m_NewVehicleWaiting[x]);
			}
			m_bWaitingSlotState[x] = FALSE;
		}
		x++;
	}
}

//----------------------------------------------------

void CVehiclePool::NewWhenModelLoaded(NEW_VEHICLE *pNewVehicle)
{
	// find a free slot
	int x=0;
	while(x!=MAX_VEHICLE_WAITING_SLOTS) {
		if(m_bWaitingSlotState[x] == FALSE) break;
		x++;
	}

	if(x==MAX_VEHICLE_WAITING_SLOTS) {
		pChatWindow->AddDebugMessage("All vehicle waiting slots are consumed!");
		return;
	}

    m_bWaitingSlotState[x] = TRUE;
	memcpy(&m_NewVehicleWaiting[x],pNewVehicle,sizeof(NEW_VEHICLE));
}

//----------------------------------------------------
