
#pragma once

#define GAMESTATE_NONE			0 // used for debugging modes and such
#define GAMESTATE_CONNECTING	1
#define GAMESTATE_CONNECTED		2
#define GAMESTATE_AWAIT_JOIN	3

#define GAMESTATE_RESTARTING	5

//----------------------------------------------------

class CNetGame // size: 910 bytes
{
private:
	RakClientInterface	*m_pRakClient;
	CPlayerPool			*m_pPlayerPool;
	CVehiclePool		*m_pVehiclePool;
	int					m_iGameState;
	BOOL				m_bLanMode;

	void UpdateNetwork();

	// Packet handlers
	void Packet_AimSync(Packet *p);
	void Packet_PassengerSync(Packet *p);
	void Packet_ConnectionSucceeded(Packet *p);
	void Packet_ConnectionBanned(Packet* packet);
	void Packet_DisconnectionNotification(Packet* packet);
	void Packet_ModifiedPacket(Packet* packet);
	void Packet_ConnectAttemptFailed(Packet* packet);
	void Packet_TrailerSync(Packet *p);

	int			m_iSpawnsAvailable;
	char _gap18[6];
	BYTE		m_byteWorldTime;
	BYTE		m_byteWorldMinute;
	BYTE		m_byteWeather;
	char _gap21[16];
	BYTE		m_byteFriendlyFire;
	char _gap32[2];
	float		m_fGravity;
	int			m_iDeathDropMoney;
	BYTE		m_byteHoldTime;
	bool		m_bInstagib;
	bool		m_bZoneNames;
	char _gap3F;
	bool		m_bUseCJWalk;
	char _gap41[9];
	DWORD		m_dwMapIcon[100];
	int field_1DA;
	int field_1DE;
	int field_1E2;
	char _gap1E6[12];
	int field_1F2;
	char _gap1F6[4];
	int field_1FA;
	int field_1FE;

	char m_szHostName[256];
	char m_szHostOrIp[128];
	int m_iPort;

	CGameMode* m_pGameMode;
	CScriptTimers* m_pScriptTimers;

public:

	CNetGame();
	~CNetGame();

	int GetGameState() { return m_iGameState; };

	CPlayerPool * GetPlayerPool() { return m_pPlayerPool; };
	RakClientInterface * GetRakClient() { return m_pRakClient; };
	CGameMode * GetBotMode() { return m_pGameMode; };

	void Init(PCHAR szHostOrIp,int iPort,PCHAR szPlayerName,PCHAR szPass,PCHAR szNpcMode);
	void Process();
	void ResetVehiclePool();
	void ResetPlayerPool();
	void ShutdownForGameModeRestart();

	void sub_415EA0(PLAYERID playerId, BOOL a2);
	void sub_415EC0(VEHICLEID VehicleID, BOOL a2);

	void StopRecordingPlayback();
};

//----------------------------------------------------
