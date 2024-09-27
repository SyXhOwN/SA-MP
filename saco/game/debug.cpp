
#include "../main.h"
#include "keystuff.h"

extern CChatWindow   *pChatWindow;
extern CGame		 *pGame;

int		iGameDebugType=0;
DWORD	dwDebugEntity1=0;
DWORD	dwDebugEntity2=0;

//----------------------------------------------------------
// Switches on the debug screen for debugging raw vehicles/actors

// MATCH
void GameDebugEntity(DWORD dwEnt1, DWORD dwEnt2, int type)
{
	iGameDebugType=type;
	dwDebugEntity1=dwEnt1;
	dwDebugEntity2=dwEnt2;
}

//----------------------------------------------------------
// Switches off any driver debug screen or sync sequence

// MATCH
void GameDebugScreensOff()
{
	iGameDebugType=0;
}

//----------------------------------------------------------

BOOL bSelVehicleInit=FALSE;
CVehicle *pVehicle=NULL;
int iSelection = 400;
GTA_CONTROLSET *pControls;
CCamera *pCam;

void GameBuildRecreateVehicle()
{
	if(pVehicle) delete pVehicle;

	pVehicle = pGame->NewVehicle(iSelection,5.0f,5.0f,500.0f,0.0f,NULL);
}

void GameBuildSelectVehicle()
{
	if(!bSelVehicleInit) {
		pControls = GameGetInternalKeys();
		pCam = pGame->GetCamera();
		pCam->SetPosition(-4.0f,-4.0f,502.0f,0.0f,0.0f,0.0f);
		pCam->LookAtPoint(5.0f,5.0f,500.0f,1);
		pGame->FindPlayerPed()->TogglePlayerControllable(0);
		pGame->DisplayGameText("Vehicle Select",4000,6);
		GameBuildRecreateVehicle();
		bSelVehicleInit = TRUE;
	}

	pGame->DisplayHud(FALSE);

	if(pVehicle && pVehicle->m_pEntity) {
		VECTOR vecTurn = { 0.0f, 0.0f, 0.03f };
		VECTOR vecMove = { 0.0f, 0.0f, 0.0f };
		pVehicle->SetTurnSpeedVector(vecTurn);
		pVehicle->SetMoveSpeedVector(vecMove);

		MATRIX4X4 mat;
		pVehicle->GetMatrix(&mat);
		mat.pos.X = 5.0f;
		mat.pos.Y = 5.0f;
		mat.pos.Z = 500.0f;
		pVehicle->SetMatrix(mat);
	}

	if(pControls->wKeys1[14] && !pControls->wKeys2[14]) {
		iSelection--;
		if(iSelection==538) iSelection-=2; // trains
		if(iSelection==399) iSelection=611;
		GameBuildRecreateVehicle();
		return;
	}

	if(pControls->wKeys1[16] && !pControls->wKeys2[16]) {
		iSelection++;
		if(iSelection==537) iSelection+=2;  // trains
		if(iSelection==612) iSelection=400;
		GameBuildRecreateVehicle();
		return;
	}

	if(pControls->wKeys1[15] && !pControls->wKeys2[15]) {

		delete pVehicle;
		pVehicle = NULL;

		pCam->SetBehindPlayer();
		GameDebugEntity(0,0,0);

		// place this vehicle near the player.
		CPlayerPed *pPlayer = pGame->FindPlayerPed();

		if(pPlayer)
		{
			MATRIX4X4 matPlayer;
			pPlayer->GetMatrix(&matPlayer);
			CHAR blank[9] = "";
			sprintf(blank, "TYPE_%d", iSelection);
			CVehicle *pTestVehicle = pGame->NewVehicle(iSelection,
				(matPlayer.pos.X - 5.0f), (matPlayer.pos.Y - 5.0f),
				matPlayer.pos.Z+1.0f,0.0f,NULL);

			if(pTestVehicle) {
				pPlayer->PutDirectlyInVehicle(pTestVehicle->m_dwGTAId,0);
			}
		}

		pCam->Restore();
		pCam->SetBehindPlayer();
		pGame->FindPlayerPed()->TogglePlayerControllable(1);
		pGame->DisplayHud(TRUE);
		bSelVehicleInit=FALSE;

		return;
	}
}

//----------------------------------------------------------

VECTOR vecSpawnPoint;
CObjectPool *pObjectPool=NULL;

// MATCH
char* GetFunctionParams(char *szScriptLine)
{
	char *szParamsStart = strchr(szScriptLine, '(');
	char *szParamsEnd = strchr(szScriptLine, ')');

	if(szParamsStart && szParamsEnd)
	{
		strncpy(szScriptLine, szParamsStart + 1, szParamsEnd - szParamsStart - 1);
		szScriptLine[szParamsEnd - szParamsStart] = 0;
		return szScriptLine;
	}
	return NULL;
}

void GameDebugProcessScriptLine(char *szScriptLine)
{
	if(!strncmp(szScriptLine, "CreateObject", 12))
	{

	}
	else if(!strncmp(szScriptLine, "CreateVehicle", 13) || !strncmp(szScriptLine, "AddStaticVehicle", 16))
	{

	}
	else if (!strncmp(szScriptLine, "SetPlayerCameraPos", 18))
	{

	}
	else if(!strncmp(szScriptLine, "SetPlayerCameraLookAt", 21))
	{

	}
	else if(!strncmp(szScriptLine, "RemoveBuildingForPlayer", 23))
	{

	}
	else if (!strncmp(szScriptLine, "SetPlayerInterior", 17))
	{

	}
}

void GameDebugLoadScript(char *szFileName)
{
	pGame->SetWorldTime(12, 0);
	pChatWindow->AddDebugMessage("DEBUGSCRIPT: Loading %s", szFileName);

	pObjectPool = new CObjectPool();

	FILE *pScriptFile = fopen(szFileName, "r");
	if(pScriptFile) {
		while(!feof(pScriptFile)) {
			char szScriptLine[257];
			fgets(szScriptLine,256,pScriptFile);
			GameDebugProcessScriptLine(szScriptLine);
		}
		fclose(pScriptFile);

		CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
		if(pPlayerPed) {
			pPlayerPed->TeleportTo(vecSpawnPoint.X, vecSpawnPoint.Y, vecSpawnPoint.Z);
		}
	} else {
		pChatWindow->AddDebugMessage("DEBUGSCRIPT: I can't open %s", szFileName);
	}
}

//----------------------------------------------------------
