
#ifndef SAMPSRV_PLAYERPOOL_H
#define SAMPSRV_PLAYERPOOL_H

#define INVALID_PLAYER_ID 0xFFFF

class CPlayerPool // size: WL 199024
{
private:

	int m_iVirtualWorld[MAX_PLAYERS];
	int field_FA0;
	int field_FA4;
	int field_FA8;
	int m_iPlayerScore[MAX_PLAYERS];
	int m_iPlayerMoney[MAX_PLAYERS];
	int m_iPlayerDrunkLevel[MAX_PLAYERS];
	DWORD field_3E8C[1000];
	CHAR m_szPlayerClientID[MAX_PLAYERS][101];
	CHAR m_szPlayerVersion[MAX_PLAYERS][25];
	int field_23A5C[MAX_PLAYERS];
	BOOL m_bPlayerSlotState[MAX_PLAYERS];
	CPlayer *m_pPlayers[MAX_PLAYERS];
	CHAR m_szPlayerName[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	BOOL m_bIsAnAdmin[MAX_PLAYERS];
	BOOL m_bIsAnNPC[MAX_PLAYERS];

	char _gap2EA24[8000];

	int field_30964;
	int m_iPoolSize;
	int field_3096C;

public:

	CPlayerPool();
	~CPlayerPool();

	BOOL Process(float fElapsedTime);
	BOOL New(PLAYERID playerId, PCHAR szPlayerName, PCHAR szClientID, PCHAR szVersion, BOOL bIsNPC);
	BOOL Delete(PLAYERID playerId, BYTE byteReason);

	// Retrieve a player
	CPlayer* GetAt(PLAYERID playerId) {
		if (playerId >= MAX_PLAYERS) { return NULL; }
		return m_pPlayers[playerId];
	};

	// Find out if the slot is inuse.
	BOOL GetSlotState(PLAYERID playerId) {
		if(playerId >= MAX_PLAYERS) { return FALSE; }
		return m_bPlayerSlotState[playerId];
	};

	PCHAR GetPlayerName(PLAYERID playerId) {
		if(playerId >= MAX_PLAYERS) { return NULL; }
		return m_szPlayerName[playerId];
	};

	PCHAR GetPlayerClientID(PLAYERID playerId) {
		if(playerId >= MAX_PLAYERS) { return NULL; }
		return m_szPlayerClientID[playerId];
	};

	void SetPlayerMoney(PLAYERID playerId, int iMoney) {
		if(playerId >= MAX_PLAYERS) return;
		m_iPlayerMoney[playerId] = iMoney;
	};

	void SetPlayerDrunkLevel(PLAYERID playerId, int iLevel) {
		if(playerId >= MAX_PLAYERS) return;
		m_iPlayerDrunkLevel[playerId] = iLevel;
	};

	void ResetPlayerScoresAndMoney() {
		memset(&m_iPlayerScore[0],0,sizeof(int) * MAX_PLAYERS);
		memset(&m_iPlayerMoney[0],0,sizeof(int) * MAX_PLAYERS);
		memset(&m_iVirtualWorld[0],0,sizeof(int) * MAX_PLAYERS);
	};

	BOOL IsAdmin(PLAYERID playerId) { return m_bIsAnAdmin[playerId]; };

	BOOL IsNPC(PLAYERID playerId) { return m_bIsAnNPC[playerId]; };

	int GetPoolSize() { return m_iPoolSize; };
};

#endif
