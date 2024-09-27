
#include <windows.h>
#include <assert.h>
#define _ASSERT assert

#include "../main.h"
#include "game.h"
#include "util.h"
#include "keystuff.h"
#include "task.h"

extern CGame *pGame;
extern CNetGame *pNetGame;

extern BYTE	*pbyteCurrentPlayer;

//-----------------------------------------------------------
// Used for instancing the local player.
// MATCH
CPlayerPed::CPlayerPed()
{
	m_dwGTAId = 1; // 0x001
	m_pPed = GamePool_FindPlayerPed();
	m_pEntity = (ENTITY_TYPE *)GamePool_FindPlayerPed();

	m_bytePlayerNumber = 0;
	SetPlayerPedPtrRecord(m_bytePlayerNumber,(DWORD)m_pPed);
	ScriptCommand(&set_actor_weapon_droppable,m_dwGTAId,1);
	ScriptCommand(&set_actor_can_be_decapitated,m_dwGTAId,0);

	field_2A8 = 1;
	field_2AC = 1;
	m_dwArrow = 0;
	field_2B9 = 0;
	m_iPissingState = 0;
	m_iDanceState = 0;
	field_2DE = 0;
	field_2E2 = 0;
	m_iCellPhoneEnabled = 0;
	m_bGoggleState = FALSE;
	field_2C1 = 0;
	field_2C5 = 0;
	field_2C9 = 0;
	field_2F6 = 0;
	field_2F7 = 0;
	field_2F8 = 0;

	int x=0;
	while(x!=10) {
		field_4C[x] = 0;
		field_27C[x] = 0;
		memset(&field_74[x], 0, sizeof(struc_97));
		x++;
	}

	field_2F9 = 0;
}

//-----------------------------------------------------------





//-----------------------------------------------------------
// If the game has internally destroyed the ped
// during this frame, the ped pointer should become 0
// MATCH
void CPlayerPed::ResetPointers()
{
	m_pPed = GamePool_Ped_GetAt(m_dwGTAId);
	m_pEntity = (ENTITY_TYPE *)m_pPed;
}

//-----------------------------------------------------------
// MATCH
void CPlayerPed::SetInitialState()
{
	DWORD dwPedPtr = (DWORD)m_pPed;

	_asm push 0
	_asm mov ecx, dwPedPtr
	_asm mov edx, 0x60CD20 ; internal_CPlayerPed_SetInitialState
	_asm call edx
}

//-----------------------------------------------------------
// MATCH
BYTE CPlayerPed::GetSpecialKey()
{
	GTA_CONTROLSET *pInternalKeys = GameGetInternalKeys();

	if(pInternalKeys->wKeys1[11]) // yes
		return 1;
	else if(pInternalKeys->wKeys1[10]) // no
		return 2;
	else if(pInternalKeys->wKeys1[9]) // honk
		return 3;

	return 0;
}

//-----------------------------------------------------------




//-----------------------------------------------------------
// MATCH
CAMERA_AIM * CPlayerPed::GetCurrentAim()
{
	return GameGetInternalAim();
}

//-----------------------------------------------------------
// MATCH
void CPlayerPed::SetCurrentAim(CAMERA_AIM *pAim)
{
	GameStoreRemotePlayerAim(m_bytePlayerNumber, pAim);
}

//-----------------------------------------------------------
// MATCH
BYTE CPlayerPed::GetCurrentWeapon()
{
	if(!m_pPed) return 0;
	if(GamePool_Ped_GetAt(m_dwGTAId) == 0) return 0;

	DWORD dwRetVal;
	ScriptCommand(&get_actor_armed_weapon,m_dwGTAId,&dwRetVal);
	return (BYTE)dwRetVal;
}

//-----------------------------------------------------------
// MATCH
int CPlayerPed::GetCurrentVehicleID()
{
	if(!m_pPed) return 0;

	VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;
	return GamePool_Vehicle_GetIndex(pVehicle);
}

//-----------------------------------------------------------





//-----------------------------------------------------------

void CPlayerPed::HideMarker()
{
	if (m_dwArrow) ScriptCommand(&disable_marker, m_dwArrow);
	m_dwArrow = NULL; // Just make sure
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsOnScreen()
{
	if(m_pPed) return GameIsEntityOnScreen((DWORD *)m_pPed);
	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::SetImmunities(BOOL bBullet, BOOL bFire, BOOL bExplosion, BOOL bDamage, BOOL bUnknown)
{
	if(!m_pPed) return;
	if(GamePool_Ped_GetAt(m_dwGTAId) == 0) return;

	ScriptCommand(&set_actor_immunities, m_dwGTAId, bBullet, bFire, bExplosion, bDamage, bUnknown);
}

//-----------------------------------------------------------

float CPlayerPed::GetHealth()
{
	if(!m_pPed) return 0.0f;
	return m_pPed->fHealth;
}

//-----------------------------------------------------------

void CPlayerPed::SetHealth(float fHealth)
{
	if(!m_pPed) return;
	m_pPed->fHealth = fHealth;
}

//-----------------------------------------------------------

float CPlayerPed::GetArmour()
{
	if(!m_pPed) return 0.0f;
	return m_pPed->fArmour;
}

//-----------------------------------------------------------

void CPlayerPed::SetArmour(float fArmour)
{
	if(!m_pPed) return;
	m_pPed->fArmour = fArmour;
}

//-----------------------------------------------------------

DWORD CPlayerPed::GetStateFlags()
{
	if(!m_pPed) return 0;
	return m_pPed->dwStateFlags;
}

//-----------------------------------------------------------

void CPlayerPed::SetStateFlags(DWORD dwState)
{
	if(!m_pPed) return;
	m_pPed->dwStateFlags = dwState;
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsDead()
{
	if(!m_pPed) return TRUE;
	if(m_pPed->fHealth > 0.0f) return FALSE;
	return TRUE;
}

//-----------------------------------------------------------

BYTE CPlayerPed::GetActionTrigger()
{
	return (BYTE)m_pPed->dwAction;
}

//-----------------------------------------------------------

void CPlayerPed::SetActionTrigger(BYTE byteTrigger)
{
	if(!m_pPed) return;

	m_pPed->dwAction = byteTrigger;
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsInVehicle()
{
	if(!m_pPed) return FALSE;

	if(IN_VEHICLE(m_pPed)) {
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

float CPlayerPed::GetTargetRotation()
{
	if(!m_pPed) return 0.0f;
	if(GamePool_Ped_GetAt(m_dwGTAId) == 0) return 0.0f;

	MATRIX4X4 mat;
	GetMatrix(&mat);

	float fZAngle = atan2(-mat.up.X, mat.up.Y) * 180.0f / PI;
	// Bound it to [0, 360)
	if ( fZAngle < 0.0f )
		fZAngle += 360.0f;
	else if ( fZAngle >= 360.0f )
		fZAngle -= 360.0f;
	return fZAngle;
}

//-----------------------------------------------------------

void CPlayerPed::SetTargetRotation(float fRotation)
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	m_pPed->fRotation2 = DegToRad(fRotation);
	m_pPed->fRotation1 = DegToRad(fRotation);
}

//-----------------------------------------------------------

void CPlayerPed::ForceTargetRotation(float fRotation)
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	m_pPed->fRotation1 = DegToRad(fRotation);
	m_pPed->fRotation2 = DegToRad(fRotation);

	ScriptCommand(&set_actor_z_angle,m_dwGTAId,fRotation);   
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsAPassenger()
{
	if( m_pPed->pVehicle && IN_VEHICLE(m_pPed) )
	{
		VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;

		if( pVehicle->pDriver != m_pPed || 
			pVehicle->entity.nModelIndex == TRAIN_PASSENGER ||
			pVehicle->entity.nModelIndex == TRAIN_FREIGHT ) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------

VEHICLE_TYPE * CPlayerPed::GetGtaVehicle()
{
	return (VEHICLE_TYPE *)m_pPed->pVehicle;
}

//-----------------------------------------------------------



//-----------------------------------------------------------	

void CPlayerPed::GiveWeapon(int iWeaponID, int iAmmo)
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	int iModelID = 0;
	iModelID = GameGetWeaponModelIDFromWeaponID(iWeaponID);

	if(iModelID == -1) return;

	if(!pGame->IsModelLoaded(iModelID)) {
		pGame->RequestModel(iModelID);
		pGame->LoadRequestedModels();
		while(!pGame->IsModelLoaded(iModelID)) Sleep(1);
	}

	*pbyteCurrentPlayer = m_bytePlayerNumber;

	GameStoreLocalPlayerWeaponSkills();
	GameSetRemotePlayerWeaponSkills(m_bytePlayerNumber);

	DWORD dwPedPtr = (DWORD)m_pPed;

	_asm mov ecx, dwPedPtr
	_asm push 1
	_asm push iAmmo
	_asm push iWeaponID
	_asm mov edx, 0x5E6080
	_asm call edx

	GameSetLocalPlayerWeaponSkills();

	SetArmedWeapon(iWeaponID, false);

	*pbyteCurrentPlayer = 0;
}

//-----------------------------------------------------------

void CPlayerPed::ClearAllWeapons()
{
	DWORD dwPedPtr = (DWORD)m_pPed;

	*pbyteCurrentPlayer = m_bytePlayerNumber;

	if(dwPedPtr) {
		_asm mov ecx, dwPedPtr
		_asm mov eax, 0x5E6320
		_asm call eax
	}

	*pbyteCurrentPlayer = 0;
}

//-----------------------------------------------------------

void CPlayerPed::SetArmedWeapon(int iWeaponType, bool bUnk)
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	*pbyteCurrentPlayer = m_bytePlayerNumber;

	GameStoreLocalPlayerWeaponSkills();
	GameSetRemotePlayerWeaponSkills(m_bytePlayerNumber);

	if((!m_pPed || !IN_VEHICLE(m_pPed)) && !bUnk)
		ScriptCommand(&set_actor_armed_weapon,m_dwGTAId,iWeaponType);
	else
	{
		DWORD dwPedPtr = (DWORD)m_pPed;
		if(dwPedPtr) {
			_asm mov ecx, dwPedPtr
			_asm push iWeaponType
			_asm mov edx, 0x5E6280
			_asm call edx
		}
	}

	GameSetLocalPlayerWeaponSkills();

	*pbyteCurrentPlayer = 0;
}

//-----------------------------------------------------------

void CPlayerPed::RemoveWeaponWhenEnteringVehicle()
{
	DWORD dwPedPtr = (DWORD)m_pPed;

	if(dwPedPtr) {
		_asm mov ecx, dwPedPtr
		_asm push 0
		_asm mov edx, 0x5E6370
		_asm call edx
	}
}

//-----------------------------------------------------------

WEAPON_SLOT_TYPE * CPlayerPed::GetCurrentWeaponSlot()
{
	if(m_pPed) {
		return &m_pPed->WeaponSlots[m_pPed->byteCurWeaponSlot];
	}
	return NULL;
}

//-----------------------------------------------------------

BOOL CPlayerPed::HasAmmoForCurrentWeapon()
{
	if(m_pPed) {
		WEAPON_SLOT_TYPE * WeaponSlot = GetCurrentWeaponSlot();

		if(!WeaponSlot) return TRUE;

		// Melee types always have ammo.
		if( WeaponSlot->dwType <= WEAPON_CANE ||
			WeaponSlot->dwType == WEAPON_PARACHUTE ) return TRUE;

		if(!WeaponSlot->dwAmmo) return FALSE;
	}
	return TRUE;
}
//-----------------------------------------------------------

float CPlayerPed::GetDistanceFromVehicle(CVehicle *pVehicle)
{
	MATRIX4X4	matFromPlayer;
	MATRIX4X4	matThis;
	float		fSX,fSY,fSZ;

	GetMatrix(&matThis);
	pVehicle->GetMatrix(&matFromPlayer);

	fSX = (matThis.pos.X - matFromPlayer.pos.X) * (matThis.pos.X - matFromPlayer.pos.X);
	fSY = (matThis.pos.Y - matFromPlayer.pos.Y) * (matThis.pos.Y - matFromPlayer.pos.Y);
	fSZ = (matThis.pos.Z - matFromPlayer.pos.Z) * (matThis.pos.Z - matFromPlayer.pos.Z);

	return (float)sqrt(fSX + fSY + fSZ);
}

//-----------------------------------------------------------

int CPlayerPed::GetVehicleSeatID()
{
	VEHICLE_TYPE *pVehicle;

	if( GetActionTrigger() == ACTION_INCAR && (pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle) != 0 ) {
		if(pVehicle->pDriver == m_pPed) return 0;
		if(pVehicle->pPassengers[0] == m_pPed) return 1;
		if(pVehicle->pPassengers[1] == m_pPed) return 2;
		if(pVehicle->pPassengers[2] == m_pPed) return 3;
		if(pVehicle->pPassengers[3] == m_pPed) return 4;
		if(pVehicle->pPassengers[4] == m_pPed) return 5;
		if(pVehicle->pPassengers[5] == m_pPed) return 6;
		if(pVehicle->pPassengers[6] == m_pPed) return 7;
	}

	return (-1);
}

//-----------------------------------------------------------

void CPlayerPed::PutDirectlyInVehicle(int iVehicleID, int iSeat)
{
	if(!m_pPed) return;
	if(!GamePool_Vehicle_GetAt(iVehicleID)) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if(GetCurrentWeapon() == WEAPON_PARACHUTE) {
		SetArmedWeapon(0, false);
	}

	VEHICLE_TYPE *pVehicle = GamePool_Vehicle_GetAt(iVehicleID);

	if(pVehicle->fHealth == 0.0f) return;

	// Check to make sure internal data structure of the vehicle hasn't been deleted
	// by checking if the vtbl points to CPlaceable_vtbl
	if (pVehicle->entity.vtable == 0x863C40) return;

	if(GetVehicleSubtypeFromVehiclePtr(pVehicle) == VEHICLE_SUBTYPE_CAR || GetVehicleSubtypeFromVehiclePtr(pVehicle) == VEHICLE_SUBTYPE_BIKE) return;
	if(iSeat > pVehicle->byteMaxPassengers) return;

	if(iSeat==0) {
		if(pVehicle->pDriver && IN_VEHICLE(pVehicle->pDriver)) return;
		ScriptCommand(&put_actor_in_car,m_dwGTAId,iVehicleID);
	} else {
		iSeat--;
		ScriptCommand(&put_actor_in_car2,m_dwGTAId,iVehicleID,iSeat);
	}
	if(m_pPed == GamePool_FindPlayerPed() && IN_VEHICLE(m_pPed)) {
		pGame->GetCamera()->SetBehindPlayer();
	}

	if(pNetGame) {
		CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
		// TODO: CPlayerPed::PutDirectlyInVehicle
	}
}

//-----------------------------------------------------------










//-----------------------------------------------------------
// Forceful removal

void CPlayerPed::RemoveFromVehicleAndPutAt(float fX, float fY, float fZ)
{
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;
	if(m_pPed && IN_VEHICLE(m_pPed)) {
		ScriptCommand(&remove_actor_from_car_and_put_at,m_dwGTAId,fX,fY,fZ);
	}
}

//-----------------------------------------------------------

void CPlayerPed::TogglePlayerControllable(int iControllable)
{
	MATRIX4X4 mat;

	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if(!iControllable) {
		ScriptCommand(&toggle_player_controllable,m_bytePlayerNumber,0);
		ScriptCommand(&lock_actor,m_dwGTAId,1);
	} else {
		ScriptCommand(&toggle_player_controllable,m_bytePlayerNumber,1);
		ScriptCommand(&lock_actor,m_dwGTAId,0);
		if(!IsInVehicle()) {
			GetMatrix(&mat);
			TeleportTo(mat.pos.X,mat.pos.Y,mat.pos.Z);
		}
	}
}

//-----------------------------------------------------------







//-----------------------------------------------------------

void CPlayerPed::HandsUp()
{
	if(!m_pPed || IN_VEHICLE(m_pPed)) return;
	if(!IsAdded()) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;
	ScriptCommand(&actor_task_handsup,m_dwGTAId,-2);
}

//-----------------------------------------------------------

BOOL CPlayerPed::HasHandsUp()
{
	if(!m_pPed || IN_VEHICLE(m_pPed)) return FALSE;
	if(!IsAdded()) return FALSE;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return FALSE;
	if(m_pPed->Tasks->pdwJumpJetPack == NULL) return FALSE;
	DWORD dwJmpVtbl = m_pPed->Tasks->pdwJumpJetPack[0];
	if(dwJmpVtbl == 0x85A29C) return TRUE;

	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::HoldItem(int iObject)
{
	if(!m_pPed) return;
	if(!IsAdded()) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	DWORD dwPed = (DWORD)m_pPed;
	_asm push 1
	_asm push iObject
	_asm mov ecx, dwPed
	_asm mov ebx, 0x5E4390
	_asm call ebx
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsJumpTask()
{
	if(m_pPed && !IN_VEHICLE(m_pPed) && m_pPed->Tasks->pdwJumpJetPack)
	{
		return GetTaskTypeFromTask(m_pPed->Tasks->pdwJumpJetPack) == 211;
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsFightTask()
{
	if(m_pPed && !IN_VEHICLE(m_pPed) && m_pPed->Tasks->pdwFighting)
	{
		return GetTaskTypeFromTask(m_pPed->Tasks->pdwFighting) == 1016;
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsTakeFallDamageTask()
{
	if(m_pPed && !IN_VEHICLE(m_pPed) && m_pPed->Tasks->pdwDamage)
	{
		return GetTaskTypeFromTask(m_pPed->Tasks->pdwDamage) == 208;
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsSwimTask()
{
	if(m_pPed && !IN_VEHICLE(m_pPed) && m_pPed->Tasks->pdwSwimWasted)
	{
		return GetTaskTypeFromTask(m_pPed->Tasks->pdwSwimWasted) == 268;
	}
	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::StartJetpack()
{
	if(!m_pPed) return;

	*pbyteCurrentPlayer = m_bytePlayerNumber;

	// reset CTasks so the CJetPack task priority can be enforced
	TeleportTo(m_pPed->entity.mat->pos.X, m_pPed->entity.mat->pos.Y, m_pPed->entity.mat->pos.Z);

	_asm mov eax, 0x439600
	_asm call eax

	*pbyteCurrentPlayer = 0;
}

//-----------------------------------------------------------

void CPlayerPed::StopJetpack()
{
	if(!m_pPed || IN_VEHICLE(m_pPed)) return;

	if(m_pPed->Tasks->pdwJumpJetPack == NULL) return;

	DWORD dwJmpVtbl = m_pPed->Tasks->pdwJumpJetPack[0];

	if(dwJmpVtbl == 0x8705C4) {
		DWORD dwJetPackTask = (DWORD)m_pPed->Tasks->pdwJumpJetPack;
		_asm mov ecx, dwJetPackTask
		_asm mov edx, 0x6801D0
		_asm push 1
		_asm call edx
		m_pPed->Tasks->pdwJumpJetPack = 0;
	}
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsInJetpackMode()
{
	if(!m_pPed || IN_VEHICLE(m_pPed)) return FALSE;
	if(m_pPed->Tasks->pdwJumpJetPack == NULL) return FALSE;

	DWORD dwJmpVtbl = m_pPed->Tasks->pdwJumpJetPack[0];

	if(dwJmpVtbl == 0x8705C4) return TRUE;

	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::StartGoggles()
{
	if (HasGoggles()) return;
	if (FindWeaponSlot( 44 ) == NULL && FindWeaponSlot( 45 ) == NULL)
		GiveWeapon( 44, 1 ); // Prevents crashing due to lack of animations.

	CTaskGoggles* pGoggles = new CTaskGoggles();
	pGoggles->ApplyToPed( this );
	m_bGoggleState = TRUE;
}

//-----------------------------------------------------------

void CPlayerPed::StopGoggles()
{
	if (!m_pPed || !HasGoggles()) return;

	m_bGoggleState = FALSE;
	DWORD dwPedPointer = (DWORD)m_pPed;
	_asm mov ecx, dwPedPointer
	_asm mov eax, 0x5E6010
	_asm call eax
}

//-----------------------------------------------------------

BOOL CPlayerPed::HasGoggles()
{
	if (!m_pPed) return FALSE;
	return (BOOL)(m_pPed->dwActiveVision != 0 || m_bGoggleState);
}

//-----------------------------------------------------------





//-----------------------------------------------------------

WORD CPlayerPed::GetAmmo()
{
	if(m_pPed) {
		WEAPON_SLOT_TYPE * WeaponSlot = GetCurrentWeaponSlot();

		if(!WeaponSlot) return -1;

		// Melee types always have ammo.
		if( WeaponSlot->dwType <= WEAPON_CANE ||
			WeaponSlot->dwType == WEAPON_PARACHUTE ) return -1;

		return (WORD)WeaponSlot->dwAmmo;
	}
	return 0;
}

//-----------------------------------------------------------



//-----------------------------------------------------------

WEAPON_SLOT_TYPE * CPlayerPed::FindWeaponSlot(DWORD dwWeapon)
{
	if (m_pPed)
	{
		BYTE i;
		for (i = 0; i < 13; i++)
		{
			if (m_pPed->WeaponSlots[i].dwType == dwWeapon) return &m_pPed->WeaponSlots[i];
		}
	}
	return NULL;
}

//-----------------------------------------------------------

void CPlayerPed::SetAnimationSet(PCHAR szAnim)
{
	if(m_pPed) {
		ScriptCommand(&set_actor_animation_set,m_dwGTAId,szAnim);
	}
}

//-----------------------------------------------------------

void CPlayerPed::SetMoney(int iAmount)
{
	ScriptCommand(&set_actor_money,m_dwGTAId,0);
	ScriptCommand(&set_actor_money,m_dwGTAId,iAmount);
}

//-----------------------------------------------------------

void CPlayerPed::ApplyAnimation( char *szAnimName, char *szAnimFile, float fT,
								 int opt1, int opt2, int opt3, int opt4, int iUnk )
{
	int iWaitAnimLoad=0;

	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	// Can't allow 'naughty' anims!
	if( !stricmp(szAnimFile,"SEX") )
		return;

	if (!pGame->IsAnimationLoaded(szAnimFile)) {
		pGame->RequestAnimation(szAnimFile);
		while(!pGame->IsAnimationLoaded(szAnimFile)) {
			Sleep(1);
			iWaitAnimLoad++;
			if(iWaitAnimLoad == 15) return; // we can't wait forever
		}		
	}

	ScriptCommand(&apply_animation,m_dwGTAId,szAnimName,szAnimFile,fT,opt1,opt2,opt3,opt4,iUnk);
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsPerformingAnimation(char *szAnimName)
{
	if(m_pPed && ScriptCommand(&is_actor_performing_anim,m_dwGTAId,szAnimName)) {
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------






//-----------------------------------------------------------

ENTITY_TYPE* CPlayerPed::GetGtaContactEntity()
{
	return (ENTITY_TYPE*)m_pPed->pContactEntity;
}

//-----------------------------------------------------------

VEHICLE_TYPE* CPlayerPed::GetGtaContactVehicle()
{
	return (VEHICLE_TYPE*)m_pPed->pContactVehicle;
}

//-----------------------------------------------------------





//-----------------------------------------------------------

char DanceStyleLibs[4][16] = {"WOP","GFUNK","RUNNINGMAN","STRIP"};
char DanceIdleLoops[4][16] = {"DANCE_LOOP","DANCE_LOOP","DANCE_LOOP","STR_Loop_B"};

void CPlayerPed::StartDancing(int iStyle)
{
	if(iStyle < 0 || iStyle > 3) return;

	m_iDanceState = 1;
	m_iDanceStyle = iStyle;

	if(m_bytePlayerNumber == 0) {
		ApplyAnimation(DanceIdleLoops[m_iDanceStyle],DanceStyleLibs[m_iDanceStyle],16.0,1,0,0,0,-1);
	}
}

//-----------------------------------------------------------

void CPlayerPed::StopDancing()
{
	m_iDanceState = 0;
	MATRIX4X4 mat;
	GetMatrix(&mat);
	TeleportTo(mat.pos.X,mat.pos.Y,mat.pos.Z);
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsDancing()
{
	if(m_iDanceState) return TRUE;
	return FALSE;
}

//-----------------------------------------------------------






//-----------------------------------------------------------

void CPlayerPed::ProcessMarkers(BOOL bMarkerStreamingEnabled, float fMarkerStreamRadius, BOOL bVisible)
{
	if(!m_pPed) return;
}

//-----------------------------------------------------------

void CPlayerPed::ApplyCommandTask(char *szTaskName, int p1, int p2, int p3,
								  VECTOR *p4, int p5, float p6, int p7, int p8, int p9)
{
	DWORD dwPed = (DWORD)m_pPed;
	if(!dwPed) return;

    _asm push p9
	_asm push p8
	_asm push p7
	_asm push p6
	_asm push p5
	_asm push p4
	_asm push p3
	_asm push p2
	_asm push p1
	_asm push dwPed
	_asm push szTaskName
	_asm mov ecx, 0xC15448
	_asm mov edx, 0x618970
	_asm call edx
}

//-----------------------------------------------------------

DWORD dwExt4;

void CPlayerPed::DestroyFollowPedTask()
{
	if(!m_pPed) return;
	dwExt4 = (DWORD)m_pPed->Tasks->pdwIK;
	if(!dwExt4) return;

	_asm mov ecx, dwExt4
	_asm mov edx, 0x639330
	_asm push 1
	_asm call edx

	m_pPed->Tasks->pdwIK = 0;
}

//-----------------------------------------------------------

void CPlayerPed::ToggleCellphone(int iOn)
{
	if(!m_pPed) return;
	m_iCellPhoneEnabled = iOn;
	ScriptCommand(&toggle_actor_cellphone,m_dwGTAId,iOn);
}

//-----------------------------------------------------------

int CPlayerPed::IsCellphoneEnabled()
{
    return m_iCellPhoneEnabled;
}

//-----------------------------------------------------------

int CPlayerPed::GetFightingStyle()
{
	if (!m_pPed) return 0;
	return m_pPed->byteFightingStyle;
}

//-----------------------------------------------------------

void CPlayerPed::SetFightingStyle(int iStyle)
{
	if (!m_pPed) return;

	ScriptCommand( &set_fighting_style, m_dwGTAId, iStyle, 6 );
}

//-----------------------------------------------------------

void CPlayerPed::StartPissing()
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if(m_iPissingState != 0) {
		// already started
		return;
	}

	if (!m_bytePlayerNumber) {
		ApplyAnimation("PISS_LOOP","PAULNMAC", 4.0f, 1, 0, 0, 0, -1);
	}

	ScriptCommand(&attach_particle_to_actor2,"PETROLCAN",m_dwGTAId, 
		0.0f, 0.58f, -0.08f, 0.0f, 0.01f, 0.0f, 1, &m_dwPissParticlesHandle);

	ScriptCommand(&make_particle_visible,m_dwPissParticlesHandle);

	m_iPissingState = 1;
}

//-----------------------------------------------------------

void CPlayerPed::StopPissing()
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;
	if(!m_iPissingState) return;

	if(m_dwPissParticlesHandle) {
		ScriptCommand(&destroy_particle,m_dwPissParticlesHandle);
		m_dwPissParticlesHandle = 0;
	}

	MATRIX4X4 mat;
	GetMatrix(&mat);
	TeleportTo(mat.pos.X,mat.pos.Y,mat.pos.Z);

	m_iPissingState = 0;
}

//-----------------------------------------------------------

int CPlayerPed::IsPissing()
{
	return m_iPissingState;
}

//-----------------------------------------------------------

void CPlayerPed::ProcessVehicleHorn()
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	GTA_CONTROLSET *pPlayerControls;

	if(!m_bytePlayerNumber) {
		pPlayerControls = GameGetInternalKeys();
	} else {
		pPlayerControls = GameGetPlayerKeys(m_bytePlayerNumber);
	}

	VEHICLE_TYPE *pGtaVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;

	if(pGtaVehicle) {
		if(IN_VEHICLE(m_pPed) && (pGtaVehicle->pDriver == m_pPed)) {
			if(pPlayerControls->wKeys1[18]) {
				pGtaVehicle->byteHorn2 = 1;
			} else {
				pGtaVehicle->byteHorn2 = 0;
			}
		}
	}
}

//-----------------------------------------------------------
