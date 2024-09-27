
#include "main.h"

CMenu::CMenu(PCHAR pTitle, float fX, float fY, BYTE byteColumns, float fCol1Width, float fCol2Width)
{
	ResetForAll();
	for (BYTE i = 0; i < MAX_MENU_ITEMS; i++)
	{
		m_charItems[i][0][0] = '\0';
		m_charItems[i][1][0] = '\0';
		m_MenuInteraction.bRow[i] = TRUE;
	}
	m_MenuInteraction.bMenu = TRUE;
	m_charHeader[0][0] = '\0';
	m_charHeader[1][0] = '\0';

	if (strlen(pTitle) >= MAX_MENU_LINE) pTitle[MAX_MENU_LINE - 1] = '\0';
	strcpy(m_charTitle, pTitle);

	m_fXPos = fX;
	m_fYPos = fY;
	m_fCol1Width = fCol1Width;
	m_fCol2Width = fCol2Width;

	if (byteColumns == 2) m_byteColumns = 2;
	else m_byteColumns = 1;
	m_byteColCount[0] = 0;
	m_byteColCount[1] = 0;
}

void CMenu::ResetForAll()
{
	for (PLAYERID i = 0; i < MAX_PLAYERS; i++)
	{
		m_bInitedForPlayer[i] = FALSE;
	}
}

BYTE CMenu::AddMenuItem(BYTE byteColumn, PCHAR pText)
{
	if (byteColumn >= MAX_COLUMNS) return 0xFF;
	BYTE byteCount = m_byteColCount[byteColumn];
	if (byteCount >= MAX_MENU_LINE) return 0xFF;
	if (strlen(pText) >= MAX_MENU_LINE) pText[MAX_MENU_LINE - 1] = '\0';

	ResetForAll();
	strcpy(m_charItems[byteCount][byteColumn], pText);
	m_byteColCount[byteColumn]++;

	return byteCount;
}

void CMenu::SetColumnTitle(BYTE byteColumn, PCHAR pText)
{
	if (byteColumn >= MAX_COLUMNS) return;
	if (strlen(pText) >= MAX_MENU_LINE) pText[MAX_MENU_LINE - 1] = '\0';
	ResetForAll();
	strcpy(m_charHeader[byteColumn], pText);
}

void CMenu::InitForPlayer(PLAYERID playerId)
{
	if (playerId >= MAX_PLAYERS) return;

	m_bInitedForPlayer[playerId] = TRUE;

	RakNet::BitStream bsMenu;

	bsMenu.Write(m_byteMenuID);
	bsMenu.Write((BOOL)(m_byteColumns == 2));
	bsMenu.Write(m_charTitle, MAX_MENU_LINE);
	bsMenu.Write(m_fXPos);
	bsMenu.Write(m_fYPos);
	bsMenu.Write(m_fCol1Width);
	if (m_byteColumns == 2)
	{
		bsMenu.Write(m_fCol2Width);
	}
	bsMenu.Write(m_MenuInteraction.bMenu);
	for (BYTE i = 0; i < MAX_MENU_ITEMS; i++)
	{
		bsMenu.Write(m_MenuInteraction.bRow[i]);
	}
	bsMenu.Write(m_charHeader[0], MAX_MENU_LINE);
	BYTE count = m_byteColCount[0];
	bsMenu.Write(count);
	for (BYTE i = 0; i < count; i++)
	{
		bsMenu.Write(m_charItems[i][0], MAX_MENU_LINE);
	}
	if (m_byteColumns == 2)
	{
		bsMenu.Write(m_charHeader[1], MAX_MENU_LINE);
		count = m_byteColCount[1];
		bsMenu.Write(count);
		for (BYTE i = 0; i < count; i++)
		{
			bsMenu.Write(m_charItems[i][1], MAX_MENU_LINE);
		}
	}
	pNetGame->SendToPlayer(RPC_ScrInitMenu, &bsMenu, playerId, 2);
}

void CMenu::ShowForPlayer(PLAYERID playerId)
{
	if (playerId >= MAX_PLAYERS) return;

	if (!m_bInitedForPlayer[playerId]) InitForPlayer(playerId);

	// Send command to display it here
	RakNet::BitStream bsMenu;
	bsMenu.Write(m_byteMenuID);
	pNetGame->SendToPlayer(RPC_ScrShowMenu, &bsMenu, playerId, 2);
}

void CMenu::HideForPlayer(PLAYERID playerId)
{
	if (playerId >= MAX_PLAYERS) return;

	RakNet::BitStream bsMenu;
	bsMenu.Write(m_byteMenuID);
	pNetGame->SendToPlayer(RPC_ScrHideMenu, &bsMenu, playerId, 2);
}
