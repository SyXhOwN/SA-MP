
#ifndef SAMPSRV_MENUPOOL_H
#define SAMPSRV_MENUPOOL_H

//----------------------------------------------------

class CMenuPool // size: WL 2024
{
private:

	CMenu *m_pMenus[MAX_MENUS];
	BOOL m_bMenuSlotState[MAX_MENUS];
	BYTE m_bytePlayerMenu[MAX_PLAYERS];

public:
	CMenuPool();
	~CMenuPool();

	BYTE New(PCHAR pTitle, float fX, float fY, BYTE byteColumns, float fCol1Width, float fCol2Width);

	BOOL Delete(BYTE byteMenuID);

	// Retrieve a menu by id
	CMenu* GetAt(BYTE byteMenuID)
	{
		if(byteMenuID > MAX_MENUS) { return NULL; }
		return m_pMenus[byteMenuID];
	};

	// Find out if the slot is inuse.
	BOOL GetSlotState(BYTE byteMenuID)
	{
		if(byteMenuID > MAX_MENUS) { return FALSE; }
		return m_bMenuSlotState[byteMenuID];
	};

	BYTE GetPlayerMenu(PLAYERID playerId)
	{
		if (playerId >= MAX_PLAYERS) return 255;
		return m_bytePlayerMenu[playerId];
	}

	void SetPlayerMenu(PLAYERID playerId, BYTE byteMenu)
	{
		if (playerId < MAX_PLAYERS && byteMenu < MAX_MENUS) m_bytePlayerMenu[playerId] = byteMenu;
	}
};

//----------------------------------------------------

#endif
