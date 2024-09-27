
#include "../main.h"

BOOL unnamed_100B2AF0();

//----------------------------------------------------

CTextDrawPool::CTextDrawPool()
{
	int x=0;
	while(x!=MAX_TEXT_DRAWS) {
		m_pTextDraw[x] = NULL;
		m_bSlotState[x] = FALSE;
		x++;
	}
	unnamed_100B2AF0();
}

//----------------------------------------------------

CTextDrawPool::~CTextDrawPool()
{
	int x=0;
	while(x != MAX_TEXT_DRAWS) {
		if(m_pTextDraw[x]) {
			delete m_pTextDraw[x];
			m_pTextDraw[x] = NULL;
			m_bSlotState[x] = FALSE;
		}
		x++;
	}
}

//----------------------------------------------------

CTextDraw* CTextDrawPool::New(WORD wText, TEXT_DRAW_TRANSMIT *TextDrawTransmit, PCHAR szText)
{
	if(m_pTextDraw[wText]) Delete(wText);

	CTextDraw* pTextDraw = new CTextDraw(TextDrawTransmit, szText);

	if(pTextDraw)
	{
		m_pTextDraw[wText] = pTextDraw;
		m_bSlotState[wText] = TRUE;
		return pTextDraw;
	}
	return NULL;
}

//----------------------------------------------------

void CTextDrawPool::Delete(WORD wText)
{
	if(m_pTextDraw[wText]) {
		delete m_pTextDraw[wText];
		m_pTextDraw[wText] = NULL;
		m_bSlotState[wText] = FALSE;
	}
}

//----------------------------------------------------
