
#include "../main.h"
#include "util.h"
#include "keystuff.h"
#include "aimstuff.h"

char *szGameTextMessage;

int unnamed_10150340[210];

BOOL ApplyPreGamePatches();

CGame::CGame()
{
	// TODO: CGame::CGame()
	m_pGameAudio = new CAudio();
	m_pGameCamera = new CCamera();
	field_8 = 0;
	field_4D = 0;
	field_29 = 0;
	field_31 = 0;
	field_61 = 0;
	field_65 = 0;
	field_69 = 0;
	field_6D = 0;
	memset(unnamed_10150340, 0, sizeof(unnamed_10150340));
	memset(field_6E, 0, sizeof(field_6E));
	field_55 = 0;
	field_59 = 1;
	field_5D = 90;
}

//-----------------------------------------------------------

void CGame::InitGame()
{
	// Create a buffer for game text.
	szGameTextMessage = (char*)calloc(1,513);

	// Init the keystate stuff.
	GameKeyStatesInit();

	// Init the aim stuff.
	GameAimSyncInit();

	// Init radar colors
	GameResetRadarColors();

	if(!ApplyPreGamePatches()) {
		MessageBox(0,
			"I can't determine your GTA version.\r\nSA-MP only supports GTA:SA v1.0 USA/EU",
			"Version Error",MB_OK | MB_ICONEXCLAMATION);
		ExitProcess(1);
	}
}

//-----------------------------------------------------------
