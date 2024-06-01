
#include "../main.h"
using namespace RakNet;
extern CNetGame* pNetGame;

#define REJECT_REASON_BAD_VERSION   1
#define REJECT_REASON_BAD_NICKNAME  2
#define REJECT_REASON_BAD_MOD		3

void EnterVehicle(RPCParameters *rpcParams) {}
void ExitVehicle(RPCParameters *rpcParams) {}
void Unk6B(RPCParameters *rpcParams) {}
void Unk25(RPCParameters *rpcParams) {}
void Unk26(RPCParameters *rpcParams) {}
void Unk27(RPCParameters *rpcParams) {}
void UpdateScoresPingsIPs(RPCParameters *rpcParams) {}

RakNetStatisticsStruct RakServerStats;

void SvrStats(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read((char *)&RakServerStats,sizeof(RakNetStatisticsStruct));
}

void GameModeRestart(RPCParameters *rpcParams)
{
	pNetGame->ShutdownForGameModeRestart();
}

void ConnectionRejected(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	BYTE byteRejectReason;

	bsData.Read(byteRejectReason);

	if(byteRejectReason==REJECT_REASON_BAD_VERSION) {
		//logprintf("BOT: CONNECTION REJECTED. INCORRECT SA-MP VERSION!");
	} 
	else if(byteRejectReason==REJECT_REASON_BAD_NICKNAME) {
		//logprintf("BOT: CONNECTION REJECTED. BAD NICKNAME!");
	}

	pNetGame->GetRakClient()->Disconnect(500);
	exit(1);
}

void ClientMessage(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	DWORD dwStrLen;
	DWORD dwColor;

	bsData.Read(dwColor);
	bsData.Read(dwStrLen);
	unsigned char* szMsg = (unsigned char *)malloc(dwStrLen+1);
	bsData.Read((char *)szMsg, dwStrLen);
	szMsg[dwStrLen] = 0;

	if(pNetGame->GetBotMode()) pNetGame->GetBotMode()->OnClientMessage(dwColor, szMsg);

	free(szMsg);
}

void WorldTime(RPCParameters *rpcParams) {}
void Unk5F(RPCParameters *rpcParams) {}
void Unk3F(RPCParameters *rpcParams) {}
void Unk97(RPCParameters *rpcParams) {}
void ScmEvent(RPCParameters *rpcParams) {}
void Weather(RPCParameters *rpcParams) {}
void Unk1D(RPCParameters *rpcParams) {}
void Unk1E(RPCParameters *rpcParams) {}

void Unk89(RPCParameters *rpcParams)
{
	// TODO: Unk89
}

void Unk8A(RPCParameters *rpcParams)
{
	// TODO: Unk8A
}

void Unk8B(RPCParameters *rpcParams)
{
	// TODO: Unk8B
}

void Chat(RPCParameters *rpcParams)
{
	// TODO: Chat
}

void RequestClass(RPCParameters *rpcParams)
{
	// TODO: RequestClass
}

void RequestSpawn(RPCParameters *rpcParams)
{
	// TODO: RequestSpawn
}

void Unk20(RPCParameters *rpcParams)
{
	// TODO: Unk20
}

void UnkA6(RPCParameters *rpcParams)
{
	// TODO: UnkA6
}

void UnkA3(RPCParameters *rpcParams)
{
	// TODO: UnkA3
}

void UnkA4(RPCParameters *rpcParams)
{
	// TODO: UnkA4
}

void UnkA5(RPCParameters *rpcParams)
{
	// TODO: UnkA5
}

//----------------------------------------------------

void RegisterRPCs(RakClientInterface * pRakClient)
{
	REGISTER_STATIC_RPC(pRakClient,EnterVehicle);
	REGISTER_STATIC_RPC(pRakClient,ExitVehicle);
	REGISTER_STATIC_RPC(pRakClient,Unk6B);
	REGISTER_STATIC_RPC(pRakClient,Unk25);
	REGISTER_STATIC_RPC(pRakClient,Unk26);
	REGISTER_STATIC_RPC(pRakClient,Unk27);
	REGISTER_STATIC_RPC(pRakClient,UpdateScoresPingsIPs);
	REGISTER_STATIC_RPC(pRakClient,SvrStats);
	REGISTER_STATIC_RPC(pRakClient,GameModeRestart);
	REGISTER_STATIC_RPC(pRakClient,ConnectionRejected);
	REGISTER_STATIC_RPC(pRakClient,ClientMessage);
	REGISTER_STATIC_RPC(pRakClient,WorldTime);
	REGISTER_STATIC_RPC(pRakClient,Unk5F);
	REGISTER_STATIC_RPC(pRakClient,Unk3F);
	REGISTER_STATIC_RPC(pRakClient,Unk97);
	REGISTER_STATIC_RPC(pRakClient,ScmEvent);
	REGISTER_STATIC_RPC(pRakClient,Weather);
	REGISTER_STATIC_RPC(pRakClient,Unk1D);
	REGISTER_STATIC_RPC(pRakClient,Unk1E);
	REGISTER_STATIC_RPC(pRakClient,Unk89);
	REGISTER_STATIC_RPC(pRakClient,Unk8A);
	REGISTER_STATIC_RPC(pRakClient,Unk8B);
	REGISTER_STATIC_RPC(pRakClient,Chat);
	REGISTER_STATIC_RPC(pRakClient,RequestClass);
	REGISTER_STATIC_RPC(pRakClient,RequestSpawn);
	REGISTER_STATIC_RPC(pRakClient,Unk20);
	REGISTER_STATIC_RPC(pRakClient,UnkA6);
	REGISTER_STATIC_RPC(pRakClient,UnkA3);
	REGISTER_STATIC_RPC(pRakClient,UnkA4);
	REGISTER_STATIC_RPC(pRakClient,UnkA5);
}

//----------------------------------------------------

void UnRegisterRPCs(RakClientInterface * pRakClient)
{
	UNREGISTER_STATIC_RPC(pRakClient,EnterVehicle);
	UNREGISTER_STATIC_RPC(pRakClient,ExitVehicle);
	UNREGISTER_STATIC_RPC(pRakClient,Unk6B);
	UNREGISTER_STATIC_RPC(pRakClient,Unk25);
	UNREGISTER_STATIC_RPC(pRakClient,Unk26);
	UNREGISTER_STATIC_RPC(pRakClient,Unk27);
	UNREGISTER_STATIC_RPC(pRakClient,UpdateScoresPingsIPs);
	UNREGISTER_STATIC_RPC(pRakClient,SvrStats);
	UNREGISTER_STATIC_RPC(pRakClient,GameModeRestart);
	UNREGISTER_STATIC_RPC(pRakClient,ConnectionRejected);
	UNREGISTER_STATIC_RPC(pRakClient,ClientMessage);
	UNREGISTER_STATIC_RPC(pRakClient,WorldTime);
	UNREGISTER_STATIC_RPC(pRakClient,Unk5F);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3F);
	UNREGISTER_STATIC_RPC(pRakClient,Unk97);
	UNREGISTER_STATIC_RPC(pRakClient,ScmEvent);
	UNREGISTER_STATIC_RPC(pRakClient,Weather);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1D);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1E);
	UNREGISTER_STATIC_RPC(pRakClient,Unk20);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA6);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA3);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA4);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA5);
	UNREGISTER_STATIC_RPC(pRakClient,Unk89);
	UNREGISTER_STATIC_RPC(pRakClient,Unk8A);
	UNREGISTER_STATIC_RPC(pRakClient,Unk8B);
	UNREGISTER_STATIC_RPC(pRakClient,Chat);
	UNREGISTER_STATIC_RPC(pRakClient,RequestClass);
	UNREGISTER_STATIC_RPC(pRakClient,RequestSpawn);
}

//----------------------------------------------------
