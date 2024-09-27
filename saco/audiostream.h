
#pragma once

#include "bass.h"

//----------------------------------------------------

class CAudioStream
{
private:
	bool field_0;

public:
	CAudioStream() {
		field_0 = 0;
	}

	BOOL Initialize();
	BOOL Stop(bool bWaitForThread=true);
	BOOL Play(char *szURL, float fX, float fY, float fZ, float fDistance, bool bUsePos);
	void HandleGameAudio();
	void Draw();

	void Test3D();

	static void DoMeta();
	static void CALLBACK MetaSync(HSYNC handle, DWORD channel, DWORD data, void *user);
	static void ProcessThread(PVOID v);

	// Delete this:
	void Process() {}
};

//----------------------------------------------------
