
#pragma once

#include <stdio.h>
#include <windows.h>
#include <process.h>

#define SAFE_DELETE(p)	{ if (p) { delete (p); (p) = NULL; } }

#define IDC_CMDEDIT		1

#define MAX_PLAYER_NAME			24
#define MAX_SETTINGS_STRING		256

#define GTASA_VERSION_UNKNOWN	0
#define GTASA_VERSION_USA10		1
#define GTASA_VERSION_EU10		2
#define CSCANNER_DISTANCE		200.0f

typedef struct _GAME_SETTINGS {
	BOOL bDebug;
	BOOL bPlayOnline;
	BOOL bWindowedMode;
	CHAR szConnectPass[MAX_SETTINGS_STRING+1];
	CHAR szConnectHost[MAX_SETTINGS_STRING+1];
	CHAR szConnectPort[MAX_SETTINGS_STRING+1];
	CHAR szNickName[MAX_SETTINGS_STRING+1];
	CHAR szDebugScript[MAX_SETTINGS_STRING+1];
} GAME_SETTINGS;

#include "d3dhook/IDirect3DDevice9Hook.h"
#include "d3dhook/ID3DXFontHook.h"

#include "d3d9/include/d3d9.h"
#include "d3d9/include/d3dx9core.h"
#include "d3d9/common/dxstdafx.h"
#include "game/game.h"

#include "../raknet/RakClientInterface.h"
#include "../raknet/RakNetworkFactory.h"
#include "../raknet/BitStream.h"
#include "../raknet/PacketEnumerations.h"
#include "../raknet/SAMPRPC.h"
#include "../raknet/GetTime.h"

#include "net/localplayer.h"
#include "net/remoteplayer.h"
#include "net/netplayer.h"
#include "net/netrpc.h"
#include "net/actorpool.h"
#include "net/playerpool.h"
#include "net/vehiclepool.h"
#include "net/pickuppool.h"
#include "net/objectpool.h"
#include "net/gangzonepool.h"
#include "net/menupool.h"
#include "net/textdrawpool.h"
#include "net/labelpool.h"
#include "net/netgame.h"
#include "net/scriptrpc.h"

#include "config.h"
#include "filehooks.h"
#include "fontrender.h"
#include "chatwindow.h"
#include "cmdwindow.h"
#include "deathwindow.h"
#include "spawnscreen.h"
#include "playertags.h"
#include "newplayertags.h"
#include "scoreboard.h"
#include "label.h"
#include "netstats.h"
#include "svrnetstats.h"
#include "helpdialog.h"
#include "audiostream.h"
#include "archive/ArchiveFS.h"

#include "unkclass3.h"
#include "unkclass4.h"
#include "unkclass5.h"
#include "unkclass8.h"
#include "unkclass10.h"
#include "unkclass11.h"
#include "unkclass12.h"
#include "unkclass13.h"
#include "unkclass14.h"
#include "unkclass15.h"

void SetStringFromCommandLine(char *szCmdLine, char *szString);
void SetStringFromQuotedCommandLine(char *szCmdLine, char *szString);
void InitSettings();

void UnFuck(DWORD addr, int size);

#include "outputdebugstring.h"

//----------------------------------------------------
// EOF
