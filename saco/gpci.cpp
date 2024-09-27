
#include "main.h"
#include "fhicl-sha1.h"
#include "../ttmath/ttmath.h"

extern CNetGame *pNetGame;

void GenerateComputerID(char* a1, char* a2, int a3)
{
	fhicl::SHA1 sha1;
	unsigned int digest[5];

	memset(digest, 0, sizeof(digest));

	sha1.Input(a2, strlen(a2));
	sha1.Result(digest);

	if(!pNetGame)
		a3--;

	// TODO: GenerateComputerID
}
