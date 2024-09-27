
#include "main.h"

CVehiclePool::CVehiclePool()
{
	for(VEHICLEID VehicleID = 0; VehicleID != MAX_VEHICLES; VehicleID++) {
		m_bVehicleSlotState[VehicleID] = FALSE;
		m_pVehicles[VehicleID] = NULL;
		m_iVirtualWorld[VehicleID] = 0;
	}
	memset(field_0,0,sizeof(field_0));
	m_iPoolSize = 0;
}

CVehiclePool::~CVehiclePool()
{
	for(VEHICLEID VehicleID = 0; VehicleID != MAX_VEHICLES; VehicleID++) {
		Delete(VehicleID);
	}
}

VEHICLEID CVehiclePool::New(int iVehicleType,
					   VECTOR * vecPos, float fRotation,
					   int iColor1, int iColor2, int iRespawnDelay)
{
	VEHICLEID VehicleID;

	if(iVehicleType < 400 || iVehicleType > 611)
		return 0xFFFF;

	for(VehicleID=1; VehicleID != MAX_VEHICLES; VehicleID++)
	{
		if(m_bVehicleSlotState[VehicleID] == FALSE) break;
	}

	if(VehicleID == MAX_VEHICLES) return 0xFFFF;

	m_pVehicles[VehicleID] = new CVehicle(iVehicleType,vecPos,fRotation,iColor1,iColor2,iRespawnDelay);

	if(m_pVehicles[VehicleID])
	{
		// TODO: CVehiclePool::New

		m_pVehicles[VehicleID]->SetID(VehicleID);
		m_bVehicleSlotState[VehicleID] = TRUE;

		return VehicleID;
	}
	else
	{
		return 0xFFFF;
	}
}

BOOL CVehiclePool::Delete(VEHICLEID VehicleID)
{
	// TODO: CVehiclePool::Delete W .text:00467700 L .text:0814CC10
	return FALSE;
}

void CVehiclePool::Process(float fElapsedTime)
{
	// TODO: CVehiclePool::Process
}

int CVehiclePool::GetModelCount()
{
	int iCount=0;
	for (int i=0; i!=212; i++) {
		if(field_0[i]) {
			iCount++;
		}
	}
	return iCount;
}
