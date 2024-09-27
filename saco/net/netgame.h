
#pragma once

#define GAMESTATE_CONNECTED		5
#define GAMESTATE_AWAIT_JOIN	6

#define GAMESTATE_CONNECTING	2
#define GAMESTATE_WAIT_CONNECT	1

#define NETMODE_NORMAL_ONFOOT_SENDRATE	30
#define NETMODE_NORMAL_INCAR_SENDRATE	30
#define NETMODE_FIRING_SENDRATE			30
#define NETMODE_SEND_MULTIPLIER			2

struct struc_41
{
	bool  bUseCJWalk;
	int   iDeathDropMoney;
	float WorldBounds[4];
	bool  bAllowWeapons;
	float fGravity;
	bool  bDisableEnterExits;
	BOOL  bVehicleFriendlyFire;
	BYTE  byteHoldTime;
	char  field_20;
	bool  bZoneNames;
	char  field_22;
	int   iSpawnsAvailable;
	float fNameTagDrawDistance;
	bool  bManualVehicleEngineAndLights;
	BYTE  byteWorldTime;
	BYTE  byteWorldMinute;
	BYTE  byteWeather;
	bool  bNameTagLOS;
	int   iShowPlayerMarkers;
	float fGlobalChatRadius;
	bool  bShowNameTags;
	bool  bLimitGlobalChatRadius;
};

typedef struct _NETGAME_POOLS
{
	CVehiclePool *pVehiclePool;
	CPlayerPool *pPlayerPool;
	CPickupPool *pPickupPool;
	CObjectPool *pObjectPool;
	CActorPool *pActorPool;
	CGangZonePool *pGangZonePool;
	CLabelPool *pLabelPool;
	CTextDrawPool *pTextDrawPool;
	CMenuPool *pMenuPool;
} NETGAME_POOLS;

//----------------------------------------------------

class CNetGame
{
//private:
public:
	// Should always be the first one in class (for the sake of ACGhost)
	RakClientInterface	*m_pRakClient;

	char _gap4[44];

	char				m_szHostOrIp[257];
	char				m_szHostName[257];
	char				field_232;
	char				field_233;
	bool				m_bPlayerNameTagStatus;
	int					m_iPort;
	int					field_239;
	DWORD				m_dwMapIcon[100];
	int					m_iGameState;
	DWORD				m_dwLastConnectAttempt;
	struc_41			*field_3D5;
	char				field_3D9;

	char _gap3DA[4];

	NETGAME_POOLS *m_pPools;

	void UpdateNetwork();

	// Packet handlers
	void Packet_AimSync(Packet *p);
	void Packet_PlayerSync(Packet *p);
	void Packet_VehicleSync(Packet *p);
	void Packet_PassengerSync(Packet *p);
	void Packet_UnoccupiedSync(Packet *p);
	void Packet_WeaponShotSync(Packet *p);
	void Packet_ConnectionSucceeded(Packet *p);
	void Packet_RSAPublicKeyMismatch(Packet* packet);
	void Packet_ConnectionBanned(Packet* packet);
	void Packet_ConnectionRequestAccepted(Packet* packet);
	void Packet_NoFreeIncomingConnections(Packet* packet);
	void Packet_DisconnectionNotification(Packet* packet);
	void Packet_ConnectionLost(Packet* packet);
	void Packet_InvalidPassword(Packet* packet);
	void Packet_ModifiedPacket(Packet* packet);
	void Packet_ConnectAttemptFailed(Packet* packet);
	void Packet_TrailerSync(Packet *p);
	void FUNC_10009F10(Packet *p);
	void FUNC_1000A100(Packet *p);

public:

	CNetGame(PCHAR szHostOrIp,int iPort,PCHAR szPlayerName,PCHAR szPass);
	~CNetGame();

	void PrintWelcomeMessage();
	void HandleConnectingState();
	void ShowWaitingScreen();

	void InitPools();
	void ResetAllPools();

	int GetGameState() { return m_iGameState; };
	void SetGameState(int iGameState) { m_iGameState = iGameState; };

	CPlayerPool * GetPlayerPool() { return m_pPools->pPlayerPool; };
	CVehiclePool * GetVehiclePool() { return m_pPools->pVehiclePool; };
	CActorPool * GetActorPool() { return m_pPools->pActorPool; };
	CPickupPool * GetPickupPool() { return m_pPools->pPickupPool; };
	CObjectPool	* GetObjectPool() { return m_pPools->pObjectPool; };
	CMenuPool * GetMenuPool() { return m_pPools->pMenuPool; };
	CLabelPool * GetLabelPool() { return m_pPools->pLabelPool; };
	CTextDrawPool * GetTextDrawPool() { return m_pPools->pTextDrawPool; };
	CGangZonePool * GetGangZonePool() { return m_pPools->pGangZonePool; };
	RakClientInterface * GetRakClient() { return m_pRakClient; };

	void InitGameLogic();
	void Process();

	void UpdatePlayerScoresAndPings();
	void ResetVehiclePool();
	void ResetPlayerPool();
	void ResetPickupPool();
	void ResetObjectPool();
	void ResetMenuPool();
	void ResetLabelPool();
	void ResetTextDrawPool();
	void ResetGangZonePool();
	void ResetActorPool();

	void  ResetMapIcons();
	void  SetMapIcon(BYTE byteIndex, float fX, float fY, float fZ, BYTE byteIcon, DWORD dwColor, int iStyle);
	void  DisableMapIcon(BYTE byteIndex);

	int GetTime();
	void TogglePlayerTagStatus();
};

//----------------------------------------------------
