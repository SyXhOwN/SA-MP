
#pragma once

// size must be exactly 68 bytes
typedef struct _MODEL_INFO_TYPE {

	DWORD vtable; // 0-4

	char _gap4[24];

	DWORD *pdwRenderWare; // 28-32

	char _gap20[4];

	DWORD dwAnimType; // 36-40
	DWORD dwPedType; // 40-44 
	DWORD dwStatType; // 44-48

	char _pad30[20];

} MODEL_INFO_TYPE;

BOOL RelocateModelInfoHack();
BOOL InitModelInfoHack();
MODEL_INFO_TYPE* __stdcall GetModelInfo(int iModelID);
void FreeModelInfo(int iModelID); // unused
