
#pragma once

#include "game.h"
#include "entity.h"

#include <d3d9.h>

enum eLandingGearState 
{
	LGS_UP,
	LGS_DOWN,
};

//-----------------------------------------------------------

class CVehicle : public CEntity
{
public:
	//char _gap0[184];
	CVehicle*   m_pTrailer;
	VEHICLE_TYPE	*m_pVehicle;
	BOOL			m_bEngineStatus;
	BOOL			m_bLightStatus;
	BOOL		m_bIsInvulnerable;

	char _gap5C[4];

	BOOL		m_bDoorsLocked; // Vehicle is enterable TRUE/FALSE
	BYTE		m_byteObjectiveVehicle; // Is this a special objective vehicle? 0/1
	BOOL		m_bSpecialMarkerEnabled;
	DWORD		m_dwTimeSinceLastDriven;
	BOOL		m_bHasBeenDriven;
	int			field_71;

	char _gap75[4];

	BYTE		m_byteColor1;
	BYTE		m_byteColor2;
	BOOL		m_bHasNewColor;

	BOOL		m_bUnoccupiedSync;
	BOOL		m_bRemoteUnocSync;
	BOOL		m_bKeepModelLoaded;
	int			field_8B;
	IDirect3DTexture9 *field_8F;
	char		field_93[33];

	CVehicle( int iType, float fPosX, float fPosY, float fPosZ, float fRotation = 0.0f, BOOL bKeepModelLoaded = FALSE, int a8 = 0);
	virtual ~CVehicle();

	virtual void Add();
	virtual void Remove();

	void  ResetPointers();

	void  ProcessMarkers();
	int	  m_dwMarkerID;

	void  SetLockedState(int iLocked);
	UINT  GetVehicleSubtype();

	float GetHealth();
	void  SetHealth(float fHealth);
	void SetColor(BYTE byteColor1, BYTE byteColor2);
	void UpdateColor();

	BOOL  HasSunk();
	BOOL  IsWrecked();
	BOOL  IsDriverLocalPlayer();
	BOOL  IsLocalPlayerInThisVehicle();
	BOOL  IsATrainPart();
	BOOL  HasTurret();
	BOOL  HasADriver();

	void  SetHydraThrusters(DWORD dwDirection);
	DWORD GetHydraThrusters();

	void  SetTankRot(float X, float Y);
	float GetTankRotX();
	float GetTankRotY();

	float GetTrainSpeed();
	void  SetTrainSpeed(float fSpeed);

	UINT  GetPassengersMax();

	void    SetSirenOn(BYTE byteState);
	BOOL    IsSirenOn();
	void	SetAlarmState(WORD wState);
	void    SetLandingGearState(eLandingGearState state);
	eLandingGearState	GetLandingGearState();
	void	SetInvulnerable(BOOL bInv);

	void    SetEngineState(BOOL bState);
	void	SetDoorState(int iState);
	void	UpdateDamageStatus(DWORD dwPanelDamage, DWORD dwDoorDamage, BYTE byteLightDamage);
	DWORD	GetPanelDamageStatus();
	DWORD	GetDoorDamageStatus();
	BYTE	GetLightDamageStatus();

	void	LinkToInterior(int iInterior);
	void	SetWheelPopped(BYTE byteFlags);
	BYTE	GetWheelPopped();
	void	AttachTrailer();
	void	DetachTrailer();
	void    SetTrailer(CVehicle *pTrailer);
	CVehicle* GetTrailer();
	CVehicle* GetTractor();
	BOOL	IsATrailer();
	BOOL	IsTowTruck();
	BOOL	IsRCVehicle();

	void	ToggleLights(BYTE bEnable = 25);
	bool	AreLightsEnabled();

	void	SetHornState(BYTE byteState);

	BOOL	IsOccupied();
	BOOL	UpdateLastDrivenTime();

	void	RemoveEveryoneFromVehicle();

	void SetPaintJob(BYTE bytePaintJob);
	BOOL AddComponent(WORD wComponentID);
	BOOL RemoveComponent(WORD wComponentID);
	BOOL VerifyInstance();
	void FUNC_100B8150(char *a2);
	void SetRotation(float fRotation);
	void FUNC_100B81A0();
	void FUNC_100B81F0();
	void SetEngineStatus(BOOL bOn);
	BOOL GetEngineStatus();
	void SetLightStatus(BOOL bOn);
	BOOL GetLightStatus();
	void OpenDoor(int iDoor, int iNodeIndex, float fAngle);
	float GetBikeBankingAngle();
	void SetBikeBankingAngle(float fAngle);
	BOOL FUNC_100B8330();

	BYTE GetMaxPassengers();
	void SetWindowOpenFlag(BYTE byteDoorID);
	void ClearWindowOpenFlag(BYTE byteDoorID);
	void FUNC_100B8A70(BOOL a2);
};

//-----------------------------------------------------------
