
#include "main.h"

extern CGame *pGame;

VECTOR g_vecStreamingPos;

//----------------------------------------------------

BOOL CAudioStream::Initialize()
{
	// TODO: CAudioStream::Initialize()
	return FALSE;
}

//----------------------------------------------------

BOOL CAudioStream::Stop(bool bWaitForThread)
{
	// TODO: CAudioStream::Stop()
	return FALSE;
}

//----------------------------------------------------

void CAudioStream::DoMeta()
{
	// TODO: CAudioStream::DoMeta
}

//----------------------------------------------------

void CALLBACK CAudioStream::MetaSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	DoMeta();
}

//----------------------------------------------------

void CAudioStream::ProcessThread(PVOID v)
{
	// TODO: CAudioStream::ProcessThread
}

//----------------------------------------------------

BOOL CAudioStream::Play(char *szURL, float fX, float fY, float fZ, float fDistance, bool bUsePos)
{
	// TODO: CAudioStream::Play
	return FALSE;
}

//----------------------------------------------------

void CAudioStream::HandleGameAudio()
{
	// TODO: CAudioStream::HandleGameAudio()
}

//----------------------------------------------------

void CAudioStream::Draw()
{
	// TODO: CAudioStream::Draw()
}

//----------------------------------------------------

void FUNC_10066B90(PVECTOR in, PVECTOR out)
{
	out->X = -in->X;
	out->Y = in->Z;
	out->Z = in->Y;
}

void FUNC_10066BB0(PVECTOR in, PVECTOR out)
{
	out->X = in->X;
	out->Y = -in->Z;
	out->Z = -in->Y;
}

void CAudioStream::Test3D()
{
	CPlayerPed* pPlayerPed = pGame->FindPlayerPed();
	if(!pPlayerPed) return;

	CAMERA_AIM *Aim = pPlayerPed->GetCurrentAim();
	VECTOR vecResult;
	VECTOR vecFront;
	VECTOR vecPos;
	BASS_3DVECTOR front;
	BASS_3DVECTOR vel;

    vecFront.X = Aim->f1x;
    vecFront.Y = Aim->f1y;;
    vecFront.Z = Aim->f1z;
    vecPos.X = Aim->pos1x;
    vecPos.Y = Aim->pos1y;
    vecPos.Z = Aim->pos1z;

	FUNC_10066BB0(&vecFront, &vecResult);

	front.x = vecResult.X;
	front.y = vecResult.Y;
	front.z = vecResult.Z;

	FUNC_10066B90(&vecPos, &vecResult);

	vecFront.X = vecResult.X;
	vecFront.Y = vecResult.Y;
	vecFront.Z = vecResult.Z;

	FUNC_10066B90(&g_vecStreamingPos, &vecResult);
}

//----------------------------------------------------
