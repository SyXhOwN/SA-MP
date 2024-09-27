
#include "main.h"

//----------------------------------------------------

int ProcessQueryPacket(unsigned int binaryAddress, unsigned short port, char* data, int length, SOCKET s)
{
	

	if ((length > 4) && (*(unsigned long*)(data) == 0x504D4153))
	{
		if(!pNetGame || (pNetGame->GetGameState() != GAMESTATE_RUNNING)) return 0;

		if (length >= 11)
		{
			// TODO: ProcessQueryPacket
		}
		return 1;
	} else {
		return 0;
	}
}

//----------------------------------------------------
