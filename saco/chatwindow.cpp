
#include "main.h"

DWORD FUNC_100B6100(char *szString, int nMaxLen);

//----------------------------------------------------

CChatWindow::CChatWindow(IDirect3DDevice9 *pD3DDevice, CFontRender *pFontRender, CHAR *szChatLogFile)
{
	int x=0;

	m_pD3DDevice		= pD3DDevice;
	m_pFontRender		= pFontRender;
	field_8 = 2;

	// Create a sprite to use when drawing text
	D3DXCreateSprite(pD3DDevice,&field_63A6);
	D3DXCreateSprite(pD3DDevice,&field_63AA);

	// Init the chat window lines to 0
	while(x!=MAX_MESSAGES) {
		memset(&m_ChatWindowEntries[x],0,sizeof(CHAT_WINDOW_ENTRY));
		x++;
	}

	m_dwChatTextColor = D3DCOLOR_ARGB(255,255,255,255);
	m_dwChatInfoColor = D3DCOLOR_ARGB(255,136,170,98);
	m_dwChatDebugColor = D3DCOLOR_ARGB(255,169,196,228);

	field_0 = 10;
	field_C = 0;

	if(szChatLogFile && strlen(szChatLogFile))
	{
		memset(&field_11[0],0,sizeof(field_11));
		strncpy(field_11,szChatLogFile,MAX_PATH);

		FILE *f = fopen(field_11, "w");
		if(f)
		{
			field_D = 1;
			fclose(f);
		}
	}

	field_63BE = 0;
	field_63BA = NULL;
	field_63B6 = NULL;
	field_63D2 = GetTickCount();
	field_63DE = 1;
	m_pScrollBar = NULL;
	m_pGameUI = NULL;
	field_11A = 0;

	CreateFonts();
}

//----------------------------------------------------

void CChatWindow::CreateFonts()
{

	// TODO: CChatWindow::CreateFonts .text:100681D0
}

//----------------------------------------------------
// MATCH
void CChatWindow::ResetDialogControls(CDXUTDialog *pGameUI)
{
	m_pGameUI = pGameUI;

	if(pGameUI) {
		m_pScrollBar = new CDXUTScrollBar(pGameUI);
		pGameUI->AddControl(m_pScrollBar);
		m_pScrollBar->SetVisible(true);
		m_pScrollBar->SetEnabled(true);

		FUNC_10067200();
	}
}

//----------------------------------------------------

void CChatWindow::AddChatMessage(CHAR *szNick, DWORD dwNickColor, CHAR *szMessage)
{
	FilterInvalidChars(szMessage);
	AddToChatWindowBuffer(CHAT_TYPE_CHAT,szMessage,szNick,m_dwChatTextColor,dwNickColor);
}

//----------------------------------------------------

void CChatWindow::AddInfoMessage(CHAR * szFormat, ...)
{
	char tmp_buf[512];
	memset(tmp_buf,0,512);

	va_list args;
	va_start(args, szFormat);
	vsprintf(tmp_buf, szFormat, args);
	va_end(args);

	FilterInvalidChars(tmp_buf);
	AddToChatWindowBuffer(CHAT_TYPE_INFO,tmp_buf,NULL,m_dwChatInfoColor,0);
}

//----------------------------------------------------

void CChatWindow::AddDebugMessage(CHAR * szFormat, ...)
{
	char tmp_buf[512];
	memset(tmp_buf,0,512);

	va_list args;
	va_start(args, szFormat);
	vsprintf(tmp_buf, szFormat, args);
	va_end(args);

	FilterInvalidChars(tmp_buf);
	AddToChatWindowBuffer(CHAT_TYPE_DEBUG,tmp_buf,NULL,m_dwChatDebugColor,0);
	OutputDebugString(tmp_buf);
}

//----------------------------------------------------

void CChatWindow::AddClientMessage(DWORD dwColor, PCHAR szStr)
{
	dwColor = (dwColor >> 8) | 0xFF000000; // convert to ARGB

	if(strlen(szStr) > 144) return;

	FUNC_100B6100(szStr, 255);
	FilterInvalidChars(szStr);

	AddToChatWindowBuffer(CHAT_TYPE_INFO,szStr,NULL,dwColor,0);
}

//----------------------------------------------------

void CChatWindow::FilterInvalidChars(PCHAR szString)
{
	while(*szString) {
		if(*szString > 0 && *szString < ' ') {
			*szString = ' ';
		}
		szString++;
	}
}

//----------------------------------------------------

void CChatWindow::AddToChatWindowBuffer(eChatMessageType eType, 
										PCHAR szString, 
										PCHAR szNick,
										DWORD dwTextColor,
										DWORD dwChatColor)
{
	// TODO: CChatWindow::AddToChatWindowBuffer
}

//----------------------------------------------------
// MATCH
void CChatWindow::FUNC_10067200()
{
	if(m_pScrollBar) {
		m_pScrollBar->SetLocation(10,40);
		m_pScrollBar->SetSize(20,((field_63E2+1)*field_0)-60);
		m_pScrollBar->SetTrackRange(1,MAX_MESSAGES);
		m_pScrollBar->SetPageSize(field_0);
		m_pScrollBar->SetTrackPos(MAX_MESSAGES-field_0);
	}
}

