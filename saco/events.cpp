
#include "main.h"
#include "game/util.h"

extern CNetGame *pNetGame;
extern CGame *pGame;

#define NUDE void _declspec(naked)

DWORD dwParams[4];

#define EVENT_TYPE_PAINTJOB			1
#define EVENT_TYPE_CARCOMPONENT		2
#define EVENT_TYPE_CARCOLOR			3
#define EVENT_ENTEREXIT_MODSHOP		4

extern BOOL bFirstSpawn;

//----------------------------------------------------------

void ProcessIncommingEvent(PLAYERID playerId, int iEventType, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3)
{
	CVehicle *pVehicle;

	if(!pNetGame) return;
	if(bFirstSpawn) return; // Local player has never spawned.

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	if(!pPlayerPool) return;
	if(!pVehiclePool) return;

	switch(iEventType) {

		case EVENT_TYPE_PAINTJOB:
			pVehicle = pVehiclePool->GetAt(dwParam1);
			if(pVehicle) pVehicle->SetPaintJob(dwParam2);
			break;

		case EVENT_TYPE_CARCOMPONENT:
			pVehicle = pVehiclePool->GetAt(dwParam1);
			if(pVehicle) pVehicle->AddComponent(dwParam2);
			break;

		case EVENT_TYPE_CARCOLOR:
			pVehicle = pVehiclePool->GetAt(dwParam1);
			if(pVehicle) pVehicle->SetColor(dwParam2,dwParam3);
			break;

		case EVENT_ENTEREXIT_MODSHOP:
			if(playerId > MAX_PLAYERS) return;
			break;
	}

	// TODO: ProcessIncommingEvent
}

//----------------------------------------------------------

VOID ProcessOutgoingEvent(DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, int iEventType)
{
	if(!pNetGame) return;

	int iVehicleID;
	CVehicle *pVehicle;
	VEHICLE_TYPE *pVehiclePtr;
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	RakNet::BitStream bsSend;

	switch(iEventType) {

		case EVENT_TYPE_PAINTJOB:
			iVehicleID = pVehiclePool->FindIDFromGtaPtr(GamePool_Vehicle_GetAt(dwParam1));
			if(iVehicleID != INVALID_VEHICLE_ID) {
				bsSend.Write(iVehicleID);
				bsSend.Write(dwParam2);
				bsSend.Write(dwParam3);
				bsSend.Write(iEventType);
				pNetGame->GetRakClient()->RPC(RPC_ScmEvent,&bsSend,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,false);
			}
			break;

		case EVENT_TYPE_CARCOMPONENT:
			iVehicleID = pVehiclePool->FindIDFromGtaPtr(GamePool_Vehicle_GetAt(dwParam1));
			if(iVehicleID != INVALID_VEHICLE_ID) {
				bsSend.Write(iVehicleID);
				bsSend.Write(dwParam2);
				bsSend.Write(dwParam3);
				bsSend.Write(iEventType);
				pNetGame->GetRakClient()->RPC(RPC_ScmEvent,&bsSend,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,false);
			}
			break;

		case EVENT_TYPE_CARCOLOR:
			iVehicleID = pVehiclePool->FindIDFromGtaPtr(GamePool_Vehicle_GetAt(dwParam1));
			if(iVehicleID != INVALID_VEHICLE_ID) {
				bsSend.Write(iVehicleID);
				bsSend.Write(dwParam2);
				bsSend.Write(dwParam3);
				bsSend.Write(iEventType);
				pNetGame->GetRakClient()->RPC(RPC_ScmEvent,&bsSend,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,false);
				
				pVehicle = pVehiclePool->GetAt(iVehicleID);
				if(pVehicle) {
					pVehicle->SetColor(dwParam2,dwParam3);
				}
			}
			break;

		case EVENT_ENTEREXIT_MODSHOP:
			iVehicleID = pVehiclePool->FindIDFromGtaPtr(GamePool_Vehicle_GetAt(dwParam1));
			pVehicle = pVehiclePool->GetAt(iVehicleID);
			if(iVehicleID != INVALID_VEHICLE_ID) {
				bsSend.Write(iVehicleID);
				bsSend.Write(dwParam2);
				bsSend.Write(dwParam3);
				bsSend.Write(iEventType);
				pNetGame->GetRakClient()->RPC(RPC_ScmEvent,&bsSend,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,false);

				if(pVehicle) {
					pVehiclePtr = pVehicle->m_pVehicle;
					if(pVehiclePtr) {
						if(dwParam2 == 0) {
							pVehicle->SetColor(pVehiclePtr->byteColor1,pVehiclePtr->byteColor2);
							pVehicle->field_71 = 0;
						} else {
							pVehicle->field_71 = 1;
						}
					}
					pVehicle->SetColor(dwParam2,dwParam3);
				}
			}
			break;
	}
}

//----------------------------------------------------------

NUDE SetEventCustomOpcode()
{
	_asm pushad

	_asm push 4
	_asm mov ecx, esi
	_asm mov ebx, 0x464080 // CRunningScript::CollectParamaters()
	_asm call ebx

	dwParams[1] = *(PDWORD)0xA43C7C;
	dwParams[2] = *(PDWORD)0xA43C80;
	dwParams[3] = *(PDWORD)0xA43C84;
	dwParams[0] = *(PDWORD)0xA43C78;

	ProcessOutgoingEvent(dwParams[1],dwParams[2],dwParams[3],dwParams[0]);

	_asm popad

	_asm mov ebx, 0x47BF2B // return to ProcessCommands2500To2600
	_asm jmp ebx
}

//----------------------------------------------------------

int IsDriving;
DWORD ScmInst;
CPlayerPed *pDriverPed;

NUDE PlayerDriverCustomOpcode()
{
	_asm pushad
	_asm mov ScmInst, esi

	IsDriving = 1;

	if(pGame) {
		pDriverPed = pGame->FindPlayerPed();
		if(pDriverPed->IsInVehicle() && !pDriverPed->IsAPassenger()) {
			IsDriving = 0;
		}
	}

	_asm push IsDriving
	_asm mov ecx, ScmInst
	_asm mov ebx, 0x4859D0 // CRunningScript::UpdateCompareFlag
	_asm call ebx

	_asm popad

	_asm mov ebx, 0x47BF2B // return to ProcessCommands2500To2600
	_asm jmp ebx
}

//----------------------------------------------------------

void InstallSCMEventsProcessor()
{
	*(DWORD *)0x47BF54 = (DWORD)SetEventCustomOpcode; // opcode 09C6 - 2502
	*(DWORD *)0x47BF88 = (DWORD)PlayerDriverCustomOpcode; // opcode 09D3 - 2515
}

//----------------------------------------------------------
