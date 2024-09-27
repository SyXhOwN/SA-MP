
#include <windows.h>
#include <math.h>
#include <stdio.h>

#include "../main.h"
#include "util.h"
#include "entity.h"

extern CGame *pGame;
extern CNetGame	*pNetGame;
extern CChatWindow *pChatWindow;

//-----------------------------------------------------------
// MATCH
void CEntity::GetMatrix(PMATRIX4X4 Matrix)
{
	if (!m_pEntity || !m_pEntity->mat) return;

	Matrix->right.X = m_pEntity->mat->right.X;
	Matrix->right.Y = m_pEntity->mat->right.Y;
	Matrix->right.Z = m_pEntity->mat->right.Z;

	Matrix->up.X = m_pEntity->mat->up.X;
	Matrix->up.Y = m_pEntity->mat->up.Y;
	Matrix->up.Z = m_pEntity->mat->up.Z;

	Matrix->at.X = m_pEntity->mat->at.X;
	Matrix->at.Y = m_pEntity->mat->at.Y;
	Matrix->at.Z = m_pEntity->mat->at.Z;

	Matrix->pos.X = m_pEntity->mat->pos.X;
	Matrix->pos.Y = m_pEntity->mat->pos.Y;
	Matrix->pos.Z = m_pEntity->mat->pos.Z;
}

//-----------------------------------------------------------
// MATCH
void CEntity::SetMatrix(MATRIX4X4 Matrix)
{
	if (!m_pEntity || !m_pEntity->mat) return;

	m_pEntity->mat->right.X = Matrix.right.X;
	m_pEntity->mat->right.Y = Matrix.right.Y;
	m_pEntity->mat->right.Z = Matrix.right.Z;

	m_pEntity->mat->up.X = Matrix.up.X;
	m_pEntity->mat->up.Y = Matrix.up.Y;
	m_pEntity->mat->up.Z = Matrix.up.Z;

	m_pEntity->mat->at.X = Matrix.at.X;
	m_pEntity->mat->at.Y = Matrix.at.Y;
	m_pEntity->mat->at.Z = Matrix.at.Z;

	m_pEntity->mat->pos.X = Matrix.pos.X;
	m_pEntity->mat->pos.Y = Matrix.pos.Y;
	m_pEntity->mat->pos.Z = Matrix.pos.Z;
}

//-----------------------------------------------------------

void CEntity::UpdateRwMatrixAndFrame()
{
	if(!m_pEntity) return;
	if(m_pEntity->vtable == 0x863C40) return;

	if(!m_pEntity) return;

	DWORD dwRenderWare = (DWORD)m_pEntity->pdwRenderWare;
	DWORD dwMatrix = (DWORD)m_pEntity->mat;
	DWORD dwEntity = (DWORD)m_pEntity;

	if(!m_pEntity) return;
	if(!m_pEntity->pdwRenderWare) return;
	if(!m_pEntity->mat) return;

	_asm mov edx, dwRenderWare
	_asm mov eax, [edx+4]
	_asm add eax, 16
	_asm push eax
	_asm mov ecx, dwMatrix
	_asm mov edx, 0x59AD70 ; CMatrix__UpdateRwMatrix
	_asm call edx

	_asm mov ecx, dwEntity
	_asm mov edx, 0x532B00 ; CEntity__UpdateRwFrame
	_asm call edx
}

//-----------------------------------------------------------
// MATCH
void CEntity::GetMoveSpeedVector(PVECTOR Vector)
{
	Vector->X = m_pEntity->vecMoveSpeed.X;
	Vector->Y = m_pEntity->vecMoveSpeed.Y;
	Vector->Z = m_pEntity->vecMoveSpeed.Z;
}

//-----------------------------------------------------------
// MATCH
void CEntity::SetMoveSpeedVector(VECTOR Vector)
{
	m_pEntity->vecMoveSpeed.X = Vector.X;
	m_pEntity->vecMoveSpeed.Y = Vector.Y;
	m_pEntity->vecMoveSpeed.Z = Vector.Z;
}

//-----------------------------------------------------------

void CEntity::FUNC_1009ED40(float fX, float fY, float fZ)
{
	int x=4;
	while(x) {
		field_4[x-1].X = field_4[x].X;
		field_4[x-1].Y = field_4[x].Y;
		field_4[x-1].Z = field_4[x].Z;
		x--;
	}

	field_4[4].X = fX;
	field_4[4].Y = fY;
	field_4[4].Z = fZ;

	fX = field_4[0].X;
	fY = field_4[0].Y;
	fZ = field_4[0].Z;

	fX += field_4[1].X;
	fY += field_4[1].Y;
	fZ += field_4[1].Z;

	fX += field_4[2].X;
	fY += field_4[2].Y;
	fZ += field_4[2].Z;

	fX += field_4[3].X;
	fY += field_4[3].Y;
	fZ += field_4[3].Z;

	fX += field_4[4].X;
	fY += field_4[4].Y;
	fZ += field_4[4].Z;

	fX *= 0.2f;
	fY *= 0.2f;
	fZ *= 0.2f;

	m_pEntity->vecMoveSpeed.X = fX;
	m_pEntity->vecMoveSpeed.Y = fY;
	m_pEntity->vecMoveSpeed.Z = fZ;

	/*fX = field_4[0].X;
	fY = field_4[0].Y;
	fZ = field_4[0].Z;

	fX += field_4[1].X;
	fY += field_4[1].Y;
	fZ += field_4[1].Z;

	fX += field_4[2].X;
	fY += field_4[2].Y;
	fZ += field_4[2].Z;

	float fRX = (field_4[0].X + field_4[1].X + field_4[2].X + field_4[3].X + field_4[4].X) * 0.2f;
	float fRY = (field_4[0].Y + field_4[1].Y + field_4[2].Y + field_4[3].Y + field_4[4].Y) * 0.2f;
	float fRZ = (field_4[0].Z + field_4[1].Z + field_4[2].Z + field_4[3].Z + field_4[4].Z) * 0.2f;

	m_pEntity->vecMoveSpeed.X = fX;
	m_pEntity->vecMoveSpeed.Y = fY;
	m_pEntity->vecMoveSpeed.Z = fZ;
	*/	
}

//-----------------------------------------------------------
// MATCH
void CEntity::GetTurnSpeedVector(PVECTOR Vector)
{
	Vector->X = m_pEntity->vecTurnSpeed.X;
	Vector->Y = m_pEntity->vecTurnSpeed.Y;
	Vector->Z = m_pEntity->vecTurnSpeed.Z;
}

//-----------------------------------------------------------
// MATCH
void CEntity::SetTurnSpeedVector(VECTOR Vector)
{
	m_pEntity->vecTurnSpeed.X = Vector.X;
	m_pEntity->vecTurnSpeed.Y = Vector.Y;
	m_pEntity->vecTurnSpeed.Z = Vector.Z;
}

//-----------------------------------------------------------
// MATCH
void CEntity::ApplyTurnSpeed()
{
	DWORD dwEnt = (DWORD)m_pEntity;
	if(!dwEnt) return;

	_asm mov ecx, dwEnt
	_asm mov eax, 0x542E20
	_asm call eax
}

//-----------------------------------------------------------
// MATCH
float CEntity::GetDistanceFromCentreOfMassToBaseOfModel()
{
	DWORD dwEnt = (DWORD)m_pEntity;
	float fResult = 0.0f;

	if(!dwEnt) return 0.0f;

	_asm mov ecx, dwEnt
	_asm mov edx, 0x536BE0
	_asm call edx
	_asm mov fResult, eax

	return fResult;
}

//-----------------------------------------------------------
// MATCH
void CEntity::GetBoundCentre(PVECTOR Vector)
{
	DWORD dwEnt = (DWORD)m_pEntity;
	if(!dwEnt) return;

	_asm push Vector
	_asm mov ecx, dwEnt
	_asm mov edx, 0x534250
	_asm call edx
}

//-----------------------------------------------------------
// MATCH
void CEntity::GetBoundRect(PFRECT Rect)
{
	DWORD dwEnt = (DWORD)m_pEntity;
	if(!dwEnt) return;

	_asm push Rect
	_asm mov ecx, dwEnt
	_asm mov edx, 0x534120
	_asm call edx
}

//-----------------------------------------------------------
// MATCH
BOOL CEntity::SetModelIndex(UINT uiModel)
{
	if(!m_pEntity) return FALSE;

	BOOL bResult=FALSE;
	int iCounter=0;

	if(!pGame->IsModelLoaded(uiModel) && !GetModelRwObject(uiModel)) {
		pGame->RequestModel(uiModel);
		pGame->LoadRequestedModels();
		while(!pGame->IsModelLoaded(uiModel)) {
			Sleep(1);
			iCounter++;
			if(iCounter>200) {
				if(pChatWindow)
					pChatWindow->AddDebugMessage("Warning: Model %u wouldn't load in time!", uiModel);
				return FALSE;
			}
		}
		bResult = TRUE;
	}

	DWORD dwThisEntity = (DWORD)m_pEntity;

	_asm {
		mov		esi, dwThisEntity
		mov		edi, uiModel
		mov     edx, [esi]
		mov     ecx, esi
		call    dword ptr [edx+32] ; destroy RW
		mov     eax, [esi]
		mov		edx, edi
		push    edi
		mov     ecx, esi
		mov     word ptr [esi+34], dx
		call    dword ptr [eax+20] ; SetModelIndex
	}

	return bResult;
}

//-----------------------------------------------------------
// MATCH
UINT CEntity::GetModelIndex()
{
	return m_pEntity->nModelIndex;
}

//-----------------------------------------------------------
// MATCH
void CEntity::TeleportTo(float x, float y, float z)
{
	DWORD dwThisEntity = (DWORD)m_pEntity;

	if(dwThisEntity && m_pEntity->vtable != 0x863C40) {
		if( GetModelIndex() != TRAIN_PASSENGER_LOCO &&
			GetModelIndex() != TRAIN_FREIGHT_LOCO &&
			GetModelIndex() != TRAIN_TRAM) {
			_asm mov ecx, dwThisEntity
			_asm mov edx, [ecx] ; vtbl
			_asm push 0
			_asm push z
			_asm push y
			_asm push x
			_asm call dword ptr [edx+56] ; method 14
		} else {
			ScriptCommand(&put_train_at,m_dwGTAId,x,y,z);
		}
	}
}

//-----------------------------------------------------------
// MATCH
float CEntity::GetDistanceFromLocalPlayerPed()
{
	MATRIX4X4	matFromPlayer;
	MATRIX4X4	matThis;
	float		fSX,fSY,fSZ;

	CPlayerPed *pLocalPlayerPed = pGame->FindPlayerPed();
	CLocalPlayer *pLocalPlayer=NULL;

	if(!pLocalPlayerPed) return 10000.0f;

	GetMatrix(&matThis);

	if(pNetGame) {
		pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
		if(pLocalPlayer && (pLocalPlayer->IsSpectating() || pLocalPlayer->IsInRCMode())) {
			pGame->GetCamera()->GetMatrix(&matFromPlayer);
		} else {
			pLocalPlayerPed->GetMatrix(&matFromPlayer);
		}
	} else {
		pLocalPlayerPed->GetMatrix(&matFromPlayer);
	}

	fSX = (matThis.pos.X - matFromPlayer.pos.X) * (matThis.pos.X - matFromPlayer.pos.X);
	fSY = (matThis.pos.Y - matFromPlayer.pos.Y) * (matThis.pos.Y - matFromPlayer.pos.Y);
	fSZ = (matThis.pos.Z - matFromPlayer.pos.Z) * (matThis.pos.Z - matFromPlayer.pos.Z);

	return (float)sqrt(fSX + fSY + fSZ);
}

//-----------------------------------------------------------
// MATCH
float CEntity::GetDistanceFromCamera()
{
	if(!m_pEntity || m_pEntity->vtable == 0x863C40) return 100000.0f;

	MATRIX4X4	matThis;
	float		fSX,fSY,fSZ;

	GetMatrix(&matThis);
	fSX = (matThis.pos.X - *(float*)0xB6F9CC) * (matThis.pos.X - *(float*)0xB6F9CC);
	fSY = (matThis.pos.Y - *(float*)0xB6F9D0) * (matThis.pos.Y - *(float*)0xB6F9D0);
	fSZ = (matThis.pos.Z - *(float*)0xB6F9D4) * (matThis.pos.Z - *(float*)0xB6F9D4);

	return (float)sqrt(fSX + fSY + fSZ);
}

//-----------------------------------------------------------
// MATCH
float CEntity::Get2DDistanceFromLocalPlayerPed()
{
	MATRIX4X4	matFromPlayer;
	MATRIX4X4	matThis;
	float		fSX,fSY,fSZ;

	CPlayerPed *pLocalPlayerPed = pGame->FindPlayerPed();
	CLocalPlayer *pLocalPlayer=NULL;

	if(!pLocalPlayerPed) return 10000.0f;

	GetMatrix(&matThis);

	if(pNetGame) {
		pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
		if(pLocalPlayer && (pLocalPlayer->IsSpectating() || pLocalPlayer->IsInRCMode())) {
			pGame->GetCamera()->GetMatrix(&matFromPlayer);
		} else {
			pLocalPlayerPed->GetMatrix(&matFromPlayer);
		}
	} else {
		pLocalPlayerPed->GetMatrix(&matFromPlayer);
	}

	fSX = (matThis.pos.X - matFromPlayer.pos.X) * (matThis.pos.X - matFromPlayer.pos.X);
	fSY = (matThis.pos.Y - matFromPlayer.pos.Y) * (matThis.pos.Y - matFromPlayer.pos.Y);

	return (float)sqrt(fSX + fSY);
}

//-----------------------------------------------------------
// MATCH
float CEntity::GetDistanceFromPoint(float X, float Y, float Z)
{
	MATRIX4X4	matThis;
	float		fSX,fSY,fSZ;

	GetMatrix(&matThis);
	fSX = (matThis.pos.X - X) * (matThis.pos.X - X);
	fSY = (matThis.pos.Y - Y) * (matThis.pos.Y - Y);
	fSZ = (matThis.pos.Z - Z) * (matThis.pos.Z - Z);

	return (float)sqrt(fSX + fSY + fSZ);
}

//-----------------------------------------------------------
// MATCH
void CEntity::Add()
{
	// Check for CPlaceable messup
	if(!m_pEntity || m_pEntity->vtable == 0x863C40)
	{
#ifdef _DEBUG
		OutputDebugString("CEntity::Add - m_pEntity == NULL or CPlaceable");
#endif
		return;
	}

	if(!m_pEntity->dwUnkModelRel) {
		// Make sure the move/turn speed is reset

		VECTOR vec = {0.0f,0.0f,0.0f};

		SetMoveSpeedVector(vec);
		SetTurnSpeedVector(vec);

		WorldAddEntity((PDWORD)m_pEntity);

		MATRIX4X4 mat;
		GetMatrix(&mat);
		TeleportTo(mat.pos.X,mat.pos.Y,mat.pos.Z);

#ifdef _DEBUG
		if (!IsAdded())
		{
			OutputDebugString("CEntity::Add failed...");
		}
#endif
	}
}

//-----------------------------------------------------------
// MATCH
BOOL CEntity::IsAdded()
{
	// Check for CPlaceable messup
	if(m_pEntity) {
		if (m_pEntity->vtable == 0x863C40) 
			return FALSE;

		if(m_pEntity->dwUnkModelRel)
			return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------
// MATCH
void CEntity::Remove()
{
	// Check for CPlaceable messup
	if(!m_pEntity || m_pEntity->vtable == 0x863C40)
	{
#ifdef _DEBUG
		OutputDebugString("CEntity::Remove - m_pEntity == NULL or CPlaceable");
#endif
		return;
	}

	if(m_pEntity->dwUnkModelRel) {
		WorldRemoveEntity((PDWORD)m_pEntity);

#ifdef _DEBUG
		if (IsAdded())
		{
			OutputDebugString("CEntity::Remove failed...");
		}
#endif
	}
}

//-----------------------------------------------------------
// MATCH
BOOL CEntity::EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
	MATRIX4X4 matWorld;
	VECTOR vecMoveSpeed;

	if(!m_pEntity) return FALSE;

	GetMatrix(&matWorld);
	GetMoveSpeedVector(&vecMoveSpeed);

	if(matWorld.pos.X > fPX)
	{
		if(vecMoveSpeed.X != 0.0f) {
			vecMoveSpeed.X = -0.2f;
			vecMoveSpeed.Z = 0.1f;
		}
		SetMoveSpeedVector(vecMoveSpeed);
		matWorld.pos.Z += 0.04f;
		SetMatrix(matWorld);
		return TRUE;
	}
	else if(matWorld.pos.X < fZX)
	{
		if(vecMoveSpeed.X != 0.0f) {
			vecMoveSpeed.X = 0.2f;
			vecMoveSpeed.Z = 0.1f;
		}
		SetMoveSpeedVector(vecMoveSpeed);
		matWorld.pos.Z += 0.04f;
		SetMatrix(matWorld);
		return TRUE;
	}
	else if(matWorld.pos.Y > fPY)
	{
		if(vecMoveSpeed.Y != 0.0f) {
			vecMoveSpeed.Y = -0.2f;
			vecMoveSpeed.Z = 0.1f;
		}

		SetMoveSpeedVector(vecMoveSpeed);
		matWorld.pos.Z += 0.04f;
		SetMatrix(matWorld);
		return TRUE;
	}
	else if(matWorld.pos.Y < fNY)
	{
		if(vecMoveSpeed.Y != 0.0f) {
			vecMoveSpeed.Y = 0.2f;
			vecMoveSpeed.Z = 0.1f;
		}

		SetMoveSpeedVector(vecMoveSpeed);
		matWorld.pos.Z += 0.04f;
		SetMatrix(matWorld);
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------
// MATCH
BOOL CEntity::HasExceededWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
	MATRIX4X4 matWorld;

	if(!m_pEntity) return FALSE;

	GetMatrix(&matWorld);

	if(matWorld.pos.X > fPX) {
		return TRUE;
	}
	else if(matWorld.pos.X < fZX) {
		return TRUE;
	}
	else if(matWorld.pos.Y > fPY) {
		return TRUE;
	}
	else if(matWorld.pos.Y < fNY) {
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------
// MATCH
void CEntity::SetCollisionChecking(int iCheck)
{
	if(!m_pEntity) return;
	if(m_pEntity->vtable == 0x863C40) return;
	
	if(iCheck) {
		m_pEntity->dwProcessingFlags |= 1;
	} else {
		m_pEntity->dwProcessingFlags &= 0xFFFFFFFE;
	}
}

//-----------------------------------------------------------
// MATCH
BOOL CEntity::IsCollisionCheckingEnabled()
{
	if(m_pEntity && m_pEntity->vtable != 0x863C40)
	{
		return (BYTE)(m_pEntity->dwProcessingFlags & 1);
	}
	return TRUE;
}

//-----------------------------------------------------------
// MATCH
void CEntity::SetGravityProcessing(int iState)
{
	if(!m_pEntity) return;
	if(m_pEntity->vtable == 0x863C40) return;

	if(iState) {
		m_pEntity->dwProcessingFlags &= 0x7FFFFFFD;
	} else {		
		m_pEntity->dwProcessingFlags |= 0x80000002;
	}
}

//-----------------------------------------------------------
// MATCH
void CEntity::SetWaitingForCollision(int iState)
{
	if(!m_pEntity) return;
	if(m_pEntity->vtable == 0x863C40) return;

	if(iState) {
		m_pEntity->dwProcessingFlags |= 0x40000;
	} else {
		m_pEntity->dwProcessingFlags &= 0xFFFBFFFF;
		Add();
	}
}

//-----------------------------------------------------------
// MATCH
void CEntity::DisableStreaming()
{
	if(!m_pEntity) return;
	if(m_pEntity->vtable == 0x863C40) return;

	m_pEntity->dwProcessingFlags |= 0x80400;
}

//-----------------------------------------------------------
// MATCH
void CEntity::EnableTunnelTransition()
{
	if(!m_pEntity) return;
	if(m_pEntity->vtable == 0x863C40) return;

	m_pEntity->dwProcessingFlags |= 0x80000000;
}

//-----------------------------------------------------------
// MATCH
void CEntity::SetApplySpeed(int iState)
{
	if(!m_pEntity) return;
	if(m_pEntity->vtable == 0x863C40) return;

	DWORD dwEnt = (DWORD)m_pEntity;

	if(!iState) {
		_asm mov edx, dwEnt
		_asm mov eax, [edx+0x40]
		_asm or ah, 0x20
		_asm mov [edx+0x40], eax
	} else {
		_asm mov edx, dwEnt
		_asm mov eax, [edx+0x40]
		_asm and ah, 0xDF
		_asm mov [edx+0x40], eax
	}
}

//-----------------------------------------------------------






//-----------------------------------------------------------
// MATCH
BOOL CEntity::IsStationary()
{
	if (!IsAdded()) return FALSE; // movespeed vectors are invalid if its not added

    if( m_pEntity->vecMoveSpeed.X == 0.0f &&
		m_pEntity->vecMoveSpeed.Y == 0.0f &&
		m_pEntity->vecMoveSpeed.Z == 0.0f )
	{
		return TRUE;
	}
    return FALSE;
}

//-----------------------------------------------------------

