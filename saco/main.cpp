
#include "main.h"
#include "game/util.h"

int				iGtaVersion=0;

GAME_SETTINGS			tSettings;

CHAR					szArtworkProxy[MAX_PATH+1];
CHAR					szConfigFile[MAX_PATH+1];
CHAR					byte_1026E710[256]; // unused
CHAR					szChatLogFile[MAX_PATH+1];
CHAR					szSAMPDir[MAX_PATH+1];
CHAR					szCacheDir[MAX_PATH+1];

CConfig					*pConfig=0;
CChatWindow				*pChatWindow=0;
CCmdWindow				*pCmdWindow=0;
CDeathWindow			*pDeathWindow=0;
CAudioStream			*pAudioStream=0;
CSpawnScreen			*pSpawnScreen=0;
CNetGame				*pNetGame=0;
//DWORD					dword_1026EB98=0;
CFontRender				*pDefaultFont=0;
CUnkClass13				*pUnkClass13=0;
CUnkClass14				*pUnkClass14=0;
CUnkClass15				*pUnkClass15=0;

BOOL					bGameInited=FALSE;
BOOL					bNetworkInited=FALSE;

BOOL					bQuitGame=FALSE;
DWORD					dwStartQuitTick=0;

WORD					wVehicleComponentDebug=0;

IDirect3D9				*pD3D;
IDirect3DDevice9		*pD3DDevice	= NULL;

HANDLE			hInstance=0;
CNewPlayerTags			*pNewPlayerTags=NULL;
CScoreBoard				*pScoreBoard=NULL;
CUnkClass3 *pUnkClass3=NULL;
CUnkClass4 *pUnkClass4=NULL;
CUnkClass5 *pUnkClass5=NULL;
CLabel					*pLabel=NULL;
CUnkClass10 *pUnkClass10=NULL;
CUnkClass11 *pUnkClass11=NULL;
CUnkClass12 *pUnkClass12=NULL;
CNetStats				*pNetStats=NULL;
CSvrNetStats			*pSvrNetStats=NULL;
CHelpDialog				*pHelpDialog=NULL;
CUnkClass8 *pUnkClass8=NULL;

bool					bShowDebugLabels = false;

CGame					*pGame=0;
DWORD					dwGraphicsLoop=0;

CFileSystem *pFileSystem=NULL;





CDXUTDialogResourceManager	*pDialogResourceManager=NULL;
CDXUTDialog					*pGameUI=NULL;
CDXUTDialog					*pDXUTDialog1=NULL;
CDXUTDialog					*pDXUTDialog2=NULL;
CDXUTDialog					*pDXUTDialog3=NULL;
CDXUTDialog					*pDXUTDialog4=NULL;
CDXUTDialog					*pDXUTDialog5=NULL;

IDirect3DSurface9			*pD3DMouseSurface=NULL;

// forwards

BOOL SubclassGameWindow();
void SetupCommands();
void TheGraphicsLoop();
LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf);
void FUNC_1009DD50();

void SetupDialogResourceManager();
void SetupGameUI();

DWORD dwOrgRwSetState=0;
DWORD dwSetStateCaller=0;
DWORD dwSetStateOption=0;
DWORD dwSetStateParam=0;
char dbgstr[256];

// polls the game until it's able to run.
void LaunchMonitor(PVOID v)
{
	if(pGame)
		pGame->InitGame();

	while(1) {
		if(*(PDWORD)ADDR_ENTRY == 7) {
			FUNC_1009DD50();
			pGame->StartGame();
			break;
		}
		else {
			Sleep(5);
		}
	}

	ExitThread(0);
}

//----------------------------------------------------

#define ARCHIVE_FILE	"samp.saa"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if(DLL_PROCESS_ATTACH==fdwReason)
	{
		hInstance = hinstDLL;
		InitSettings();

		if(tSettings.bDebug || tSettings.bPlayOnline)
		{
			SetUnhandledExceptionFilter(exc_handler);
			dwGraphicsLoop = (DWORD)TheGraphicsLoop;

			CHAR szArchiveFile[MAX_PATH];
			GetModuleFileNameA((HMODULE)hInstance, szArchiveFile, MAX_PATH);
			DWORD dwFileNameLen = strlen(szArchiveFile);
			while(szArchiveFile[dwFileNameLen] != '\\')
				dwFileNameLen--;
			strcpy(szArchiveFile+dwFileNameLen+1, ARCHIVE_FILE);

			pFileSystem = new CArchiveFS();
			if(!pFileSystem->Load(ARCHIVE_FILE)) _asm int 3

			AddFontResourceA("gtaweap3.ttf");
			AddFontResourceA("sampaux3.ttf");

			InstallFileSystemHooks();
			InstallShowCursorHook();

			pGame = new CGame();

			_beginthread(LaunchMonitor,0,NULL);
		}
	}
	else if(DLL_PROCESS_DETACH==fdwReason)
	{
		if(tSettings.bDebug || tSettings.bPlayOnline) {
			UninstallFileSystemHooks();
		}
	}

	return TRUE;
}

//----------------------------------------------------

PCHAR GetSAMPPath()
{
	return szSAMPDir;
}

//----------------------------------------------------

PCHAR GetCachePath()
{
	return szCacheDir;
}

//----------------------------------------------------

void SetupCacheDirectories()
{
	ZeroMemory(szCacheDir, sizeof(szCacheDir));

	CHAR szPath[MAX_PATH+1];
	DWORD cbData = MAX_PATH;
	HKEY hKey;
	DWORD dwType;
	ZeroMemory(szPath, sizeof(szPath));

	sprintf(szCacheDir, "%s\\cache", szSAMPDir);
	if(RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\SAMP", 0, KEY_READ, &hKey) == ERROR_SUCCESS &&
		RegQueryValueEx(hKey, "model_cache", NULL, &dwType, (LPBYTE) szPath, &cbData) == ERROR_SUCCESS)
	{
		strncpy(szCacheDir, szPath, MAX_PATH+1);
	}
	if(!IsFileOrDirectoryExists(szCacheDir))
		CreateDirectoryA(szCacheDir, NULL);

	CHAR szLocalDir[MAX_PATH+1];
	sprintf(szLocalDir, "%s\\local", szCacheDir);
	if(!IsFileOrDirectoryExists(szLocalDir))
		CreateDirectoryA(szLocalDir, NULL);
}

//----------------------------------------------------

PCHAR GetArtworkProxy()
{
	ZeroMemory(szArtworkProxy, sizeof(szArtworkProxy));

	DWORD cbData = MAX_PATH;
	HKEY hKey;
	DWORD dwType;

	if(RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\SAMP", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
		if(RegQueryValueEx(hKey, "artwork_proxy", NULL, &dwType, (LPBYTE) szArtworkProxy, &cbData) == ERROR_SUCCESS) {
			return szArtworkProxy;
		} else {
			return NULL;
		}
	} else {
		return NULL;
	}
}

//----------------------------------------------------

void SetupDirectories()
{
	ZeroMemory(szSAMPDir, sizeof(szSAMPDir));

	if(strlen((char*)0xC92368) == 0) {
		GetCurrentDirectory(MAX_PATH+1, szSAMPDir);
	} else {
		sprintf(szSAMPDir, "%s\\SAMP", (char*)0xC92368);
		if(!IsFileOrDirectoryExists(szSAMPDir))
			CreateDirectoryA(szSAMPDir, NULL);

		CHAR szScreensDir[MAX_PATH+1];
		sprintf(szScreensDir, "%s\\screens", szSAMPDir);
		if(!IsFileOrDirectoryExists(szScreensDir))
			CreateDirectoryA(szScreensDir, NULL);

		SetupCacheDirectories();
	}
}

//----------------------------------------------------

DWORD dwFogEnabled = 0;
DWORD dwFogColor = 0x00FF00FF;
BOOL gDisableAllFog = FALSE;

void SetupD3DFog(BOOL bEnable)
{
	float fFogStart = 500.0f;
	float fFogEnd = 700.0f;

	if(gDisableAllFog) bEnable = FALSE;

	if(pD3DDevice) {
		pD3DDevice->SetRenderState(D3DRS_FOGENABLE, bEnable);
		//pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, dwFogColor);
		pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
		pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		//pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fFogStart));
		//pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fFogEnd));
	}
}

//----------------------------------------------------

void _declspec(naked) RwRenderStateSetHook()
{
	_asm mov eax, [esp]
	_asm mov dwSetStateCaller, eax
	_asm mov eax, [esp+4]
	_asm mov dwSetStateOption, eax
	_asm mov eax, [esp+8]
	_asm mov dwSetStateParam, eax

	if(dwSetStateOption == 14) {
		if(dwSetStateParam) {
			SetupD3DFog(TRUE);
			dwFogEnabled = 1;
		} else {
			SetupD3DFog(FALSE);
			dwFogEnabled = 0;
		}
		_asm mov [esp+8], 0 ; no fog
	}

	_asm mov eax, dwOrgRwSetState
	_asm jmp eax
}

//----------------------------------------------------

void HookRwRenderStateSet()
{
	DWORD dwNewRwSetState = (DWORD)RwRenderStateSetHook;

	_asm mov ebx, 0xC97B24
	_asm mov eax, [ebx]
	_asm mov edx, [eax+32]
	_asm mov dwOrgRwSetState, edx
	_asm mov edx, dwNewRwSetState
	_asm mov [eax+32], edx

#ifdef _DEBUG
	sprintf(dbgstr,"HookRwRenderStateSet(0x%X)",dwOrgRwSetState);
	OutputDebugString(dbgstr);
#endif
}

//----------------------------------------------------

void CallRwRenderStateSet(int state, int option)
{
	_asm push option
	_asm push state
	_asm mov ebx, 0xC97B24
	_asm mov eax, [ebx]
	_asm call dword ptr [eax+32]
	_asm add esp, 8
}

//----------------------------------------------------

void CallRwRenderStateGet(int state, int *option)
{
	_asm push option
	_asm push state
	_asm mov ebx, 0xC97B24
	_asm mov eax, [ebx]
	_asm call dword ptr [eax+36]
	_asm add esp, 8
}

//----------------------------------------------------

void DoInitStuff()
{
	// GAME INIT
	if(!bGameInited)
	{
		SetupDirectories();

		sprintf(szConfigFile, "%s\\sa-mp.cfg", szSAMPDir);
		sprintf(szChatLogFile, "%s\\chatlog.txt", szSAMPDir);

		pConfig = new CConfig(szConfigFile);

		timeBeginPeriod(1); // increases the accuracy of Sleep()
		FUNC_1009DD50();
		SubclassGameWindow();

		// Grab the real IDirect3D9 * from the game.
		pD3D = (IDirect3D9 *)pGame->GetD3D();

		// Grab the real IDirect3DDevice9 * from the game.
		pD3DDevice = (IDirect3DDevice9 *)pGame->GetD3DDevice();
		*(IDirect3DDevice9Hook**)ADDR_ID3D9DEVICE = new IDirect3DDevice9Hook();

		pD3DDevice->ShowCursor(FALSE);

		// Create instances of the chat and input classes.
		pDefaultFont = new CFontRender(pD3DDevice);
		pChatWindow = new CChatWindow(pD3DDevice,pDefaultFont,szChatLogFile);
		pCmdWindow = new CCmdWindow(pD3DDevice);



		// TODO: DoInitStuff()

		bGameInited = TRUE;

		return;
	}

	// NET GAME INIT
	if(!bNetworkInited && tSettings.bPlayOnline) {

		pNetGame = new CNetGame(tSettings.szConnectHost,atoi(tSettings.szConnectPort),
				tSettings.szNickName,tSettings.szConnectPass);

		bNetworkInited = TRUE;
		return;
	}
}

//----------------------------------------------------

void DoProcessStuff()
{
	DoInitStuff();

	SetupD3DFog(TRUE);

	// Process the netgame if it's active.
	if(pNetGame) {
		//Sleep(0); // This hands the context over to raknet
		pNetGame->Process();
	}

	if(pAudioStream) {
		pAudioStream->Process();
	}
}

//----------------------------------------------------

void TheGraphicsLoop()
{
	_asm pushad // because we're called from a hook

	DoProcessStuff();

	_asm popad
}

//----------------------------------------------------

void QuitGame()
{
	if(pNetGame && pNetGame->GetGameState() == GAMESTATE_CONNECTED) {
		pNetGame->GetRakClient()->Disconnect(500);
	}
	bQuitGame = TRUE;
	dwStartQuitTick = GetTickCount();
}

//----------------------------------------------------

void InitSettings()
{
	PCHAR szCmdLine = GetCommandLineA();

	memset(&tSettings,0,sizeof(GAME_SETTINGS));

	while(*szCmdLine) {

		if(*szCmdLine == '-' || *szCmdLine == '/') {
			szCmdLine++;
			switch(*szCmdLine) {
				case 'd':
					tSettings.bDebug = TRUE;
					tSettings.bPlayOnline = FALSE;
					break;
				case 'c':
					tSettings.bPlayOnline = TRUE;
					tSettings.bDebug = FALSE;
					break;
				case 'z':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectPass);
					break;
				/*
				// We'll do this using ALT+ENTER
				case 'w':
					tSettings.bWindowedMode = TRUE;
					break;
				*/
				case 'h':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectHost);
					break;
				case 'p':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectPort);
					break;
				case 'n':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szNickName);
					break;
				case 'l':
					szCmdLine++;
					SetStringFromQuotedCommandLine(szCmdLine,tSettings.szDebugScript);
					break;
			}
		}

		szCmdLine++;
	}
}

//----------------------------------------------------

void SetStringFromCommandLine(char *szCmdLine, char *szString)
{
	while(*szCmdLine == ' ') szCmdLine++;
	while(*szCmdLine &&
		  *szCmdLine != ' ' &&
		  *szCmdLine != '-' &&
		  *szCmdLine != '/')
	{
		*szString = *szCmdLine;
		szString++; szCmdLine++;
	}
	*szString = '\0';
}

//----------------------------------------------------

void SetStringFromQuotedCommandLine(char *szCmdLine, char *szString)
{
	while(*szCmdLine != '"') szCmdLine++;
	szCmdLine++;
	while(*szCmdLine &&
		  *szCmdLine != '"')
	{
		*szString = *szCmdLine;
		szString++; szCmdLine++;
	}
	*szString = '\0';
}

//----------------------------------------------------

void d3d9DestroyDeviceObjects()
{
	// TOOD: d3d9DestroyDeviceObjects
}

void d3d9RestoreDeviceObjects()
{
	// TODO: d3d9RestoreDeviceObjects
}

//----------------------------------------------------

float GetElapsedTime()
{
	static BOOL bTimerInit = false;
	static BOOL bUsingOPF  = false;
	static LONGLONG nTicksPerSec = 0;

	if (!bTimerInit)
	{
		bTimerInit = true;
		LARGE_INTEGER qwTicksPerSec;
		bUsingOPF = QueryPerformanceFrequency(&qwTicksPerSec);
		if (bUsingOPF) nTicksPerSec = qwTicksPerSec.QuadPart;
	}

	if (bUsingOPF)
	{
		LARGE_INTEGER qwTime;
		QueryPerformanceCounter(&qwTime);
		static LONGLONG llLastTime = qwTime.QuadPart;
		double fElapsedTime = (double)(qwTime.QuadPart - llLastTime) / (double) nTicksPerSec;
		llLastTime = qwTime.QuadPart;
		return (float)fElapsedTime;
	} else {
		double fTime = timeGetTime() * 0.001;
		static double fLastTime = fTime;
		double fElapsedTime = (double)(fTime - fLastTime);
		fLastTime = fTime;
		return (float)fElapsedTime;
	}
}

//----------------------------------------------------

void DisableMouseProcess();
void DIResetMouse();
void UpdatePads();

DWORD GLOBAL_1026EBD0;

DWORD __stdcall FUNC_100C4530(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	if(pNetGame && pGame) {
		UnFuck(0x541DF5,5);
		memset((PVOID)0x541DF5,0x90,5);

		DisableMouseProcess();
		DIResetMouse();
		UpdatePads();

		UnFuck(0x6194A0,1);
		*(BYTE*)0x6194A0 = 0xC3;

		GLOBAL_1026EBD0++;
	}
	return 69;
}

//----------------------------------------------------

DWORD GLOBAL_10117478 = 69;
DWORD GLOBAL_1026EBE8;
DWORD GLOBAL_1026EBEC;
DWORD GLOBAL_1026EBF0;
DWORD GLOBAL_1014FD98;

int FUNC_100C46B0(bool a1)
{
	if(GLOBAL_10117478 == 69)
	{
		GLOBAL_1026EBEC = GLOBAL_1014FD98;
		GLOBAL_1026EBE8 = GLOBAL_1014FD98;
		GLOBAL_10117478 = 70;
	}
	else
	{
		if(a1)
		{
			if(GLOBAL_1014FD98 <= GLOBAL_1026EBEC && FUNC_100C4530(59, 1, 6, 12, 17, 2, 50) != 69)
				exit(0);
			if(GLOBAL_1026EBE8 >= GLOBAL_1014FD98 && FUNC_100C4530(60, 1, 6, 12, 17, 2, 50) != 69)
				exit(0);
			GLOBAL_1026EBE8 = GLOBAL_1014FD98;
		}
		if(GLOBAL_1026EBD0 > 5 && FUNC_100C4530(65, 1, 6, 12, 17, 2, 50) != 69)
			exit(0);
	}
	GLOBAL_1026EBF0++;
	return GLOBAL_1026EBF0;
}

//----------------------------------------------------

int GetFontSize()
{
	int size;

	if (pGame->GetScreenWidth() < 1024)
		size = 14;
	else if (pGame->GetScreenWidth() < 1400)
		size = 16;
	else if (pGame->GetScreenWidth() < 1600)
		size = 18;
	else
		size = 20;

	return size + 2 * pConfig->GetIntVariable("fontsize");
}

int GetDeathWindowFontSize()
{
	int size = 14;
	if (pGame->GetScreenWidth() < 1024)
		size = 12;

	int fontsize = size + 2 * pConfig->GetIntVariable("fontsize");
	if(fontsize < size)
		fontsize = size;
	return fontsize;
}

int GetUIFontSize()
{
	return 2 * pConfig->GetIntVariable("fontsize") + 20;
}

int GetFontWeight()
{
	int fontweight = pConfig->GetIntVariable("fontweight");
	if(fontweight == 0) return FW_BOLD;

	return fontweight != 1 ? FW_BOLD : FW_NORMAL;
}

char *GetFontFace()
{
	if(pConfig && pConfig->GetStringVariable("fontface"))
	{
		return pConfig->GetStringVariable("fontface");
	}
	return "Arial";
}

void SetupDialogResourceManager()
{
	pDialogResourceManager = new CDXUTDialogResourceManager();
	pDialogResourceManager->OnCreateDevice(pD3DDevice);
	pDialogResourceManager->OnResetDevice();
}

void CALLBACK OnDialogEvent3( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	// TODO: OnDialogEvent3
}

void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	// TODO: OnGUIEvent
}

void CALLBACK OnDialogEvent1( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	// TODO: OnDialogEvent1
}

void CALLBACK OnDialogEvent2( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	// TODO: OnDialogEvent2
}

void CALLBACK OnDialogEvent4( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	// nothing
}

void CALLBACK OnDialogEvent5( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	// nothing
}

void SetupGameUI()
{
	SAFE_DELETE(pDXUTDialog1);
	SAFE_DELETE(pGameUI);
	SAFE_RELEASE(pD3DMouseSurface);
	SAFE_DELETE(pDXUTDialog2);
	SAFE_DELETE(pDXUTDialog3);
	SAFE_DELETE(pDXUTDialog4);
	SAFE_DELETE(pDXUTDialog5);

	pDXUTDialog1 = new CDXUTDialog();
	pDXUTDialog1->Init(pDialogResourceManager);
	pDXUTDialog1->SetCallback( OnDialogEvent1 );
	pDXUTDialog1->SetSize(640,480);
	pDXUTDialog1->SetLocation(0,0);
	pDXUTDialog1->SetBackgroundColors(D3DCOLOR_ARGB( 150, 10, 10, 10 ));
	pDXUTDialog1->EnableMouseInput(true);
	pDXUTDialog1->EnableKeyboardInput(true);
	pDXUTDialog1->SetVisible(false);
	if(pScoreBoard) pScoreBoard->ResetDialogControls(pDXUTDialog1);

	pGameUI = new CDXUTDialog();
	pGameUI->Init(pDialogResourceManager);
	pGameUI->SetCallback( OnGUIEvent );
	pGameUI->SetLocation(0,0);
	pGameUI->SetSize(pGame->GetScreenWidth(),pGame->GetScreenHeight());
	pGameUI->EnableMouseInput(true);
	pGameUI->EnableKeyboardInput(true);
	
	pDXUTDialog2 = new CDXUTDialog();
	pDXUTDialog2->Init(pDialogResourceManager);
	pDXUTDialog2->SetCallback( OnDialogEvent2 );
	pDXUTDialog2->SetLocation(0,0);
	pDXUTDialog2->SetSize(310,40);
	pDXUTDialog2->SetBackgroundColors(D3DCOLOR_ARGB( 150, 10, 10, 10 ));
	pDXUTDialog2->EnableMouseInput(true);
	pDXUTDialog2->EnableKeyboardInput(false);
	pDXUTDialog2->SetVisible(false);
	pDXUTDialog2->AddButton(6, "<<", 10, 5, 90, 30);
	pDXUTDialog2->AddButton(5, ">>", 110, 5, 90, 30);
	pDXUTDialog2->AddButton(4, "Spawn", 210, 5, 90, 30);
	if(pChatWindow) pChatWindow->ResetDialogControls(pGameUI);
	if(pCmdWindow) pCmdWindow->ResetDialogControls(pGameUI);

	pDXUTDialog3 = new CDXUTDialog();
	pDXUTDialog3->Init(pDialogResourceManager);
	pDXUTDialog3->SetCallback(OnDialogEvent3);
	pDXUTDialog3->SetLocation(0,0);
	pDXUTDialog3->SetSize(600,300);
	pDXUTDialog3->EnableMouseInput(true);
	pDXUTDialog3->EnableKeyboardInput(true);
	pDXUTDialog3->SetBackgroundColors(D3DCOLOR_ARGB( 220, 5, 5, 5 ));
	pDXUTDialog3->SetVisible(false);
	if(pUnkClass3) pUnkClass3->ResetDialogControls(pDXUTDialog3);

	pDXUTDialog4 = new CDXUTDialog();
	pDXUTDialog4->Init(pDialogResourceManager);
	pDXUTDialog4->SetCallback(OnDialogEvent4);
	pDXUTDialog4->SetLocation(0,0);
	pDXUTDialog4->SetSize(500,160);
	pDXUTDialog4->EnableMouseInput(true);
	pDXUTDialog4->EnableKeyboardInput(true);
	pDXUTDialog4->SetBackgroundColors(D3DCOLOR_ARGB( 220, 5, 5, 5 ));
	pDXUTDialog4->SetVisible(false);
	if(pUnkClass4) pUnkClass4->ResetDialogControls(pDXUTDialog4);

	pDXUTDialog5 = new CDXUTDialog();
	pDXUTDialog5->Init(pDialogResourceManager);
	pDXUTDialog5->SetCallback(OnDialogEvent5);
	pDXUTDialog5->SetLocation(0,0);
	pDXUTDialog5->SetSize(500,160);
	pDXUTDialog5->EnableMouseInput(true);
	pDXUTDialog5->EnableKeyboardInput(false);
	pDXUTDialog5->SetBackgroundColors(D3DCOLOR_ARGB( 220, 5, 5, 5 ));
	pDXUTDialog5->SetVisible(false);
	if(pUnkClass5) pUnkClass5->ResetDialogControls(pDXUTDialog5);

	if(!pD3DMouseSurface)
	{
		pD3DDevice->CreateOffscreenPlainSurface(32,32,D3DFMT_A8R8G8B8,
			D3DPOOL_SYSTEMMEM,&pD3DMouseSurface,NULL);

		D3DXLoadSurfaceFromFile(pD3DMouseSurface,NULL,NULL,
			"mouse.png",NULL,D3DX_FILTER_NONE,0,NULL);

		if(pD3DMouseSurface)
			pD3DDevice->SetCursorProperties(0,0,pD3DMouseSurface);
	}
}

