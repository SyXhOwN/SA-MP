
#include "../main.h"

using namespace RakNet;
extern CNetGame*	pNetGame;
extern ONFOOT_SYNC_DATA ofSync;
extern BYTE byteMySeatID;

//----------------------------------------------------

void ScrSetSpawnInfo(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;
	PLAYER_SPAWN_INFO SpawnInfo;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	bsData.Read((PCHAR)&SpawnInfo, sizeof(PLAYER_SPAWN_INFO));
}

//----------------------------------------------------

void ScrUnk45(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrSetPlayerName(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk99(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrSetPlayerPos(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	
	bsData.Read(ofSync.vecPos.X);
	bsData.Read(ofSync.vecPos.Y);
	bsData.Read(ofSync.vecPos.Z);
}

//----------------------------------------------------

void ScrUnk0D(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk0E(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

VEHICLEID MyVehicleID = INVALID_VEHICLE_ID;

void ScrPutPlayerInVehicle(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	VEHICLEID vehicleid;
	BYTE seatid;

	bsData.Read(vehicleid);
	bsData.Read(seatid);

	MyVehicleID = vehicleid;
	byteMySeatID = seatid;

	if(pNetGame->GetBotMode()) {
		pNetGame->GetBotMode()->OnNPCEnterVehicle(vehicleid, seatid);
	}

	//logprintf("NPC: Entering vehicle %u", vehicleid);
}

//----------------------------------------------------

void ScrRemovePlayerFromVehicle(RPCParameters *rpcParams)
{
	MyVehicleID = INVALID_VEHICLE_ID;
	byteMySeatID = 0;

	if(pNetGame->GetBotMode()) {
		pNetGame->GetBotMode()->OnNPCExitVehicle();
	}
}

//----------------------------------------------------

void ScrSetPlayerColor(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrDisplayGameText(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	char szMessage[512];
	int iType;
	int iTime;
	int iLength;

	bsData.Read(iType);
	bsData.Read(iTime);
	bsData.Read(iLength);

	if(iLength > 512) return;

	bsData.Read(szMessage,iLength);
	szMessage[iLength] = '\0';

	//OutputDebugString(szMessage);
}

//----------------------------------------------------

void ScrSetInterior(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrSetCameraPos(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrSetCameraLookAt(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk9F(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnkA0(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnkA1(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk41(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrSetCameraBehindPlayer(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk0F(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk10(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk11(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk12(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrSetPlayerFacingAngle(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	float fAngle;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(fAngle);
	pNetGame->SetMyZAngle(fAngle);
}

//----------------------------------------------------

void ScrUnk14(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk15(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk16(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk40(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk37(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk38(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk90(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk42(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk91(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrSetGravity(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk93(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk94(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk95(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk2C(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk2D(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk2E(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk2F(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk30(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk31(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk4F(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk50(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk63(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk7A(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk7B(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk7C(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk7D(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk7E(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk7F(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk39(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk4A(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk4B(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrInitMenu(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrShowMenu(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrHideMenu(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk85(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk86(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk87(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrAddGangZone(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrRemoveGangZone(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrFlashGangZone(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrStopFlashGangZone(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk56(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk57(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void ScrUnk58(RPCParameters *rpcParams)
{
}

//----------------------------------------------------

void RegisterScriptRPCs(RakClientInterface* pRakClient)
{
	REGISTER_STATIC_RPC(pRakClient, ScrSetSpawnInfo);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk45);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk99);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerName);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerPos);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk0D);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk0E);
	REGISTER_STATIC_RPC(pRakClient, ScrPutPlayerInVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrRemovePlayerFromVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerColor);
	REGISTER_STATIC_RPC(pRakClient, ScrDisplayGameText);
	REGISTER_STATIC_RPC(pRakClient, ScrSetInterior);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraPos);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraLookAt);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk9F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA0);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA1);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraBehindPlayer);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk0F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk10);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk11);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk12);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerFacingAngle);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk14);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk15);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk16);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk40);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk41);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk42);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk37);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk38);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk90);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk91);
	REGISTER_STATIC_RPC(pRakClient, ScrSetGravity);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk93);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk94);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk95);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2C);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2D);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2E);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk30);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk31);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk4F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk50);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk63);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7A);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7C);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7D);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7E);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk39);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk4A);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk4B);
	REGISTER_STATIC_RPC(pRakClient, ScrInitMenu);
	REGISTER_STATIC_RPC(pRakClient, ScrShowMenu);
	REGISTER_STATIC_RPC(pRakClient, ScrHideMenu);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk85);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk86);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk87);
	REGISTER_STATIC_RPC(pRakClient, ScrAddGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrRemoveGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrFlashGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrStopFlashGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk56);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk57);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk58);
}

//----------------------------------------------------

void UnRegisterScriptRPCs(RakClientInterface* pRakClient)
{
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetSpawnInfo);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk45);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerName);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk99);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerPos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk0D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk0E);
	UNREGISTER_STATIC_RPC(pRakClient, ScrPutPlayerInVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemovePlayerFromVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerColor);
	UNREGISTER_STATIC_RPC(pRakClient, ScrDisplayGameText);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetInterior);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraPos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraLookAt);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk9F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA0);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA1);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraBehindPlayer);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk0F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk10);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk11);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk12);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerFacingAngle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk14);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk15);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk16);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk40);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk41);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk37);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk38);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk90);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk91);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetGravity);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk93);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk94);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk95);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2C);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2E);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk30);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk31);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk4F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk50);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk63);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7A);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7C);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7E);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk39);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk4A);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk4B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrInitMenu);
	UNREGISTER_STATIC_RPC(pRakClient, ScrShowMenu);
	UNREGISTER_STATIC_RPC(pRakClient, ScrHideMenu);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk85);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk86);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk87);
	UNREGISTER_STATIC_RPC(pRakClient, ScrAddGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemoveGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrFlashGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrStopFlashGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk56);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk57);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk58);
}

//----------------------------------------------------
