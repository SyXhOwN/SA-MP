
#ifndef SAMPSRV_VEHICLEPOOL_H
#define SAMPSRV_VEHICLEPOOL_H

class CVehiclePool
{
private:
	char field_0[212];
	int m_iVirtualWorld[MAX_VEHICLES];
	BOOL m_bVehicleSlotState[MAX_VEHICLES];
	CVehicle *m_pVehicles[MAX_VEHICLES];
	int m_iPoolSize;

public:
	CVehiclePool();
	~CVehiclePool();

	VEHICLEID New(int iVehicleType, VECTOR * vecPos, float fRotation, int iColor1, int iColor2, int iRespawnDelay);

	BOOL Delete(VEHICLEID VehicleID);

	// Find out if the slot is inuse.
	BOOL GetSlotState(VEHICLEID VehicleID)
	{
		if(VehicleID >= MAX_VEHICLES) { return FALSE; }
		return m_bVehicleSlotState[VehicleID];
	};

	void Process(float fElapsedTime);

	int GetModelCount();

	int GetPoolSize() { return m_iPoolSize; };

	int GetVehicleVirtualWorld(VEHICLEID VehicleID) {
		if (VehicleID >= MAX_VEHICLES) { return 0; }
		return m_iVirtualWorld[VehicleID];		
	};
};

#endif
