
#include "../main.h"
#include "../mod.h"
#include "../buildinfo.h"
#include "../../raknet/SocketDataEncryptor.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CConfig       *pConfig;
extern CAudioStream  *pAudioStream;

void FUNC_100AABE0(); // patches.cpp

#define NETGAME_VERSION 4057

//----------------------------------------------------

BYTE __stdcall GetPacketID(Packet *p)
{
	if (p==0) return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP) {
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else {
		return (unsigned char) p->data[0];
	}
}

//----------------------------------------------------

bool __stdcall HasTimestamps(Packet *p)
{
	if (p==0) return 0;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		return true;
	return false;
}

//----------------------------------------------------

int CNetGame::GetTime()
{
	return (int)RakNet::GetTime();
}

//----------------------------------------------------

CNetGame::CNetGame(PCHAR szHostOrIp, int iPort,
				   PCHAR szPlayerName, PCHAR szPass)
{
	field_3D5 = (struc_41 *)calloc(1,sizeof(struc_41));

	memset(m_szHostName,0,256);
	memset(m_szHostOrIp,0,256);

	strcpy(m_szHostName, "SA-MP");
	strncpy(m_szHostOrIp, szHostOrIp, sizeof(m_szHostOrIp));
	m_iPort = iPort;

	m_pRakClient = RakNetworkFactory::GetRakClientInterface();

	InitPools();

	m_pPools->pPlayerPool->SetLocalPlayerName(szPlayerName);

	SocketDataEncryptor::SetKey(iPort);

	RegisterRPCs(m_pRakClient);
	RegisterScriptRPCs(m_pRakClient);	// Register server-side scripting RPCs.

	m_pRakClient->SetPassword(szPass);

	int i;
	for (i = 0; i < 100; i++) m_dwMapIcon[i] = NULL;

	pGame->EnableClock(0); // Hide the clock by default
	pGame->EnableZoneNames(0);
	field_3D5->iDeathDropMoney = 0;
	field_3D5->iSpawnsAvailable = 0;
	field_3D5->bNameTagLOS = true;
	field_3D5->byteHoldTime = 1;
	field_3D5->bUseCJWalk = false;
	field_3D5->bDisableEnterExits = false;
	field_3D5->bZoneNames = false;
	field_3D5->field_20 = 0;
	field_3D5->fNameTagDrawDistance = 60.0f;
	field_3D5->field_22 = 1;
	field_3D5->byteWorldTime = 12;
	field_3D5->byteWorldMinute = 0;
	field_3D5->byteWeather = 1;
	field_3D5->fGravity = (float)0.008000000;

	m_bPlayerNameTagStatus = true;
	if(pConfig && pConfig->GetIntVariable("nonametagstatus"))
		m_bPlayerNameTagStatus = false;

	field_232 = 0;
	field_233 = 0;
	m_dwLastConnectAttempt = GetTickCount();
	m_iGameState = GAMESTATE_WAIT_CONNECT;
	field_239 = 0;
	field_3D9 = 0;

	PrintWelcomeMessage();
}

//----------------------------------------------------

CNetGame::~CNetGame()
{
	m_pRakClient->Disconnect(0);
	UnRegisterRPCs(m_pRakClient);
	UnRegisterScriptRPCs(m_pRakClient);	// Unregister server-side scripting RPCs.
	RakNetworkFactory::DestroyRakClientInterface(m_pRakClient);
	SAFE_DELETE(m_pPools->pPlayerPool);
	SAFE_DELETE(m_pPools->pVehiclePool);
	SAFE_DELETE(m_pPools->pActorPool);
	SAFE_DELETE(m_pPools->pTextDrawPool);
	SAFE_DELETE(m_pPools->pGangZonePool);
	SAFE_DELETE(m_pPools->pLabelPool);
	SAFE_DELETE(m_pPools->pPickupPool);
	SAFE_DELETE(m_pPools->pObjectPool);
	SAFE_DELETE(m_pPools->pMenuPool);
	SAFE_DELETE(m_pPools);
	SAFE_DELETE(field_3D5);
}

//----------------------------------------------------

void CNetGame::InitPools()
{
	m_pPools = (NETGAME_POOLS *)calloc(1,sizeof(NETGAME_POOLS));

	m_pPools->pObjectPool	= new CObjectPool();
	m_pPools->pMenuPool		= new CMenuPool();
	m_pPools->pLabelPool	= new CLabelPool();
	m_pPools->pTextDrawPool	= new CTextDrawPool();
	m_pPools->pGangZonePool	= new CGangZonePool();
	m_pPools->pPlayerPool	= new CPlayerPool();
	m_pPools->pVehiclePool	= new CVehiclePool();
	m_pPools->pPickupPool	= new CPickupPool();
	m_pPools->pActorPool	= new CActorPool();
}

//----------------------------------------------------

void CNetGame::ResetAllPools()
{
	ResetVehiclePool();
	ResetActorPool();
	ResetTextDrawPool();
	ResetGangZonePool();
	ResetLabelPool();
	ResetMapIcons();
	ResetPickupPool();
	ResetObjectPool();
	ResetMenuPool();
}

//----------------------------------------------------

void CNetGame::PrintWelcomeMessage()
{
	if(pChatWindow) pChatWindow->AddDebugMessage("{FFFFFF}SA-MP {B9C9BF}" SAMP_VERSION " {FFFFFF}Started");
}

//----------------------------------------------------

void CNetGame::HandleConnectingState()
{
	if((GetTickCount() - m_dwLastConnectAttempt) > 3000)
	{
		if(pChatWindow) pChatWindow->AddDebugMessage("Connecting to %s:%d...",m_szHostOrIp,m_iPort);
		m_pRakClient->Connect(m_szHostOrIp,m_iPort,0,0,2);
		m_dwLastConnectAttempt = GetTickCount();
		SetGameState(GAMESTATE_CONNECTING);
	}
}

//----------------------------------------------------

void CNetGame::ShowWaitingScreen()
{
	if(pGame->FindPlayerPed()->IsInVehicle()) {
		pGame->FindPlayerPed()->RemoveFromVehicleAndPutAt(1093.4f,-2036.5f,82.710602f);
	} else {
		pGame->FindPlayerPed()->TeleportTo(1133.0504f,-2038.4034f,69.1f);
	}
	pGame->GetCamera()->SetPosition(1093.0f,-2036.0f,90.0f,0.0f,0.0f,0.0f);
	pGame->GetCamera()->LookAtPoint(384.0f,-1557.0f,20.0f,2);
	pGame->SetWorldWeather(1);
	pGame->DisplayHud(FALSE);
}

//----------------------------------------------------

void CNetGame::InitGameLogic()
{
	if(field_3D5->bManualVehicleEngineAndLights)
		FUNC_100AABE0();

	field_3D5->WorldBounds[0] = 20000.0f;
	field_3D5->WorldBounds[1] = -20000.0f;
	field_3D5->WorldBounds[2] = 20000.0f;
	field_3D5->WorldBounds[3] = -20000.0f;
}

//----------------------------------------------------

float GetElapsedTime();

void CNetGame::Process()
{
	float fElapsedTime = GetElapsedTime();

	UpdateNetwork();

	if (field_3D5->byteHoldTime)	{
		pGame->SetWorldTime(field_3D5->byteWorldTime, field_3D5->byteWorldMinute);
	}

	// KEEP THE ANIMS LOADED DURING THE NETGAME
	pGame->RequestAnimationsAndModels();

	if(GetGameState() == GAMESTATE_CONNECTED) {

	}
	else {
		ShowWaitingScreen();
	}

	if(GetGameState() == GAMESTATE_WAIT_CONNECT)
		HandleConnectingState();

	// TODO: CNetGame::Process()
}

//----------------------------------------------------
// UPDATE NETWORK
//----------------------------------------------------

void CNetGame::UpdateNetwork()
{
	Packet* pkt=NULL;
	unsigned char packetIdentifier;

	while((pkt = m_pRakClient->Receive()))
	{
		packetIdentifier = GetPacketID(pkt);

		switch(packetIdentifier)
		{
		case ID_PLAYER_SYNC:
			Packet_PlayerSync(pkt);
			break;
		case ID_VEHICLE_SYNC:
			Packet_VehicleSync(pkt);
			break;
		case ID_PASSENGER_SYNC:
			Packet_PassengerSync(pkt);
			break;
		case ID_WEAPON_SHOT_SYNC:
			Packet_WeaponShotSync(pkt);
			break;
		case ID_AIM_SYNC:
			Packet_AimSync(pkt);
			break;
		case ID_TRAILER_SYNC:
			Packet_TrailerSync(pkt);
			break;
		case ID_UNOCCUPIED_SYNC:
			Packet_UnoccupiedSync(pkt);
			break;
		case 208:
			FUNC_10009F10(pkt);
			break;
		case 12:
			FUNC_1000A100(pkt);
			break;
		case ID_CONNECTION_BANNED:
			Packet_ConnectionBanned(pkt);
			break;
		case ID_RSA_PUBLIC_KEY_MISMATCH:
			Packet_RSAPublicKeyMismatch(pkt);
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			Packet_NoFreeIncomingConnections(pkt);
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			Packet_DisconnectionNotification(pkt);
			break;
		case ID_CONNECTION_LOST:
			Packet_ConnectionLost(pkt);
			break;
		case ID_INVALID_PASSWORD:
			Packet_InvalidPassword(pkt);
			break;
		case ID_MODIFIED_PACKET:
			Packet_ModifiedPacket(pkt);
			break;
		case ID_CONNECTION_ATTEMPT_FAILED: 
			Packet_ConnectAttemptFailed(pkt); 
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			Packet_ConnectionSucceeded(pkt);
			break;
		}

		m_pRakClient->DeallocatePacket(pkt);
	}
}

//----------------------------------------------------
// PACKET HANDLERS INTERNAL
//----------------------------------------------------

void CNetGame::Packet_PlayerSync(Packet *p)
{
	// TODO: CNetGame::Packet_PlayerSync
}

//----------------------------------------------------

void CNetGame::Packet_VehicleSync(Packet *p)
{
	// TODO: CNetGame::Packet_VehicleSync
}

//----------------------------------------------------

void CNetGame::Packet_PassengerSync(Packet *p)
{
	// TODO: CNetGame::Packet_PassengerSync
}

//----------------------------------------------------

void CNetGame::Packet_WeaponShotSync(Packet *p)
{
	// TODO: CNetGame::Packet_WeaponShotSync
}

//----------------------------------------------------

void CNetGame::Packet_AimSync(Packet *p)
{
	// TODO: CNetGame::Packet_AimSync
}

//----------------------------------------------------

void CNetGame::Packet_TrailerSync(Packet *p)
{
	// TODO: CNetGame::Packet_TrailerSync
}

//----------------------------------------------------

void CNetGame::Packet_UnoccupiedSync(Packet *p)
{
	// TODO: CNetGame::Packet_UnoccupiedSync
}

//----------------------------------------------------

void CNetGame::FUNC_10009F10(Packet *p)
{
	// TODO: CNetGame::FUNC_10009F10
}

//----------------------------------------------------

void CNetGame::FUNC_1000A100(Packet *p)
{
	// TODO: CNetGame::FUNC_1000A100
}

//----------------------------------------------------

void CNetGame::Packet_RSAPublicKeyMismatch(Packet* packet)
{
	pChatWindow->AddDebugMessage("Failed to initialize encryption.");
}

//----------------------------------------------------

void CNetGame::Packet_ConnectionBanned(Packet* packet)
{
	pChatWindow->AddDebugMessage("You are banned from this server.");
}

//----------------------------------------------------

void CNetGame::Packet_ConnectionRequestAccepted(Packet* packet)
{
	pChatWindow->AddDebugMessage("Server has accepted the connection.");
}

//----------------------------------------------------

void CNetGame::Packet_NoFreeIncomingConnections(Packet* packet)
{
	pChatWindow->AddDebugMessage("The server is full. Retrying...");
	SetGameState(GAMESTATE_WAIT_CONNECT);
}

//----------------------------------------------------

void CNetGame::Packet_DisconnectionNotification(Packet* packet)
{
	pChatWindow->AddDebugMessage("Server closed the connection.");
	if(pAudioStream)
		pAudioStream->Stop();
	m_pRakClient->Disconnect(2000);
}

//----------------------------------------------------

void CNetGame::Packet_ConnectionLost(Packet* packet)
{
	// TODO: CNetGame::Packet_ConnectionLost
}

//----------------------------------------------------

void CNetGame::Packet_InvalidPassword(Packet* packet)
{
	pChatWindow->AddDebugMessage("Wrong server password.");
	m_pRakClient->Disconnect(0);
}

//----------------------------------------------------

void CNetGame::Packet_ModifiedPacket(Packet* packet)
{
#ifdef _DEBUG
	char szBuffer[256];
	sprintf(szBuffer, "Packet was modified, sent by id: %d, ip: %s",
					(unsigned int)packet->playerIndex, packet->playerId.ToString());
	pChatWindow->AddDebugMessage(szBuffer);
	//m_pRakClient->Disconnect(0);
#endif
}

//----------------------------------------------------
// RST

void CNetGame::Packet_ConnectAttemptFailed(Packet* packet)
{
	pChatWindow->AddDebugMessage("The server didn't respond. Retrying..");
	SetGameState(GAMESTATE_WAIT_CONNECT);
}

//----------------------------------------------------
// Connection Success

void CNetGame::Packet_ConnectionSucceeded(Packet *p)
{
	
	RakNet::BitStream bsReturnParams((PCHAR)p->data, p->length, true);

	BYTE bytePacketID=0;
	unsigned int binaryAddr=0;
	unsigned short port=0;
	unsigned short playerId=0;
	unsigned int uiChallenge=0;

	bsReturnParams.Read(bytePacketID);
	bsReturnParams.Read(binaryAddr);
	bsReturnParams.Read(port);
	bsReturnParams.Read(playerId);
	bsReturnParams.Read(uiChallenge);

	uiChallenge ^= NETGAME_VERSION;

	if(pChatWindow) {
		pChatWindow->AddDebugMessage("Connected. Joining the game...");
	}

	m_iGameState = GAMESTATE_AWAIT_JOIN;

	int iVersion = NETGAME_VERSION;
	BYTE byteMod = MOD_VERSION;
	BYTE byteNameLen = (BYTE)strlen(GetPlayerPool()->GetLocalPlayerName());

	RakNet::BitStream bsSend;
	bsSend.Write(iVersion);
	bsSend.Write(byteMod);
	bsSend.Write(byteNameLen);
	bsSend.Write(GetPlayerPool()->GetLocalPlayerName(),byteNameLen);
	bsSend.Write(uiChallenge);

	bsSend.Write((BYTE)strlen(SAMP_VERSION));
	bsSend.Write(SAMP_VERSION,strlen(SAMP_VERSION));
	bsSend.Write(uiChallenge);
	m_pRakClient->RPC(&RPC_ClientJoin,&bsSend,HIGH_PRIORITY,RELIABLE,0,false);
}

//----------------------------------------------------

void CNetGame::TogglePlayerTagStatus()
{
	if(!m_bPlayerNameTagStatus)
	{
		pChatWindow->AddInfoMessage("NameTag Player Status: ON");
		if(pConfig)
			pConfig->SetIntVariable("nonametagstatus", 0);
		m_bPlayerNameTagStatus = true;
	}
	else
	{
		pChatWindow->AddInfoMessage("NameTag Player Status: OFF");
		if(pConfig)
			pConfig->SetIntVariable("nonametagstatus", 1);
		m_bPlayerNameTagStatus = false;
	}
}

//----------------------------------------------------

void CNetGame::UpdatePlayerScoresAndPings()
{
	static DWORD dwLastUpdateTick = 0;

	if ((GetTickCount() - dwLastUpdateTick) > 3000) {
		dwLastUpdateTick = GetTickCount();
		RakNet::BitStream bsParams;
		if(m_pRakClient)
			m_pRakClient->RPC(&RPC_UpdateScoresPingsIPs, &bsParams, HIGH_PRIORITY, RELIABLE,0,false);
	}
}

//----------------------------------------------------

void CNetGame::ResetVehiclePool()
{
	if(m_pPools->pVehiclePool) {
		delete m_pPools->pVehiclePool;
	}
	m_pPools->pVehiclePool = new CVehiclePool();
}

//----------------------------------------------------

void CNetGame::ResetPlayerPool()
{
	if(m_pPools->pPlayerPool) {
		delete m_pPools->pPlayerPool;
	}
	m_pPools->pPlayerPool = new CPlayerPool();
}

//----------------------------------------------------

void CNetGame::ResetPickupPool()
{
	if(m_pPools->pPickupPool) {
		delete m_pPools->pPickupPool;
	}
	m_pPools->pPickupPool = new CPickupPool();
}

//----------------------------------------------------

void CNetGame::ResetMenuPool()
{
	if(m_pPools->pMenuPool) {
		delete m_pPools->pMenuPool;
	}
	m_pPools->pMenuPool = new CMenuPool();
}

//----------------------------------------------------

void CNetGame::ResetLabelPool()
{
	if(m_pPools->pLabelPool) {
		delete m_pPools->pLabelPool;
	}
	m_pPools->pLabelPool = new CLabelPool();
}

//----------------------------------------------------

void CNetGame::ResetTextDrawPool()
{
	if(m_pPools->pTextDrawPool) {
		delete m_pPools->pTextDrawPool;
	}
	m_pPools->pTextDrawPool = new CTextDrawPool();
}

//----------------------------------------------------

void CNetGame::ResetObjectPool()
{
	if(m_pPools->pObjectPool) {
		delete m_pPools->pObjectPool;
	}
	m_pPools->pObjectPool = new CObjectPool();
}

//----------------------------------------------------

void CNetGame::ResetGangZonePool()
{
	if(m_pPools->pGangZonePool) {
		delete m_pPools->pGangZonePool;
	}
	m_pPools->pGangZonePool = new CGangZonePool();
}

//-----------------------------------------------------------

void CNetGame::ResetActorPool()
{
	if (m_pPools->pActorPool) {
		delete m_pPools->pActorPool;
	}
	m_pPools->pActorPool = new CActorPool();
}

//-----------------------------------------------------------
// Puts a personal marker using any of the radar icons on the map

void CNetGame::SetMapIcon(BYTE byteIndex, float fX, float fY, float fZ, BYTE byteIcon, DWORD dwColor, int iStyle)
{
	if (byteIndex >= 100) return;
	if (m_dwMapIcon[byteIndex] != NULL) DisableMapIcon(byteIndex);
	//ScriptCommand(&create_radar_marker_without_sphere, fX, fY, fZ, byteIcon, &m_dwMapIcon);
	m_dwMapIcon[byteIndex] = pGame->CreateRadarMarkerIcon(byteIcon, fX, fY, fZ, dwColor, iStyle);
}

//-----------------------------------------------------------
// Removes the Map Icon

void CNetGame::DisableMapIcon(BYTE byteIndex)
{
	if (byteIndex >= 100) return;
	ScriptCommand(&disable_marker, m_dwMapIcon[byteIndex]);
	m_dwMapIcon[byteIndex] = NULL;
}

//----------------------------------------------------

void CNetGame::ResetMapIcons()
{
	BYTE i;
	for (i = 0; i < 100; i++)
	{
		if (m_dwMapIcon[i] != NULL) DisableMapIcon(i);
	}
}

//----------------------------------------------------
