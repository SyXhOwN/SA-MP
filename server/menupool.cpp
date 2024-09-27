
#include "main.h"

//----------------------------------------------------

CMenuPool::CMenuPool()
{
	for (BYTE byteMenuID = 0; byteMenuID < MAX_MENUS; byteMenuID++)
	{
		m_bMenuSlotState[byteMenuID] = FALSE;
		m_pMenus[byteMenuID] = NULL;
	}
	for (PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		m_bytePlayerMenu[playerId] = 255;
	}
}

//----------------------------------------------------

CMenuPool::~CMenuPool()
{
	for (BYTE byteMenuID = 0; byteMenuID < MAX_MENUS; byteMenuID++)
	{
		if (m_pMenus[byteMenuID])
		{
			delete m_pMenus[byteMenuID];
			m_pMenus[byteMenuID] = NULL;
		}
	}
}

//----------------------------------------------------

BYTE CMenuPool::New(PCHAR pTitle, float fX, float fY, BYTE byteColumns, float fCol1Width, float fCol2Width)
{
	BYTE byteMenuID;

	for (byteMenuID = 1; byteMenuID < MAX_MENUS; byteMenuID++)
	{
		if (m_bMenuSlotState[byteMenuID] == FALSE) break;
	}
	if (byteMenuID == MAX_MENUS) return 0xFF;

	CMenu* pMenu = new CMenu(pTitle, fX, fY, byteColumns, fCol1Width, fCol2Width);

	if (pMenu)
	{
		m_bMenuSlotState[byteMenuID] = TRUE;
		pMenu->SetID(byteMenuID);
		m_pMenus[byteMenuID] = pMenu;
		return byteMenuID;
	}
	return 0xFF;
}

//----------------------------------------------------

BOOL CMenuPool::Delete(BYTE byteMenuID)
{
	if (m_bMenuSlotState[byteMenuID] == FALSE || !m_pMenus[byteMenuID])
	{
		return FALSE;
	}
	m_bMenuSlotState[byteMenuID] = FALSE;
	delete m_pMenus[byteMenuID];
	m_pMenus[byteMenuID] = NULL;

	return TRUE;
}

