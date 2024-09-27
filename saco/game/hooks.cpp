
#include "../main.h"
#include "game.h"
#include "util.h"
#include "keystuff.h"
#include "aimstuff.h"

extern int iGtaVersion;
extern CNetGame* pNetGame;
extern CGame* pGame;
extern CChatWindow *pChatWindow;

extern DWORD dwGraphicsLoop; // Used for the external dll game loop.

int WINAPI exc_filter(unsigned int code, struct _EXCEPTION_POINTERS *ep, char *what);

#define NUDE void _declspec(naked)

//-----------------------------------------------------------
// Globals which are used to avoid stack frame alteration
// inside the following hook procedures.

GTA_CONTROLSET *pGcsKeys;

DWORD	dwFarClipHookAddr=0;
DWORD	dwFarClipReturnAddr=0;

// used generically
PED_TYPE	*_pPlayer;

BYTE	byteInternalPlayer=0;
DWORD	dwCurPlayerActor=0;
BYTE	byteCurPlayer=0;
BYTE	byteSavedCameraMode;
WORD	wSavedCameraMode2;
BYTE	*pbyteCameraMode = (BYTE *)0xB6F1A8;
BYTE	*pbyteCurrentPlayer = (BYTE *)0xB7CD74;
WORD    *wCameraMode2 = (WORD*)0xB6F858;
DWORD	*pdwVehicleEnginePed = (DWORD *)0xB6B990;

float   fHealth;

float fFarClip=1400.0f;

//-----------------------------------------------------------
// currently unknown where would it be placed
DWORD GLOBAL_101516D4;
WORD wLastRendObj;
CPlayerPool *_pPlayerPool;

//-----------------------------------------------------------
// x86 codes to perform our unconditional jmp for detour entries. 

BYTE GameProcess_HookJmpCode[] = {0xFF,0x25,0xD1,0xBE,0x53,0x00}; //53BED1
BYTE TaskEnterVehicleDriver_HookJmpCode[] = {0xFF,0x25,0xBB,0x19,0x69,0x00,0x90};//0x6919BB
BYTE TaskExitVehicle_HookJmpCode[] = {0xFF,0x25,0xBA,0xB8,0x63,0x00,0x90};//0x63B8BA
BYTE RadarTranslateColor_HookJmpCode[] = {0xFF,0x25,0x79,0x4A,0x58,0x00,0x90}; // 584A79
BYTE CheatProcessHook_JmpCode[] = {0xFF,0x25,0xAA,0x85,0x43,0x00,0x90}; // 4385AA
BYTE AddVehicleHook_HookJmpCode[] = {0xFF,0x25,0x33,0x14,0x42,0x00}; // 421433
BYTE SetFarClip_HookJmpCode[] = {0xFF,0x25,0x61,0x36,0x53,0x00,0x90,0x90,0x90}; // 533661
BYTE CGameShutdown_HookJmpCode[] = {0xFF,0x25,0xF1,0xC8,0x53,0x00,0x90}; // 53C8F1
BYTE PedDamage_HookJmpCode[] = {0xFF,0x25,0xBC,0x5A,0x4B,0x00}; // 4B5ABC
BYTE VehicleAudio_HookJmpCode[] = {0xFF,0x25,0x74,0x22,0x50,0x00,0x90,0x90,0x90,0x90}; // 502274
BYTE GenTaskAlloc_HookJmpCode[] = {0xFF,0x25,0x61,0x38,0x4C,0x00}; // 4C3861
BYTE GetText_HookJmpCode[] = {0xFF,0x25,0x43,0x00,0x6A,0x00,0x90,0x90,0x90}; // 0x6A004325
BYTE PedSay_HookJmpCode[] = {0xFF,0x25,0xD8,0xFF,0x5E,0x00,0x90}; //5EFFD8
BYTE CProjectileInfo_Update_HookJmpCode[] = {0xFF,0x25,0x1B,0x8B,0x73,0x00}; //00738B1B
BYTE CWeapon__Satchel__Activate_HookJmpCode[] = {0xFF,0x25,0x5B,0x88,0x73,0x00}; // 0073885B
BYTE SetColor_HookJmpCode[] = {0xFF,0x25,0x36,0xA0,0x63,0x00,0x90};
BYTE Rand_HookJmpCode[] = {0xFF,0x25,0x77,0xAB,0x5E,0x00,0x90}; // 5EAB77
BYTE VehicleModel_SetEnvironmentMap_JmpCode[] = {0xFF,0x25,0x39,0x88,0x4C,0x00,0x90,0x90}; // 4C8839
BYTE HOOK_7_JmpCode[] = {0xFF,0x25,0x34,0x39,0x4D,0x00,0x90,0x90,0x90,0x90};
BYTE HOOK_8_JmpCode[] = {0xFF,0x25,0x09,0x46,0x4D,0x00,0x90};
BYTE Unk10_JmpCode[] = {0xFF,0x25,0xE5,0x42,0x4D,0x00,0x90};
BYTE Unk11_JmpCode[] = {0xFF,0x25,0x29,0x3B,0x4D,0x00};
BYTE HOOK_36_JmpCode[] = {0xFF,0x25,0x21,0xC7,0x59,0x00};
BYTE HOOK_40_JmpCode[] = {0xFF,0x25,0x84,0x80,0x53,0x00,0x90};
BYTE HOOK_48_JmpCode[] = {0xFF,0x25,0xA6,0x34,0x55,0x00,0x90,0x90,0x90};
BYTE HOOK_50_JmpCode[] = {0xFF,0x25,0x33,0x34,0x4B,0x00};
BYTE Unk12_JmpCode[] = {0xFF,0x25,0xC6,0x84,0x6D,0x00,0x90,0x90};
BYTE HOOK_61_JmpCode[] = {0xFF,0x25,0xD7,0x87,0x40,0x00};
BYTE HOOK_60_JmpCode[] = {0xFF,0x25,0xDB,0x74,0x56,0x00};

//-----------------------------------------------------------

// TODO: HOOK_*() functions
NUDE HOOK_1() {}
NUDE HOOK_2() {}
NUDE HOOK_3() {}
NUDE HOOK_4() {}
NUDE CPed_Render_Hook() {}

NUDE HOOK_7() {}
NUDE HOOK_8() {}
NUDE HOOK_10() {}
NUDE TaskUseGun_Hook() {}
NUDE HOOK_12() {}
NUDE CPlayerPed_ProcessCollision_Hook() {}
NUDE HOOK_14() {}
NUDE HOOK_15() {}
NUDE AllVehicles_ProcessControl_Hook() {}
NUDE VehicleHorn_Hook() {}
NUDE ZoneOverlay_Hook() {}
NUDE PlayerWalk_Hook() {}
NUDE PickUpPickup_Hook() {}
NUDE CWeapon_FireCamera_Hook() {}
NUDE CCameraCamShake_Sniper_Hook() {}
NUDE CTrain_ProcessControl_Derailment() {}
NUDE TaskEnterVehicleDriver_Hook() {}
NUDE TaskExitVehicle() {}
NUDE CheatProcessorHook() {}
NUDE RadarTranslateColor() {}
NUDE CGameShutdownHook() {}
NUDE PedDamage_Hook() {}
NUDE AnimCrashFixHook() {}
NUDE HOOK_31() {}
NUDE CProjectile_Update_Hook() {}
NUDE CWeapon__Satchel__Activate_Hook() {}
NUDE GetText_Hook() {}
NUDE HOOK_35() {}
NUDE HOOK_36() {}
NUDE HOOK_37() {}
NUDE HOOK_38() {}
NUDE HOOK_39() {}
NUDE HOOK_40() {}
NUDE HOOK_41() {}
NUDE HOOK_42() {}
NUDE HOOK_43() {}
NUDE HOOK_44() {}
NUDE HOOK_45() {}
NUDE HOOK_46() {}
NUDE HOOK_47() {}
NUDE HOOK_48() {}
NUDE HOOK_49() {}
NUDE HOOK_50() {}
NUDE HOOK_51() {}
NUDE HOOK_52() {}
NUDE HOOK_53() {}
NUDE HOOK_54() {}
NUDE HOOK_55() {}
NUDE HOOK_56() {}
NUDE HOOK_57() {}
NUDE HOOK_58() {}
NUDE HOOK_59() {}
NUDE HOOK_60() {}
NUDE HOOK_61() {}
NUDE HOOK_62() {}
NUDE HOOK_63() {}
NUDE HOOK_64() {}
NUDE HOOK_65() {}
NUDE HOOK_66() {}

//-----------------------------------------------------------

NUDE HOOK_6()
{
	Sleep(1);

	_asm mov edx, 0x561A80
	_asm jmp edx
}

//-----------------------------------------------------------

NUDE GameProcessHook()
{
	_asm add esp, 190h
	_asm ret
}

//-----------------------------------------------------------
// A hook function that switches keys for
// CPlayerPed::ProcessControl(void)

BYTE bytePatchPedRot[6] = { 0xD9,0x96,0x5C,0x05,0x00,0x00 };
BYTE bytePatchPedRot2[6] = { 0xD9,0x96,0x5C,0x05,0x00,0x00 };
BYTE bytePatchPedPos[5] = { 0xE8,0x17,0x88,0xFF,0xFF };

void TryProcessPedControl()
{
	__try
	{
		// call the internal CPlayerPed:ProcessControl
		_asm mov ecx, dwCurPlayerActor
		_asm mov edx, 0x60EA90
		_asm call edx
	}
	__except(exc_filter(GetExceptionCode(), GetExceptionInformation(), "opcode"))
	{
		if(*pbyteCurrentPlayer != 0 && pNetGame) {
			_pPlayerPool = pNetGame->GetPlayerPool();

			// TODO: TryProcessPedControl()
		}
	}
}

void DisablePedRotationFromCamera()
{
	// Reapply the no ped rots from Cam patch
	memset((PVOID)0x6884C4,0x90,6);

	memset((PVOID)0x688200,0x90,6);
}

void EnablePedRotationFromCamera()
{
	// Apply the original code to set ped rot from Cam
	memcpy((PVOID)0x6884C4,bytePatchPedRot,6);

	memcpy((PVOID)0x688200,bytePatchPedRot2,6);
}

void DisablePedPositionUpdate()
{
	UnFuck(0x5E92F4,5);
	memset((PVOID)0x5E92F4,0x90,5);
}

void EnablePedPositionUpdate()
{
	memcpy((PVOID)0x5E92F4,bytePatchPedPos,5);
}

NUDE CPlayerPed_ProcessControl_Hook()
{
	_asm mov dwCurPlayerActor, ecx // store the passed actor
	_asm pushad

	_pPlayer = (PED_TYPE *)dwCurPlayerActor;
	byteInternalPlayer = *pbyteCurrentPlayer; // get the current internal player number
	byteCurPlayer = FindPlayerNumFromPedPtr(dwCurPlayerActor); // get the ordinal of the passed actor

	if( dwCurPlayerActor && 
		(byteCurPlayer != 0) &&
		(byteInternalPlayer == 0) ) // not local player and local player's keys set.
	{
		// key switching
		GameStoreLocalPlayerKeys(); // remember local player's keys
		GameSetRemotePlayerKeys(byteCurPlayer); // set remote player's keys

		// save the internal cammode, apply the context.
		byteSavedCameraMode = *pbyteCameraMode;
		*pbyteCameraMode = GameGetPlayerCameraMode(byteCurPlayer);

		// save the second internal cammode, apply the context
		wSavedCameraMode2 = *wCameraMode2;
		*wCameraMode2 = GameGetPlayerCameraMode(byteCurPlayer);
		if(*wCameraMode2 == 4) *wCameraMode2 = 0;

		// save the camera zoom factor, apply the context
		GameStoreLocalPlayerCameraExtZoom();
		GameSetRemotePlayerCameraExtZoom(byteCurPlayer);

		// aim switching
		GameStoreLocalPlayerAim();
		GameSetRemotePlayerAim(byteCurPlayer);

		GameStoreLocalPlayerWeaponSkills();
		GameSetRemotePlayerWeaponSkills(byteCurPlayer);

		*pbyteCurrentPlayer = byteCurPlayer; // Set the internal player to the passed actor

		fHealth = _pPlayer->fHealth;

		DisablePedPositionUpdate();
		TryProcessPedControl();
		EnablePedPositionUpdate();
	}
	else // it's the local player or keys have already been set.
	{
		// TODO: CPlayerPed_ProcessControl_Hook()
	}

	_asm popad
	_asm ret
}






//-----------------------------------------------------------

void InstallMethodHook(	DWORD dwInstallAddress,
						DWORD dwHookFunction )
{
	DWORD oldProt, oldProt2;
	VirtualProtect((LPVOID)dwInstallAddress,4,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PDWORD)dwInstallAddress = (DWORD)dwHookFunction;
	VirtualProtect((LPVOID)dwInstallAddress,4,oldProt,&oldProt2);
}

//-----------------------------------------------------------

void InstallHook( DWORD dwInstallAddress,
				  DWORD dwHookFunction,
				  DWORD dwHookStorage,
				  BYTE * pbyteJmpCode,
				  int iJmpCodeSize )
{
	DWORD oldProt, oldProt2;

	// Install the pointer to procaddr.
	VirtualProtect((PVOID)dwHookStorage,4,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PDWORD)dwHookStorage = (DWORD)dwHookFunction;
	VirtualProtect((PVOID)dwHookStorage,4,oldProt,&oldProt2);

	// Install the Jmp code.
	VirtualProtect((PVOID)dwInstallAddress,iJmpCodeSize,PAGE_EXECUTE_READWRITE,&oldProt);
	memcpy((PVOID)dwInstallAddress,pbyteJmpCode,iJmpCodeSize);
	VirtualProtect((PVOID)dwInstallAddress,iJmpCodeSize,oldProt,&oldProt2);
}

//-----------------------------------------------------------

void InstallCallHook(DWORD dwInstallAddress, DWORD dwHookFunction, BYTE byteJumpCode = 0xE8)
{
	DWORD oldProt, oldProt2;
	DWORD disp = dwHookFunction - (dwInstallAddress + 5);

	VirtualProtect((LPVOID)dwInstallAddress,5,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PBYTE)(dwInstallAddress) = byteJumpCode;
	*(PDWORD)(dwInstallAddress+1) = (DWORD)disp;
	VirtualProtect((LPVOID)dwInstallAddress,5,oldProt,&oldProt2);
}

//-----------------------------------------------------------

void InstallGameAndGraphicsLoopHooks()
{
	UnFuck(0x53EB13,4);
	*(int *)0x53EB13 = dwGraphicsLoop - 0x53EB12 - 5; // relative addr

	InstallCallHook(0x58FC53,(DWORD)HOOK_1);

	InstallCallHook(0x58FBBF,(DWORD)HOOK_2);

	InstallCallHook(0x53C104,(DWORD)HOOK_3);

	InstallCallHook(0x53E981,(DWORD)HOOK_4);

	// For fixing fogging issues (needed for both debug and net)
	InstallMethodHook(0x86D1B0,(DWORD)CPed_Render_Hook); // This is PlayerPed
	InstallMethodHook(0x86C0F0,(DWORD)CPed_Render_Hook);
	InstallMethodHook(0x86C168,(DWORD)CPed_Render_Hook);
	InstallMethodHook(0x86C248,(DWORD)CPed_Render_Hook);
	InstallMethodHook(0x86C3A0,(DWORD)CPed_Render_Hook);

	InstallCallHook(0x53E930,(DWORD)HOOK_6);
}

//-----------------------------------------------------------

void GameInstallHooks()
{
	InstallGameAndGraphicsLoopHooks();

	InstallHook(0x4D3AA0,(DWORD)HOOK_7,0x4D3934,HOOK_7_JmpCode,sizeof(HOOK_7_JmpCode));

	InstallHook(0x4D4610,(DWORD)HOOK_8,0x4D4609,HOOK_8_JmpCode,sizeof(HOOK_8_JmpCode));

	InstallMethodHook(0x86D190,(DWORD)CPlayerPed_ProcessControl_Hook);

	InstallMethodHook(0x86C0D0,(DWORD)HOOK_10);

	InstallMethodHook(0x86D744,(DWORD)TaskUseGun_Hook);

	InstallCallHook(0x7330A2,(DWORD)HOOK_12);

	InstallMethodHook(0x86D194,(DWORD)CPlayerPed_ProcessCollision_Hook);

	InstallCallHook(0x5689FD,(DWORD)HOOK_14);

	InstallCallHook(0x53EA03,(DWORD)HOOK_15);

	InstallMethodHook(0x871148,(DWORD)AllVehicles_ProcessControl_Hook); // Automobile
	InstallMethodHook(0x8721C8,(DWORD)AllVehicles_ProcessControl_Hook); // boat
	InstallMethodHook(0x871388,(DWORD)AllVehicles_ProcessControl_Hook); // motorbike1
	InstallMethodHook(0x871970,(DWORD)AllVehicles_ProcessControl_Hook); // plane
	InstallMethodHook(0x8716A8,(DWORD)AllVehicles_ProcessControl_Hook); // heli
	InstallMethodHook(0x871550,(DWORD)AllVehicles_ProcessControl_Hook); // pushbike
	InstallMethodHook(0x871800,(DWORD)AllVehicles_ProcessControl_Hook); // truck
	InstallMethodHook(0x871B10,(DWORD)AllVehicles_ProcessControl_Hook); // quad
	InstallMethodHook(0x872398,(DWORD)AllVehicles_ProcessControl_Hook); // train
	InstallMethodHook(0x871C50,(DWORD)AllVehicles_ProcessControl_Hook);

	InstallCallHook(0x501B1D,(DWORD)VehicleHorn_Hook);
	InstallCallHook(0x501B42,(DWORD)VehicleHorn_Hook);
	InstallCallHook(0x501FC2,(DWORD)VehicleHorn_Hook);
	InstallCallHook(0x502067,(DWORD)VehicleHorn_Hook);
	InstallCallHook(0x5021AE,(DWORD)VehicleHorn_Hook);

	// Radar and map hooks for gang zones
	InstallCallHook(0x5869BF,(DWORD)ZoneOverlay_Hook);
	InstallCallHook(0x5759E4,(DWORD)ZoneOverlay_Hook);

	InstallCallHook(0x609A4E,(DWORD)PlayerWalk_Hook);
	InstallCallHook(0x4579C6,(DWORD)PickUpPickup_Hook, 0xE9);
	InstallCallHook(0x73C252,(DWORD)CWeapon_FireCamera_Hook, 0xE9);

	// Hook the call to CCamera::CamShake when called for sniper fire
	InstallCallHook(0x73ACE2,(DWORD)CCameraCamShake_Sniper_Hook);

	// Hook the train derailment code
	InstallCallHook(0x6F8CF8,(DWORD)CTrain_ProcessControl_Derailment);

	InstallHook(0x6402F0,(DWORD)TaskEnterVehicleDriver_Hook,
		0x6919BB,TaskEnterVehicleDriver_HookJmpCode,sizeof(TaskEnterVehicleDriver_HookJmpCode));

	InstallHook(0x63B8C0,(DWORD)TaskExitVehicle,
		0x63B8BA,TaskExitVehicle_HookJmpCode,sizeof(TaskExitVehicle_HookJmpCode));

	InstallHook(0x438576,(DWORD)CheatProcessorHook,
		0x4385AA,CheatProcessHook_JmpCode,sizeof(CheatProcessHook_JmpCode));

	InstallHook(0x584770,(DWORD)RadarTranslateColor,0x584A79,
		RadarTranslateColor_HookJmpCode,sizeof(RadarTranslateColor_HookJmpCode));

	InstallHook(0x53C900,(DWORD)CGameShutdownHook,0x53C8F1,
		CGameShutdown_HookJmpCode,sizeof(CGameShutdown_HookJmpCode));

	InstallHook(0x4B5AC0,(DWORD)PedDamage_Hook,0x4B5ABC,
		PedDamage_HookJmpCode,sizeof(PedDamage_HookJmpCode));

	// Fix for 0x004D41C5 crash
	InstallCallHook(0x4D41C0, (DWORD)AnimCrashFixHook, 0xE9);

	InstallCallHook(0x4E7427,(DWORD)HOOK_31);

	// Fix for crash when the player who threw the satchel died
	InstallHook(0x738F3A, (DWORD)CProjectile_Update_Hook, 0x738B1B, CProjectileInfo_Update_HookJmpCode, sizeof(CProjectileInfo_Update_HookJmpCode));
	// Fix for all satchels blowing up when someone activated their satchel
	InstallHook(0x738877, (DWORD)CWeapon__Satchel__Activate_Hook, 0x73885B, CWeapon__Satchel__Activate_HookJmpCode, sizeof(CWeapon__Satchel__Activate_HookJmpCode));
	
	InstallHook(0x6A0050, (DWORD)GetText_Hook, 0x6A0043, GetText_HookJmpCode, sizeof (GetText_HookJmpCode));

	InstallCallHook(0x6FDED6,(DWORD)HOOK_35);

	if(iGtaVersion == GTASA_VERSION_USA10)
	{
		InstallHook(0x7FB020,(DWORD)HOOK_36,0x59C721,HOOK_36_JmpCode,sizeof(HOOK_36_JmpCode));
		GLOBAL_101516D4 = 0x7FB026;
	}
	else
	{
		InstallHook(0x7FB060,(DWORD)HOOK_36,0x59C721,HOOK_36_JmpCode,sizeof(HOOK_36_JmpCode));
		GLOBAL_101516D4 = 0x7FB066;
	}

	InstallCallHook(0x6D0E7E,(DWORD)HOOK_37);

	InstallMethodHook(0x866FA8,(DWORD)HOOK_38);

	InstallCallHook(0x586C0A,(DWORD)HOOK_39);

	InstallHook(0x538090,(DWORD)HOOK_40,0x538084,HOOK_40_JmpCode,sizeof(HOOK_40_JmpCode));

	InstallCallHook(0x718599,(DWORD)HOOK_41);

	InstallMethodHook(0x866F7C,(DWORD)HOOK_42);

	InstallMethodHook(0x866F80,(DWORD)HOOK_43);
	InstallMethodHook(0x8585E8,(DWORD)HOOK_43);

	InstallMethodHook(0x871218,(DWORD)HOOK_44);
	InstallMethodHook(0x871778,(DWORD)HOOK_44);
	InstallMethodHook(0x8718D0,(DWORD)HOOK_44);
	InstallMethodHook(0x871A40,(DWORD)HOOK_44);
	InstallMethodHook(0x871BE0,(DWORD)HOOK_44);

	InstallCallHook(0x5648D3,(DWORD)HOOK_45);

	InstallCallHook(0x53DFDD,(DWORD)HOOK_46);

	InstallCallHook(0x53E019,(DWORD)HOOK_47);

	InstallHook(0x5534B0,(DWORD)HOOK_48,0x5534A6,HOOK_48_JmpCode,sizeof(HOOK_48_JmpCode));

	InstallCallHook(0x5342F9,(DWORD)HOOK_49);

	InstallHook(0x4B35A0,(DWORD)HOOK_50,0x4B3433,HOOK_50_JmpCode,sizeof(HOOK_50_JmpCode));

	InstallCallHook(0x41B02E,(DWORD)HOOK_51);

	InstallCallHook(0x41AF80,(DWORD)HOOK_52);

	InstallCallHook(0x41AB78,(DWORD)HOOK_53);

	InstallMethodHook(0x871178,(DWORD)HOOK_54);
	InstallMethodHook(0x8716D8,(DWORD)HOOK_54);
	InstallMethodHook(0x8719A0,(DWORD)HOOK_54);
	InstallMethodHook(0x871B40,(DWORD)HOOK_54);

	InstallMethodHook(0x8713B8,(DWORD)HOOK_55);
	InstallMethodHook(0x871580,(DWORD)HOOK_55);

	InstallMethodHook(0x871830,(DWORD)HOOK_56);

	InstallMethodHook(0x8721F8,(DWORD)HOOK_57);

	InstallCallHook(0x6E0954,(DWORD)HOOK_58);
	InstallCallHook(0x6B2BCB,(DWORD)HOOK_58);
	InstallCallHook(0x4F77DA,(DWORD)HOOK_58);

	InstallMethodHook(0x872A74,(DWORD)HOOK_59);

	InstallHook(0x5674E0,(DWORD)HOOK_60,0x5674DB,HOOK_60_JmpCode,sizeof(HOOK_60_JmpCode));
}

//-----------------------------------------------------------

void FUNC_100A6FF0()
{
	InstallHook(0x4087EA,(DWORD)HOOK_61,0x4087D7,HOOK_61_JmpCode,sizeof(HOOK_61_JmpCode));
}

//-----------------------------------------------------------

void FUNC_100A7010()
{
	InstallCallHook(0x742495,(DWORD)HOOK_62);
	InstallCallHook(0x7424EC,(DWORD)HOOK_62);
	InstallCallHook(0x742548,(DWORD)HOOK_62);

	InstallCallHook(0x740721,(DWORD)HOOK_63);
	InstallCallHook(0x740B69,(DWORD)HOOK_63);
	InstallCallHook(0x736247,(DWORD)HOOK_63);

	InstallCallHook(0x7424CB,(DWORD)HOOK_64);

	InstallCallHook(0x73AC4B,(DWORD)HOOK_65);
}

//-----------------------------------------------------------

void FUNC_100A71C0()
{
	InstallCallHook(0x6D7C90,(DWORD)HOOK_66,0xE9);
}

//-----------------------------------------------------------
