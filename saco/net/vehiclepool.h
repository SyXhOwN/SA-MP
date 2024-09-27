
#pragma once

#define INVALID_VEHICLE_ID			0xFFFF
#define MAX_VEHICLE_WAITING_SLOTS	100

#pragma pack(1)
typedef struct _NEW_VEHICLE {
	//char _gap0[40];
	VEHICLEID VehicleId;
	int		  iVehicleType;
	char _gap6[34];
} NEW_VEHICLE;

//----------------------------------------------------

class CVehiclePool
{
private:

	//char _gap0[96408];
	int field_0;
	NEW_VEHICLE			m_NewVehicleWaiting[MAX_VEHICLE_WAITING_SLOTS];
	BOOL				m_bWaitingSlotState[MAX_VEHICLE_WAITING_SLOTS];
	CVehicle			*m_pVehicles[MAX_VEHICLES];
	BOOL				m_bVehicleSlotState[MAX_VEHICLES];
	VEHICLE_TYPE		*m_pGTAVehicles[MAX_VEHICLES];
	char _gap6EF4[12000];
	int field_9DD4[2000];
	BOOL				m_bIsActive[MAX_VEHICLES];
	char _gapDC54[40000];
	int field_17894;

public:

	CVehiclePool();
	~CVehiclePool();

	BOOL New(NEW_VEHICLE *pNewVehicle);

	BOOL Delete(VEHICLEID VehicleID);	

	// Retrieve a vehicle
	CVehicle* GetAt(VEHICLEID VehicleID) {
		if(VehicleID >= MAX_VEHICLES || !m_bVehicleSlotState[VehicleID]) { return NULL; }
		return m_pVehicles[VehicleID];
	};

	// Find out if the slot is inuse.
	BOOL GetSlotState(VEHICLEID VehicleID) {
		if(VehicleID >= MAX_VEHICLES) { return FALSE; }
		return m_bVehicleSlotState[VehicleID];
	};

	VEHICLEID FindIDFromGtaPtr(VEHICLE_TYPE * pGtaVehicle);

	int FindGtaIDFromID(int iID);
	int FindGtaIDFromGtaPtr(VEHICLE_TYPE * pGtaVehicle);

	void NotifyVehicleDeath(VEHICLEID VehicleID);

	int FindNearestToLocalPlayerPed();

	void ProcessWaitingList();
	void NewWhenModelLoaded(NEW_VEHICLE *pNewVehicle);
};

//----------------------------------------------------
