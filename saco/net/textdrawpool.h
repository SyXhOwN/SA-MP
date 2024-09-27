
#pragma once

//----------------------------------------------------

class CTextDrawPool
{
private:
	BOOL			m_bSlotState[MAX_TEXT_DRAWS];
	CTextDraw		*m_pTextDraw[MAX_TEXT_DRAWS];
public:
	CTextDrawPool();
	~CTextDrawPool();

	CTextDraw * New(WORD wText, TEXT_DRAW_TRANSMIT *TextDrawTransmit, PCHAR szText);
	void Delete(WORD wText);
};

//----------------------------------------------------
