
#include "main.h"
#include "../raknet/SocketDataEncryptor.h"

float fRestartWaitTime=0.0f;

int CanFileBeOpenedForReading(char * filename);

char szGameModeFile[256];

//----------------------------------------------------

// This is from RakNet sources.
BYTE GetPacketID(Packet *p) 
{
	if (p==0) return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP) {
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else return (unsigned char) p->data[0];
}

//----------------------------------------------------

bool HasTimestamps(Packet *p)
{
	if (p==0) return 0;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		return true;
	return false;
}

//----------------------------------------------------

DWORD CNetGame::GetTime()
{
	return (DWORD)RakNet::GetTime();
}

//----------------------------------------------------

CNetGame::CNetGame()
{
	m_bAllowWeapons = FALSE;
	m_byteWorldTime = 12;
	m_byteWeather	= 1;
	m_bStuntBonus   = true;
	m_fGravity		= 0.008f;
	m_iDeathDropMoney = 0;
	field_6A		= false;
	m_bVehicleFriendlyFire = FALSE;

	// Init member variables
	m_pPlayerPool = NULL;
	m_pVehiclePool = NULL;
	m_pGameMode = NULL;
	m_pPickupPool = NULL;
	m_pObjectPool = NULL;
	m_pMenuPool = NULL;
	m_pTextPool = NULL;
	m_bNameTagLOS = true;
	m_pGangZonePool = NULL;
	m_pLabelPool = NULL;
	m_pActorPool = NULL;
	m_bLanMode = FALSE;
	field_6B = 1;

	m_bLimitGlobalChatRadius = FALSE;
	m_fGlobalChatRadius = 10000.0f;
	m_bLimitPlayerMarkerRadius = FALSE;
	m_fPlayerMarkerRadius = 10000.0f;
	m_fNameTagDrawDistance = 70.0f;
	m_bDisableEnterExits = false;
	field_5C = 0;

	m_iCurrentGameModeIndex = 0;
	m_iCurrentGameModeRepeat = 0;
	m_bFirstGameModeLoaded = FALSE;

	m_pScriptTimers = new CScriptTimers;
	m_pScriptHttps = new CScriptHttps;

#ifndef WIN32
	m_dElapsedTime = 0.0;
#endif

	if(pConsole->GetIntVariable("maxplayers") > MAX_PLAYERS) {
		pConsole->SetIntVariable("maxplayers", MAX_PLAYERS);
	}

	CHAR *szBindAddress = pConsole->GetStringVariable("bind");
	if (szBindAddress && szBindAddress[0] == 0)
		szBindAddress = NULL;

	DWORD dwPort = pConsole->GetIntVariable("port");
	DWORD dwMaxPlayers = pConsole->GetIntVariable("maxplayers");
	BOOL bLanMode = pConsole->GetBoolVariable("lanmode");

	// Setup RakNet
	m_pRak = RakNetworkFactory::GetRakServerInterface();

	SocketDataEncryptor::SetKey(dwPort);



	// TODO: CNetGame::CNetGame W: 0048EEE0 L: 080AF360

/*
  sub_496D10(v9);
  dword_515CFC = sub_44E9D0();
  _this->field_44 = sub_48DBD0(_this);
  _this->field_48 = 0;
  _this->field_4C = 0;
  sub_46A7B0(v8);
  if ( (unsigned __int8)(*(int (__stdcall **)(int, _DWORD, int, int, char *))(*(_DWORD *)_this->field_40 + 4))(
                          v19,
                          0,
                          iSleep,
                          v8,
                          v7) )
  {
    sub_48EAE0(_this);
    if ( !sub_48DD50(0) )
    {
      logprintf("I couldn't load any gamemode scripts. Please verify your server.cfg");
      logprintf("It needs a gamemode0 line at the very least.");
      _fcloseall();
      exit(1);
    }
    (*(void (**)(void))(*(_DWORD *)_this->field_40 + 84))();
    v10 = CConsole::GetStringVariable(pConsole, "password");
    if ( v10 && *v10 )
      (*(void (__stdcall **)(char *))(*(_DWORD *)_this->field_40 + 16))(v10);
    sprintf(&pInitStr, "%02d:%02d", _this->field_59, 0);
    CConsole::AddStringVariable(pConsole, "worldtime", 4u, &pInitStr, 0);
    if ( v20 )
      _this->field_50 = 1;
    GetCurrentDirectoryA(0x200u, &Value);
    v11 = strlen(&Value);
    if ( *((_BYTE *)&v22 + v11 + 3) != 92 )
    {
      *(&Value + v11) = 92;
      v24[v11] = 0;
    }
    v12 = (char *)&v22 + 3;
    do
      v13 = (v12++)[1];
    while ( v13 );
    *(_DWORD *)v12 = 1769104243;
    *((_DWORD *)v12 + 1) = 1768322160;
    *((_DWORD *)v12 + 2) = 1551066476;
    v12[12] = 0;
    SetEnvironmentVariableA("AMXFILE", &Value);
    v14 = operator new(0x1034u);
    v22 = v14;
    v26 = 2;
    if ( v14 )
      v15 = sub_46A7C0(v14);
    else
      v15 = 0;
    _this->field_4 = v15;
    if ( v21 )
    {
      v16 = (int (__stdcall **)(int, int, int))"ON";
      if ( !v20 )
        v16 = &off_4B9C04;
      printf(
        "\nStarted server on %s:%d, with maxplayers: %d lanmode is %s.\n\n",
        v21,
        (unsigned __int16)v8,
        (unsigned __int16)v19,
        v16);
    }
    else
    {
      v17 = (int (__stdcall **)(int, int, int))"ON";
      if ( !v20 )
        v17 = &off_4B9C04;
      printf(
        "\nStarted server on port: %d, with maxplayers: %d lanmode is %s.\n\n",
        (unsigned __int16)v8,
        (unsigned __int16)v19,
        v17);
    }
    _this->field_5E = 0;
  }
  else if ( v7 )
  {
    logprintf("Unable to start server on %s:%d. Port in use?", v7, (unsigned __int16)v8);
  }
  else
  {
    logprintf("Unable to start server on port: %d. Port in use?", (unsigned __int16)v8);
  }*/
}

//----------------------------------------------------

CNetGame::~CNetGame()
{
	logprintf("--- Server Shutting Down.");

	SAFE_DELETE(m_pGameMode);
	SAFE_DELETE(m_pFilterScripts);
	SAFE_DELETE(m_pScriptTimers);
	SAFE_DELETE(m_pScriptHttps);
	SAFE_DELETE(m_pLabelPool);
	SAFE_DELETE(m_pVehiclePool);
	SAFE_DELETE(m_pPlayerPool);
	SAFE_DELETE(m_pObjectPool);
	SAFE_DELETE(m_pPickupPool);
	SAFE_DELETE(m_pMenuPool);
	SAFE_DELETE(m_pTextPool);
	SAFE_DELETE(m_pGangZonePool);
	SAFE_DELETE(m_pActorPool);

	m_pRak->Disconnect(100);
	UnRegisterRPCs(m_pRak);

	RakNetworkFactory::DestroyRakServerInterface(m_pRak);
}

//----------------------------------------------------
// Loads filterscripts after pools initialisation

void CNetGame::LoadAllFilterscripts()
{
	logprintf("");
	logprintf("Filterscripts");
	logprintf("---------------");
	int iScriptCount = 0;
	char* szFilterScript = strtok(pConsole->GetStringVariable("filterscripts"), " ");
	while (szFilterScript)
	{
		logprintf("  Loading filterscript '%s.amx'...", szFilterScript);
		if (m_pFilterScripts->LoadFilterScript(szFilterScript))
		{
			iScriptCount++;
		} else {
			logprintf("  Unable to load filterscript '%s.amx'.", szFilterScript);
		}
		szFilterScript = strtok(NULL, " ");
	}
	logprintf("  Loaded %d filterscripts.\n", iScriptCount);
}

//----------------------------------------------------
// Handles rotation and setting of the current
// script file to be used. If szFile is NULL, it
// will attempt to rotate scripts as per configuration.
// returns FALSE if it was not able to set the script,
// true otherwise.

// Returns the name of the next gamemode
// Code taken from SetNextScriptFile but required by "gmx" (easiest way without major re-write)
char *CNetGame::GetNextScriptFile()
{
	char *szTemp;
	char szCurGameModeConsoleVar[64];

	m_iCurrentGameModeIndex++;

	sprintf(szCurGameModeConsoleVar,"gamemode%u",m_iCurrentGameModeIndex);
	szTemp = strtok(pConsole->GetStringVariable(szCurGameModeConsoleVar), " ");

	// if the var isn't there then cycle back to 0
	if(!szTemp || !strlen(szTemp)) {
		m_iCurrentGameModeIndex = 0;
		sprintf(szCurGameModeConsoleVar,"gamemode%u",m_iCurrentGameModeIndex);
		szTemp = strtok(pConsole->GetStringVariable(szCurGameModeConsoleVar), " ");
	}

	// if it's still NULL then we've got an error.
	if(!szTemp || !strlen(szTemp)) return NULL;
	return szTemp;
}

BOOL CNetGame::SetNextScriptFile(char *szFile)
{
	char szConfigFileName[64];
	char *szTemp;
	int  iConfigRepeatCount=0;

	if(NULL == szFile) {
		// rotate by config

		if(m_iCurrentGameModeRepeat || !m_bFirstGameModeLoaded) {
			// repeats of this script, cycle to the current
			m_iCurrentGameModeIndex--;
		}

		szTemp = this->GetNextScriptFile();
		if (szTemp == NULL) return false;

		sscanf(szTemp,"%s%d",szConfigFileName,&iConfigRepeatCount);

		// set it and verify the file is readable
		sprintf(szGameModeFile,"gamemodes/%s.amx",szConfigFileName);

		if(!CanFileBeOpenedForReading(szGameModeFile)) {
			return FALSE;
		}

		if(!m_iCurrentGameModeRepeat) {
			m_iCurrentGameModeRepeat = iConfigRepeatCount;
		}

		m_iCurrentGameModeRepeat--;

		m_bFirstGameModeLoaded = TRUE;

		return TRUE;

	} else {
		// set the script from szFile

		// set it and verify the file is readable
		sprintf(szGameModeFile,"gamemodes/%s.amx",szFile);

		if(!CanFileBeOpenedForReading(szGameModeFile)) {
			return FALSE;
		}

		m_iCurrentGameModeRepeat = 0;

		return TRUE;
	}

}

//----------------------------------------------------

void CNetGame::Init(BOOL bFirst = false)
{
	m_iSpawnsAvailable = 0;

	// Setup player pool
	if(!m_pPlayerPool) {
		m_pPlayerPool = new CPlayerPool();
	} else {
		m_pPlayerPool->ResetPlayerScoresAndMoney();
	}

	// Setup vehicle pool
	if(!m_pVehiclePool) {
		m_pVehiclePool = new CVehiclePool();
	}

	// Setup pickup pool
	if(!m_pPickupPool) {
		m_pPickupPool = new CPickupPool();
	}

	// Setup Object pool
	if (!m_pObjectPool) {
		m_pObjectPool = new CObjectPool();
	}

	// Setup Menu pool
	if (!m_pMenuPool) {
		m_pMenuPool = new CMenuPool();
	}

	// Setup Text pool
	if (!m_pTextPool) {
		m_pTextPool = new CTextDrawPool();
	}

	// Setup Gang Zone pool
	if (!m_pGangZonePool) {
		m_pGangZonePool = new CGangZonePool();
	}

	// Setup Label pool
	if (!m_pLabelPool) {
		m_pLabelPool = new CLabelPool();
	}

	// Setup Actor pool
	if (!m_pActorPool) {
		m_pActorPool = new CActorPool();
	}

	// Setup gamemode
	if(!m_pGameMode) {
		m_pGameMode = new CGameMode();
	}

	// Default tags/markers
	m_bShowNameTags = TRUE;
	m_iShowPlayerMarkers = 1;
	m_bNameTagLOS = true;
	m_bUseCJWalk = FALSE;

	// Set the default world time for clients.
	m_byteWorldTime = 12; // 12:00

	// Set the default weather
	m_byteWeather   = 1;

	m_bLimitGlobalChatRadius = FALSE;
	m_bLimitPlayerMarkerRadius = FALSE;
	m_fGlobalChatRadius = 10000.0f;
	m_fPlayerMarkerRadius = 10000.0f;
	m_fNameTagDrawDistance = 70.0f;
	m_bDisableEnterExits = false;
	m_bManualVehicleEngineAndLights = false;

	if (bFirst) LoadAllFilterscripts();
	// Has to be done here as it need to be AFTER the pools but BEFORE the gamemode

	// Start the gamemode script.
	m_pGameMode->Load(szGameModeFile);

	logprintf("Number of vehicle models: %d", m_pVehiclePool->GetModelCount());

	// Flag we're in a running state.
	m_iGameState = GAMESTATE_RUNNING;
}

void CNetGame::ShutdownForGameModeRestart()
{
	// TODO: CNetGame::ShutdownForGameModeRestart W: 0048F430 L: 080ABBE0

/*
  sub_804C8A0(&v8);
  sub_80ABAD0(this, "(", &v8, -1, 2);
  sub_80D1690(this->field_8);
  v1 = (void *)this->field_0;
  if ( this->field_0 )
  {
    sub_80A5060(this->field_0);
    operator delete(v1);
    this->field_0 = 0;
  }
  v2 = (void *)this->field_C;
  if ( v2 )
  {
    sub_814CCA0(this->field_C);
    operator delete(v2);
    this->field_C = 0;
  }
  if ( this->field_10 )
  {
    operator delete((void *)this->field_10);
    this->field_10 = 0;
  }
  v3 = (void *)this->field_14;
  if ( v3 )
  {
    sub_80C8A20(this->field_14);
    operator delete(v3);
    this->field_14 = 0;
  }
  v4 = (void *)this->field_18;
  if ( v4 )
  {
    sub_80AADA0(this->field_18);
    operator delete(v4);
    this->field_18 = 0;
  }
  v5 = (void *)this->field_1C;
  if ( v5 )
  {
    sub_814A2D0(this->field_1C);
    operator delete(v5);
    this->field_1C = 0;
  }
  if ( this->field_24 )
  {
    operator delete((void *)this->field_24);
    this->field_24 = 0;
  }
  v6 = (void *)this->field_20;
  if ( v6 )
  {
    sub_804C1E0(this->field_20);
    operator delete(v6);
    this->field_20 = 0;
  }
  v7 = (void *)this->field_28;
  if ( v7 )
  {
    sub_8094C00(this->field_28);
    operator delete(v7);
    this->field_28 = 0;
  }
  this->field_5A = 0;
  dword_81CA600 = 0;
  this->field_59 = 12;
  this->field_5D = 1;
  LODWORD(this->field_62) = 1006834287;
  this->field_66 = 0;
  this->field_6A = 0;
  this->gap78[0] = 0;
  this->field_5E = 2;
  sub_804CCB0(&v8);
*/
}

//----------------------------------------------------

void CNetGame::ReInitWhenRestarting()
{
	Init();

	// TODO: CNetGame::ReInitWhenRestarting()
}

//----------------------------------------------------

#ifdef WIN32

#pragma comment(lib, "winmm.lib")
float GetElapsedTime()
{
	static BOOL bTimerInit = false;
	static BOOL bUsingOPF  = false;
	static LONGLONG nTicksPerSec = 0;

	if (!bTimerInit)
	{
		bTimerInit = true;
		LARGE_INTEGER qwTicksPerSec;
		bUsingOPF = QueryPerformanceFrequency(&qwTicksPerSec);
		if (bUsingOPF) nTicksPerSec = qwTicksPerSec.QuadPart;
	}

	if (bUsingOPF)
	{
		LARGE_INTEGER qwTime;
		QueryPerformanceCounter(&qwTime);
		static LONGLONG llLastTime = qwTime.QuadPart;
		double fElapsedTime = (double)(qwTime.QuadPart - llLastTime) / (double) nTicksPerSec;
		llLastTime = qwTime.QuadPart;
		return (float)fElapsedTime;
	} else {
		double fTime = timeGetTime() * 0.001;
		static double fLastTime = fTime;
		double fElapsedTime = (double)(fTime - fLastTime);
		fLastTime = fTime;
		return (float)fElapsedTime;
	}
}

#else

float GetElapsedTime()
{
	static timeval lasttv;
	timeval tv;
	float fRet;

	gettimeofday(&tv, NULL);

	if (!timerisset(&lasttv)) memcpy(&lasttv, &tv, sizeof(timeval));

	fRet = (float)((tv.tv_sec - lasttv.tv_sec) * 1000000) + (tv.tv_usec - lasttv.tv_usec);
	fRet /= 1000000.0f;

	memcpy(&lasttv,&tv,sizeof(timeval));

	return fRet;
}

#endif // WIN32

//----------------------------------------------------

void CNetGame::MasterServerAnnounce(float fElapsedTime)
{
	static float fRemainingTime = 0.0f;
	fRemainingTime -= fElapsedTime;
	char szPort[32];

	if(fRemainingTime <= 0.0f)
	{
		fRemainingTime = 300.0f; // 300secs = 5mins.
		sprintf(szPort,"%d",pConsole->GetIntVariable("port"));

		CHAR *szBindAddress = pConsole->GetStringVariable("bind");
		if (szBindAddress && szBindAddress[0] == 0)
			szBindAddress = NULL;

#ifdef WIN32
		char szParams[256];
		sprintf(szParams, "%s", szPort);
		ShellExecute(0,"open","announce.exe",szParams,NULL,SW_HIDE);
#else
		char szCurrentDir[256];
		char szAnnounceCmd[256];
		getcwd(szCurrentDir,256);
		if(szBindAddress)
			sprintf(szAnnounceCmd,"%s/announce %s %s &",szCurrentDir,szPort,szBindAddress);
		else
			sprintf(szAnnounceCmd,"%s/announce %s &",szCurrentDir,szPort);
		//printf("Running announce. %s",szAnnounceCmd);
		system(szAnnounceCmd);
#endif

	}
}

//----------------------------------------------------

void CNetGame::UpdateTickCounter()
{
	if((GetTime() - m_iLastServerTickUpdate) >= 1000)
	{
		m_iLastServerTickUpdate = GetTime();
		m_iServerTickRate = m_iServerTickCount;
		m_iServerTickCount = 0;
	}
	m_iServerTickCount++;
}

//----------------------------------------------------

void CNetGame::Process()
{
	float fElapsedTime = GetElapsedTime();

	UpdateTickCounter();
	UpdateNetwork();

	if(m_iGameState == GAMESTATE_RUNNING)
	{
		if(m_pPlayerPool) m_pPlayerPool->Process(fElapsedTime);
		if(m_pVehiclePool) m_pVehiclePool->Process(fElapsedTime);
		if(m_pObjectPool) m_pObjectPool->Process(fElapsedTime);
		if(m_pGameMode) m_pGameMode->Frame(fElapsedTime);
		if(m_pScriptTimers) m_pScriptTimers->Process((DWORD)(fElapsedTime * 1000.0f));
		if(m_pScriptHttps) m_pScriptHttps->Process();
	}
	else if(m_iGameState == GAMESTATE_RESTARTING) 
	{
		fRestartWaitTime += fElapsedTime;
		if(fRestartWaitTime > 12.0f) // wait 12 seconds, then restart
		{
			ReInitWhenRestarting();
		}
	}

	if (pConsole->GetBoolVariable("announce")) {
		// Announce the server to the master
		MasterServerAnnounce(fElapsedTime);
	}

	// Execute the tick event for loaded plugins
	pPlugins->DoProcessTick();

	#ifndef WIN32
		m_dElapsedTime += (double)fElapsedTime;
	#endif
}

//----------------------------------------------------
// UPDATE NETWORK
//----------------------------------------------------

void CNetGame::UpdateNetwork()
{
	Packet* p;
	unsigned char packetIdentifier;

	while(p=m_pRak->Receive())
	{
		packetIdentifier = GetPacketID(p);

		switch(packetIdentifier) {

		case ID_NEW_INCOMING_CONNECTION_2:
			Packet_NewIncomingConnection(p);
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			Packet_DisconnectionNotification(p);
			break;
		case ID_CONNECTION_LOST:
			Packet_ConnectionLost(p);
			break;
		case ID_MODIFIED_PACKET:
			Packet_ModifiedPacket(p);
			break;
		/*
		// Not in latest RakNet, so not going to support it.
		case ID_REMOTE_PORT_REFUSED:
			Packet_RemotePortRefused(p);
			break;
		*/
		case ID_PLAYER_SYNC:
			Packet_PlayerSync(p);
			break;
		case ID_VEHICLE_SYNC:
			Packet_VehicleSync(p);
			break;
		case ID_PASSENGER_SYNC:
			Packet_PassengerSync(p);
			break;
		case ID_SPECTATOR_SYNC:
			Packet_SpectatorSync(p);
			break;
		case ID_WEAPON_SHOT_SYNC:
			Packet_WeaponShotSync(p);
			break;
		case ID_AIM_SYNC:
			Packet_AimSync(p);
			break;
		case ID_RCON_COMMAND:
			Packet_InGameRcon(p);
			break;
		case ID_STATS_UPDATE:
			Packet_StatsUpdate(p);
			break;
		case ID_WEAPONS_UPDATE:
			Packet_WeaponsUpdate(p);
			break;
		case ID_TRAILER_SYNC:
			Packet_TrailerSync(p);
			break;
		case ID_UNOCCUPIED_SYNC:
			Packet_UnoccupiedSync(p);
			break;
		}

		m_pRak->DeallocatePacket(p);
	}
}

//----------------------------------------------------

void CNetGame::BroadcastData(char *szUniqueID,
							 RakNet::BitStream *bitStream,
							 PLAYERID excludedPlayer,
							 char orderingStream)
{
	// TODO: CNetGame::BroadcastData
}

void CNetGame::SendToPlayer(char *szUniqueID,
							RakNet::BitStream *bitStream,
							PLAYERID playerId,
							char orderingChannel)
{
	CPlayerPool * pPlayerPool = GetPlayerPool();
	PacketReliability reliability;

	if(!pPlayerPool) return;

	reliability = RELIABLE_ORDERED;
	if(orderingChannel == 3)
      reliability = RELIABLE;

	if(pPlayerPool->GetSlotState(playerId))
	{
		m_pRak->RPC(szUniqueID, bitStream, HIGH_PRIORITY, reliability, orderingChannel,
			m_pRak->GetPlayerIDFromIndex(playerId), false, false);
	}

	// TODO: CNetGame::SendToPlayer
}


//----------------------------------------------------
// PACKET HANDLERS
//----------------------------------------------------

void CNetGame::Packet_PlayerSync(Packet *p)
{
	CPlayer * pPlayer = GetPlayerPool()->GetAt((PLAYERID)p->playerIndex);
	RakNet::BitStream bsPlayerSync((PCHAR)p->data, p->length, false);

	if(GetGameState() != GAMESTATE_RUNNING) return;

	BYTE				bytePacketID=0;
	ONFOOT_SYNC_DATA	ofSync;

	bsPlayerSync.Read(bytePacketID);
	bsPlayerSync.Read((PCHAR)&ofSync,sizeof(ONFOOT_SYNC_DATA));

	if(pPlayer)	{
		// TODO: CNetGame::Packet_PlayerSync
	}
}

//----------------------------------------------------

void CNetGame::Packet_AimSync(Packet *p)
{
	CPlayer * pPlayer = GetPlayerPool()->GetAt((PLAYERID)p->playerIndex);
	RakNet::BitStream bsPlayerSync((PCHAR)p->data, p->length, false);

	if(GetGameState() != GAMESTATE_RUNNING) return;

	BYTE			bytePacketID=0;
	AIM_SYNC_DATA	aimSync;

	bsPlayerSync.Read(bytePacketID);
	bsPlayerSync.Read((PCHAR)&aimSync,sizeof(AIM_SYNC_DATA));

	if(pPlayer)	{
		pPlayer->StoreAimSyncData(&aimSync);
	}
}

//----------------------------------------------------

void CNetGame::Packet_VehicleSync(Packet *p)
{
	CPlayer * pPlayer = GetPlayerPool()->GetAt((PLAYERID)p->playerIndex);
	RakNet::BitStream bsVehicleSync((PCHAR)p->data, p->length, false);

	if(GetGameState() != GAMESTATE_RUNNING) return;

	BYTE		bytePacketID=0;
	INCAR_SYNC_DATA icSync;

	bsVehicleSync.Read(bytePacketID);
	bsVehicleSync.Read((PCHAR)&icSync,sizeof(INCAR_SYNC_DATA));

	if(pPlayer)	{

	}

	// TODO: CNetGame::Packet_VehicleSync
}

//----------------------------------------------------

void CNetGame::Packet_PassengerSync(Packet *p)
{
	CPlayer * pPlayer = GetPlayerPool()->GetAt((PLAYERID)p->playerIndex);
	RakNet::BitStream bsPassengerSync((PCHAR)p->data, p->length, false);

	if(GetGameState() != GAMESTATE_RUNNING) return;

	BYTE bytePacketID=0;
	PASSENGER_SYNC_DATA psSync;

	bsPassengerSync.Read(bytePacketID);
	bsPassengerSync.Read((PCHAR)&psSync,sizeof(PASSENGER_SYNC_DATA));

	if(pPlayer)	{

	}

	// TODO: CNetGame::Packet_PassengerSync
}

//----------------------------------------------------

void CNetGame::Packet_SpectatorSync(Packet *p)
{
	CPlayer * pPlayer = GetPlayerPool()->GetAt((PLAYERID)p->playerIndex);
	RakNet::BitStream bsSpectatorSync((PCHAR)p->data, p->length, false);

	if(GetGameState() != GAMESTATE_RUNNING) return;

	// TODO: CNetGame::Packet_SpectatorSync
}

//----------------------------------------------------

void CNetGame::Packet_TrailerSync(Packet *p)
{
	CPlayer * pPlayer = GetPlayerPool()->GetAt((PLAYERID)p->playerIndex);
	RakNet::BitStream bsTrailerSync((PCHAR)p->data, p->length, false);

	if(GetGameState() != GAMESTATE_RUNNING) return;

	BYTE bytePacketID=0;
	TRAILER_SYNC_DATA trSync;

	bsTrailerSync.Read(bytePacketID);
	bsTrailerSync.Read((PCHAR)&trSync, sizeof(TRAILER_SYNC_DATA));

	/*if(pPlayer)	{
		if(unnamed_804B640(&trSync.field_1E) && unnamed_804B5D0(&trSync.field_2)) {
			pPlayer->StoreTrailerFullSyncData(&trSync);
		}
	}*/
	// TODO: CNetGame::Packet_TrailerSync
}

//----------------------------------------------------

void CNetGame::Packet_UnoccupiedSync(Packet *p)
{
	// TODO: CNetGame::Packet_UnoccupiedSync
}

//----------------------------------------------------

void CNetGame::Packet_StatsUpdate(Packet *p)
{
	RakNet::BitStream bsStats((PCHAR)p->data, p->length, false);
	CPlayerPool *pPlayerPool = GetPlayerPool();
	PLAYERID playerId = (PLAYERID)p->playerIndex;
	int iMoney;
	int iDrunkLevel;
	BYTE bytePacketID;

	bsStats.Read(bytePacketID);
	bsStats.Read(iMoney);
	bsStats.Read(iDrunkLevel);

	if(pPlayerPool) {
		if(pPlayerPool->GetSlotState(playerId)) {
			pPlayerPool->SetPlayerMoney(playerId,iMoney);
			pPlayerPool->SetPlayerDrunkLevel(playerId,iDrunkLevel);
		}
	}

	// TODO: CNetGame::Packet_StatsUpdate
}

//----------------------------------------------------

void CNetGame::Packet_WeaponsUpdate(Packet *p)
{
	// TODO: CNetGame::Packet_WeaponsUpdate
}

//----------------------------------------------------

void CNetGame::Packet_WeaponShotSync(Packet *p)
{
	if(GetGameState() != GAMESTATE_RUNNING) return;

	// TODO: CNetGame::Packet_WeaponShotSync
}








//----------------------------------------------------

void CNetGame::Packet_NewIncomingConnection(Packet* packet)
{
	if(!packet) return;

	logprintf("[connection] incoming connection: %s id: %u",
		packet->playerId.ToString(), packet->playerIndex);

	if(m_pGameMode)
	{
		if(m_pFilterScripts)
		{
			m_pFilterScripts->OnIncomingConnection(packet->playerIndex,
				packet->playerId.ToString(false), packet->playerId.port);

			m_pGameMode->OnIncomingConnection(packet->playerIndex,
				packet->playerId.ToString(false), packet->playerId.port);
		}
	}
}

//----------------------------------------------------

void CNetGame::Packet_DisconnectionNotification(Packet* packet)
{
	m_pPlayerPool->Delete((PLAYERID)packet->playerIndex,1);
}

//----------------------------------------------------

void CNetGame::Packet_ConnectionLost(Packet* packet)
{
	m_pPlayerPool->Delete((PLAYERID)packet->playerIndex,0);	
}

//----------------------------------------------------

void CNetGame::Packet_ModifiedPacket(Packet* packet)
{
	logprintf("Packet was modified, sent by id: %d, ip: %s",
		packet->playerIndex, packet->playerId.ToString());
}

//----------------------------------------------------

void CNetGame::Packet_RemotePortRefused(Packet* packet)
{
	logprintf("Remote Port Refused for Player: %u\n", packet->playerIndex);
}

//----------------------------------------------------

void CNetGame::Packet_InGameRcon(Packet* packet)
{
	// TODO: CNetGame::Packet_InGameRcon
}

//----------------------------------------------------

void CNetGame::SendClientMessageToAll(DWORD dwColor, char* szMessage, ...)
{
	va_list va;
	va_start(va, szMessage);
	char szBuffer[1024] = { 0 };
	vsprintf(szBuffer, szMessage, va);
	va_end(va);

	RakNet::BitStream bsParams;
	DWORD dwStrLen = strlen(szBuffer);

	bsParams.Write(dwColor);
	bsParams.Write(dwStrLen);
	bsParams.Write(szBuffer, dwStrLen);
	BroadcastData(RPC_ClientMessage, &bsParams, INVALID_PLAYER_ID, 3);
}

//----------------------------------------------------

void CNetGame::SetWorldTime(BYTE byteHour)
{
	RakNet::BitStream bsTime;

	m_byteWorldTime = byteHour;
	bsTime.Write(m_byteWorldTime);
	BroadcastData(RPC_WorldTime, &bsTime, INVALID_PLAYER_ID, 2);

	char szTime[256];
	sprintf(szTime, "%02d:%02d", m_byteWorldTime, 0);
	pConsole->SetStringVariable("worldtime", szTime);
}

//----------------------------------------------------

void CNetGame::SetWeather(BYTE byteWeather)
{
	RakNet::BitStream bsWeather;
	m_byteWeather = byteWeather;
	bsWeather.Write(m_byteWeather);
	BroadcastData(RPC_Weather, &bsWeather, INVALID_PLAYER_ID, 2);

	char szWeather[128];
	sprintf(szWeather, "%d", m_byteWeather);
	pConsole->SetStringVariable("weather", szWeather);
}

//----------------------------------------------------

void CNetGame::SetGravity(float fGravity)
{
	m_fGravity =		fGravity;
	RakNet::BitStream	bsGravity;
	bsGravity.Write(m_fGravity);

	char szGravity[128];
	sprintf(szGravity, "%f", m_fGravity);

	pConsole->SetStringVariable("gravity", szGravity);
	BroadcastData(RPC_ScrSetGravity, &bsGravity, INVALID_PLAYER_ID, 2);
}

//----------------------------------------------------

void CNetGame::KickPlayer(PLAYERID kickPlayer)
{
	if(kickPlayer >= MAX_PLAYERS) return;

	// TODO: CNetGame::KickPlayer
}

//----------------------------------------------------

void CNetGame::BlockIpAddress(char *ip_address, int timems)
{
	m_pRak->AddToBanList(ip_address, timems);
}

//----------------------------------------------------

void CNetGame::UnBlockIpAddress(char *ip_address)
{
	m_pRak->RemoveFromBanList(ip_address);
}

//----------------------------------------------------

void CNetGame::AddBan(char * nick, char * ip_mask, char * reason)
{
	const struct tm *tm;
	time_t now;
	now = time(NULL);
	tm = localtime(&now);
	char *s;
	s = new char[256];
	strftime(s, 256, "[%d/%m/%y | %H:%M:%S]", tm);

	m_pRak->AddToBanList(ip_mask);

	FILE * fileBanList = fopen("samp.ban","a");
	if(!fileBanList) return;

	fprintf(fileBanList,"%s %s %s - %s\n", ip_mask, s, nick, reason);
	fclose(fileBanList);

	delete [] s;
}

//----------------------------------------------------

void CNetGame::RemoveBan(char* ip_mask)
{
	m_pRak->RemoveFromBanList(ip_mask);

	FILE* fileBanList = fopen("samp.ban", "r");
	FILE* fileWriteList = fopen("samp.ban.temp", "w");
	if(!fileBanList || !fileWriteList) return;

	char line[256];
	char line2[256];
	char* ip;

	while(!feof(fileBanList))
	{
		if (fgets(line, 256, fileBanList))
		{
			strcpy(line2, line);
			ip = strtok(line, " \t");
			if (strcmp(ip_mask, ip) != 0)
			{
				fprintf(fileWriteList, "%s", line2);
			}
		}
	}
	fclose(fileBanList);
	fclose(fileWriteList);
	remove("samp.ban");
	rename("samp.ban.temp", "samp.ban");
}

//----------------------------------------------------

void CNetGame::LoadBanList()
{
	m_pRak->ClearBanList();

	FILE * fileBanList = fopen("samp.ban","r");

	if(!fileBanList) {
		return;
	}

	char tmpban_ip[256];

	while(!feof(fileBanList)) {
		if (fgets(tmpban_ip,256,fileBanList)) {
			int len = strlen(tmpban_ip);
			if (len > 0 && tmpban_ip[len - 1] == '\n')
				tmpban_ip[len - 1] = 0;
			len = strlen(tmpban_ip);
			if (len > 0 && tmpban_ip[len - 1] == '\r')
				tmpban_ip[len - 2] = 0;
			if (tmpban_ip[0] != 0 && tmpban_ip[0] != '#') {
				char *ban_ip = strtok(tmpban_ip, " \t");
				m_pRak->AddToBanList(ban_ip);
			}
		}
	}

	logprintf("");
	logprintf("Ban list");
	logprintf("--------");
	logprintf(" Loaded: samp.ban");
	logprintf("");

	fclose(fileBanList);
}

//----------------------------------------------------

int CNetGame::AddSpawn(PLAYER_SPAWN_INFO *pSpawnInfo)
{
	if (m_iSpawnsAvailable < MAX_SPAWNS)
	{
		memcpy(&m_AvailableSpawns[m_iSpawnsAvailable],pSpawnInfo,sizeof(PLAYER_SPAWN_INFO));
		return m_iSpawnsAvailable++;
	}
	return MAX_SPAWNS;
}

//----------------------------------------------------

const PCHAR CNetGame::GetWeaponName(int iWeaponID)
{
	switch(iWeaponID) {
      case WEAPON_BRASSKNUCKLE:
         return "Brass Knuckles";
      case WEAPON_GOLFCLUB:
         return "Golf Club";
      case WEAPON_NITESTICK:
         return "Nite Stick";
      case WEAPON_KNIFE:
         return "Knife";
      case WEAPON_BAT:
         return "Baseball Bat";
      case WEAPON_SHOVEL:
         return "Shovel";
      case WEAPON_POOLSTICK:
         return "Pool Cue";
      case WEAPON_KATANA:
         return "Katana";
      case WEAPON_CHAINSAW:
         return "Chainsaw";
      case WEAPON_DILDO:
         return "Dildo";
      case WEAPON_DILDO2:
         return "Dildo";
      case WEAPON_VIBRATOR:
         return "Vibrator";
      case WEAPON_VIBRATOR2:
         return "Vibrator";
      case WEAPON_FLOWER:
         return "Flowers";
      case WEAPON_CANE:
         return "Cane";
      case WEAPON_GRENADE:
         return "Grenade";
      case WEAPON_TEARGAS:
         return "Teargas";
      case WEAPON_MOLTOV:
         return "Molotov Cocktail";
      case WEAPON_COLT45:
         return "Colt 45";
      case WEAPON_SILENCED:
         return "Silenced Pistol";
      case WEAPON_DEAGLE:
         return "Desert Eagle";
      case WEAPON_SHOTGUN:
         return "Shotgun";
      case WEAPON_SAWEDOFF:
         return "Sawn-off Shotgun";
      case WEAPON_SHOTGSPA: // wtf?
         return "Combat Shotgun";
      case WEAPON_UZI:
         return "UZI";
      case WEAPON_MP5:
         return "MP5";
      case WEAPON_AK47:
         return "AK47";
      case WEAPON_M4:
         return "M4";
      case WEAPON_TEC9:
         return "TEC9";
      case WEAPON_RIFLE:
         return "Rifle";
      case WEAPON_SNIPER:
         return "Sniper Rifle";
      case WEAPON_ROCKETLAUNCHER:
         return "Rocket Launcher";
      case WEAPON_HEATSEEKER:
         return "Heat Seaker";
      case WEAPON_FLAMETHROWER:
         return "Flamethrower";
      case WEAPON_MINIGUN:
         return "Minigun";
      case WEAPON_SATCHEL:
         return "Satchel Explosives";
      case WEAPON_BOMB:
         return "Bomb";
      case WEAPON_SPRAYCAN:
         return "Spray Can";
      case WEAPON_FIREEXTINGUISHER:
         return "Fire Extinguisher";
      case WEAPON_CAMERA:
         return "Camera";
      case WEAPON_NIGHTVISION:
         return "Night Vision";
      case WEAPON_INFRARED:
         return "Thermal Goggles";
      case WEAPON_PARACHUTE:
         return "Parachute";
      case WEAPON_VEHICLE:
         return "Vehicle";
      case WEAPON_DROWN:
         return "Drowned";
      case WEAPON_COLLISION:
         return "Splat";
	}

	return "";
}

//----------------------------------------------------
// EOF
