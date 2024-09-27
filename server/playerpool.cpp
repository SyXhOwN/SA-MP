
#include "main.h"
extern CNetGame *pNetGame;

CPlayerPool::CPlayerPool()
{
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		m_bPlayerSlotState[playerId] = FALSE;
		m_pPlayers[playerId] = NULL;
		field_23A5C[playerId] = 0;
		field_3E8C[playerId] = pNetGame->GetTime();
	}
	field_30964 = 0;
	m_iPoolSize = 0;
	field_FA0 = pNetGame->GetTime();
	field_FA4 = 0;
	field_FA8 = 0;
	field_3096C = 0;
}

CPlayerPool::~CPlayerPool()
{
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		Delete(playerId,0);
	}
}

//----------------------------------------------------

BOOL CPlayerPool::New(PLAYERID playerId, PCHAR szPlayerName, PCHAR szClientID, PCHAR szVersion, BOOL bIsNPC)
{
	if(playerId > MAX_PLAYERS) return FALSE;
	if(strlen(szPlayerName) > MAX_PLAYER_NAME) return FALSE;

	m_pPlayers[playerId] = new CPlayer();

	if(m_pPlayers[playerId])
	{
		strcpy(m_szPlayerName[playerId],szPlayerName);
		memset(m_szPlayerClientID[playerId],0,sizeof(m_szPlayerClientID[playerId]));
		memset(m_szPlayerVersion[playerId],0,sizeof(m_szPlayerVersion[playerId]));

		if(szClientID) {
			if(strlen(szClientID) >= 100)
				return FALSE;
			strcpy(m_szPlayerClientID[playerId],szClientID);
		}

		if(szVersion) {
			if(strlen(szVersion) >= 24)
				return FALSE;
			strcpy(m_szPlayerVersion[playerId],szVersion);
		}

		m_bPlayerSlotState[playerId] = TRUE;
		m_iPlayerScore[playerId] = 0;
		m_iPlayerMoney[playerId] = 0;
		m_bIsAnAdmin[playerId] = FALSE;
		m_iVirtualWorld[playerId] = 0;
		m_iPlayerDrunkLevel[playerId] = 0;

		BYTE byteIsNPC;
		if(bIsNPC) {
			m_bIsAnNPC[playerId] = TRUE;
			byteIsNPC = 1;
		} else {
			m_bIsAnNPC[playerId] = FALSE;
			byteIsNPC = 0;
		}

		// Notify all the other players of a newcommer with
		// a 'ServerJoin' join RPC 
		RakNet::BitStream bsSend;
		bsSend.Write(playerId);
		bsSend.Write((DWORD)0);
		bsSend.Write(byteIsNPC);
		BYTE namelen = strlen(szPlayerName);
		bsSend.Write(namelen);
		bsSend.Write(szPlayerName,namelen);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CPlayerPool::Delete(PLAYERID playerId, BYTE byteReason)
{
	// TODO: CPlayerPool::Delete W .text:00466570 L .text:080D0A90
	return FALSE;
}

BOOL CPlayerPool::Process(float fElapsedTime)
{
	// TODO: CPlayerPool::Process
	return TRUE;
}
