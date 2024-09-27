
#pragma once

void ProcessLineOfSight(VECTOR *vecOrigin, VECTOR *vecLine, VECTOR *colPoint,
		DWORD *pHitEntity, int bCheckBuildings, int bCheckVehicles, int bCheckPeds,
		int bCheckObjects, int bCheckDummies, int bSeeThroughStuff,
		int  bIgnoreSomeObjectsForCamera, int bUnk1);

float FloatDifference(float f1, float f2);
float FloatOffset(float f1, float f2);

void __stdcall SetRadarColor(int nIndex,DWORD dwColor);

void __stdcall WorldRemoveEntity(DWORD *dwEnt);
void __stdcall WorldAddEntity(DWORD *dwEnt);

int __stdcall GameGetWeaponModelIDFromWeaponID(int iWeaponID);
void __stdcall GameDisableCheatCodes();

PED_TYPE * __stdcall GamePool_Ped_GetAt(int iID);
int __stdcall GamePool_Ped_GetIndex(PED_TYPE *pActor);
VEHICLE_TYPE * __stdcall GamePool_Vehicle_GetAt(int iID);
DWORD __stdcall GamePool_Vehicle_GetIndex(VEHICLE_TYPE *pVehicle);
ENTITY_TYPE * __stdcall GamePool_Object_GetAt(int iID);
PED_TYPE * __stdcall GamePool_FindPlayerPed();

DWORD __stdcall TranslateColorCodeToRGBA(int iCode);
BOOL __stdcall GameIsEntityOnScreen(DWORD * pdwEnt);
void __stdcall InitPlayerPedPtrRecords();
void __stdcall SetPlayerPedPtrRecord(BYTE bytePlayer, DWORD dwPedPtr);
DWORD __stdcall GetPlayerPedPtrRecord(BYTE bytePlayer);
BYTE __stdcall FindPlayerNumFromPedPtr(DWORD dwPedPtr);

DWORD __stdcall CRC32FromUpcaseString(char *szString);

void GameResetRadarColors();

float DegToRad(float fDegrees);
float DegToRadInv(float fDegrees);

bool IsNumeric(char *szString);

void SaveCameraRaster(char *filename);

BOOL IsFileOrDirectoryExists(char * szPath);

DWORD FUNC_100B6100(char *szString, int nMaxLen);

bool unnamed_100B4B50(VECTOR *vecPos);

BUILDING_TYPE * GamePool_GetBuilding();
DUMMY_TYPE * GamePool_GetDummy();
OBJECT_TYPE * GamePool_GetObject();

void ReplaceBuildingModel(ENTITY_TYPE *pEntity, int iModelID);
int GetInvalidModelInfoCount();

bool HasCollisionLoaded(VECTOR *vec);
BOOL __stdcall IsATrainPart(ENTITY_TYPE *pEntity);

BOOL IsHexChar(char c);
BOOL IsHexChar(wchar_t c);
DWORD GetColorFromEmbedCode(char *szString);
DWORD GetColorFromEmbedCode(wchar_t *szString);
void RemoveColorEmbedsFromString(char *szString);

UINT GetVehicleSubtypeFromVehiclePtr(VEHICLE_TYPE *pVehicle);

unsigned short __stdcall GetModelReferenceCount(int nModelIndex);

DWORD * GetNextTaskFromTask(DWORD *task);
int GetTaskTypeFromTask(DWORD *task);

DWORD *__stdcall GetModelRwObject(int iModelID);

