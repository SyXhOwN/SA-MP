
#include "../main.h"

//----------------------------------------------------

CPlayerPool::CPlayerPool()
{
	// loop through and initialize all net players to null and slot states to false
	PLAYERID playerId = 0;
	while(playerId < MAX_PLAYERS) {
		m_pPlayers[playerId] = NULL;
		m_bPlayerSlotState[playerId] = FALSE;
		playerId++;
	}
	m_pLocalPlayer = new CLocalPlayer();
	field_0 = 0;
	field_22 = 0;
	m_LocalPlayerID = 0;
	field_2F3A = 0;
}

//----------------------------------------------------

CPlayerPool::~CPlayerPool()
{
	delete m_pLocalPlayer;
	m_pLocalPlayer = NULL;

	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		Delete(playerId,0);
	}
}

//----------------------------------------------------

BOOL CPlayerPool::New(PLAYERID playerId, PCHAR szPlayerName, BOOL bIsNPC)
{
	CNetPlayer *pNetPlayer = new CNetPlayer(szPlayerName, bIsNPC);

	if(playerId < MAX_PLAYERS && (m_pPlayers[playerId] = pNetPlayer) != NULL)
	{
		m_pPlayers[playerId]->m_pRemotePlayer->SetID(playerId);
		// TODO: CPlayerPool::New
		m_bPlayerSlotState[playerId] = TRUE;
		RecalcPoolSize();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

BOOL CPlayerPool::Delete(PLAYERID playerId, BYTE byteReason)
{
	if(!GetSlotState(playerId) || !m_pPlayers[playerId])
	{
		return FALSE; // Player already deleted or not used.
	}

	if (GetLocalPlayer() && GetLocalPlayer()->IsSpectating() && GetLocalPlayer()->m_SpectateID == playerId) {
		GetLocalPlayer()->ToggleSpectating(FALSE);
	}

	m_bPlayerSlotState[playerId] = FALSE;
	delete m_pPlayers[playerId];
	m_pPlayers[playerId] = NULL;

	RecalcPoolSize();

	return TRUE;
}

//----------------------------------------------------

void CPlayerPool::RecalcPoolSize()
{
	int iNewIndex = 0;
	int i = 0;

	do
	{
		if(m_bPlayerSlotState[i])
			iNewIndex = i;
		if(m_bPlayerSlotState[i+1])
			iNewIndex = i+1;
		if(m_bPlayerSlotState[i+2])
			iNewIndex = i+2;
		if(m_bPlayerSlotState[i+3])
			iNewIndex = i+3;
		i += 4;
	}
	while(i < MAX_PLAYERS);

	field_2F3A = iNewIndex;
}
