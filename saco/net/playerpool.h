
#pragma once

#include <string>

#define INVALID_PLAYER_ID 0xFFFF
#define NO_TEAM 255

//----------------------------------------------------

class CPlayerPool
{
private:

	int field_0;
	PLAYERID		m_LocalPlayerID;
	std::string	field_6;
	int field_22;
	CLocalPlayer	*m_pLocalPlayer;
	BOOL			m_bPlayerSlotState[MAX_PLAYERS];

	char _gapFDA[4016];

	CNetPlayer		*m_pPlayers[MAX_PLAYERS];
	int field_2F3A;

public:

	void SetLocalPlayerName(PCHAR szName) { field_6 = szName; };
	PCHAR GetLocalPlayerName() { return (PCHAR)field_6.c_str(); };

	CLocalPlayer * GetLocalPlayer() { return m_pLocalPlayer; };

	BOOL New(PLAYERID playerId, PCHAR szPlayerName, BOOL bIsNPC);
	BOOL Delete(PLAYERID playerId, BYTE byteReason);

	void RecalcPoolSize();

	void SetAt(PLAYERID playerId, CNetPlayer *pPlayer) {
		if(playerId >= MAX_PLAYERS) return;
		m_pPlayers[playerId] = pPlayer;
	};

	CNetPlayer* GetAt(PLAYERID playerId) {
		if(playerId >= MAX_PLAYERS || m_pPlayers[playerId] == NULL) { return NULL; }
		return m_pPlayers[playerId];
	};

	// Find out if the slot is inuse.
	BOOL GetSlotState(PLAYERID playerId) {
		if(playerId >= MAX_PLAYERS) { return FALSE; }
		return m_bPlayerSlotState[playerId];
	};

	PLAYERID GetLocalPlayerID() { return m_LocalPlayerID; };

	CPlayerPool();
	~CPlayerPool();
};

//----------------------------------------------------
