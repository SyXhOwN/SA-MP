
#include "../main.h"
#include "font.h"
#include "util.h"
#include "textures.h"
extern CGame *pGame;

DWORD unnamed_1026B568[200];
BOOL unnamed_1026B888[200];

void CallRwRenderStateSet(int state, int option);

BOOL unnamed_100B2AF0()
{
	memset(unnamed_1026B888, 0, sizeof(unnamed_1026B888));
	memset(unnamed_1026B568, 0, sizeof(unnamed_1026B568));
	return TRUE;
}

int unnamed_100B2B20()
{
	int x=0;
	while(x!=200) {
		if(unnamed_1026B888[x] == FALSE) {
			unnamed_1026B888[x] = TRUE;
			return x;
		}
		x++;
	}
	return (-1);
}

void unnamed_100B2B50(int iSlot)
{
	if(iSlot < 0 || iSlot >= 200) return;

	unnamed_1026B888[iSlot] = FALSE;

	if(unnamed_1026B568[iSlot])
		DestroyTexture(unnamed_1026B568[iSlot]);
	unnamed_1026B568[iSlot] = 0;
}

BOOL unnamed_100B2B90(char *a1, char *a2, bool a3)
{
	char v5[64];
	if(a3)
		strcpy(v5, "SAMP\\");
	else
		strcpy(v5, "models\\txd");

	if(FindTextureSlot(a1) != -1)
		return TRUE;

	int v3 = AddTextureSlot(a1);

	char v9[64];
	sprintf(v9, "%s\\%s.txd", v5, a2);

	if(IsFileOrDirectoryExists(v9))
	{
		LoadTexture(v3, v9);
		AddTextureRef(v3);
		return TRUE;
	}
	return FALSE;
}

DWORD unnamed_100B2C60(char *a1, char *a2)
{
	int v1 = FindTextureSlot(a1);

	if(v1 == -1) return 0;

	PushCurrentTexture();
	SetCurrentTexture(v1);
	DWORD v4 = ReloadTexture(a2);
	PopCurrentTexture();

	return v4;
}

void unnamed_100B2CA0(DWORD a1, int a2, int a3)
{
	if(!a1) return;

	CallRwRenderStateSet(9, 2);

	_asm push a3
	_asm push a2
	_asm mov ecx, a1
	_asm mov edx, 0x728350
	_asm call edx
}

CTextDraw::CTextDraw(TEXT_DRAW_TRANSMIT *TextDrawTransmit, PCHAR szText)
{
	memset(&m_TextDrawData,0,sizeof(TEXT_DRAW_DATA));

	m_TextDrawData.field_0 = TextDrawTransmit->field_1;
	m_TextDrawData.field_4 = TextDrawTransmit->field_5;
	m_TextDrawData.field_8 = TextDrawTransmit->field_9;
	m_TextDrawData.field_C = 0;
	m_TextDrawData.field_D = ((unsigned __int8)TextDrawTransmit->field_0 >> 3) & 1;
	m_TextDrawData.field_E = TextDrawTransmit->field_0 & 1;
	m_TextDrawData.field_F = TextDrawTransmit->field_D;
	m_TextDrawData.field_13 = TextDrawTransmit->field_11;
	m_TextDrawData.field_17 = TextDrawTransmit->field_15;
	m_TextDrawData.field_1B = ((unsigned __int8)TextDrawTransmit->field_0 >> 4) & 1;
	m_TextDrawData.field_1C = TextDrawTransmit->field_1B;
	m_TextDrawData.field_20 = TextDrawTransmit->field_19;
	m_TextDrawData.field_21 = TextDrawTransmit->field_1A;
	m_TextDrawData.field_22 = ((unsigned __int8)TextDrawTransmit->field_0 >> 1) & 1;
	m_TextDrawData.field_23 = ((unsigned __int8)TextDrawTransmit->field_0 >> 2) & 1;
	m_TextDrawData.field_24 = TextDrawTransmit->field_1F;
	m_TextDrawData.field_28 = TextDrawTransmit->field_21;
	m_TextDrawData.field_2C = TextDrawTransmit->field_25;
	m_TextDrawData.field_38 = -1;
	m_TextDrawData.field_3C = -1;
	m_TextDrawData.field_44 = TextDrawTransmit->field_20;
	m_TextDrawData.field_45 = TextDrawTransmit->field_29;
	m_TextDrawData.field_47 = TextDrawTransmit->field_2B;
	m_TextDrawData.field_4B = TextDrawTransmit->field_2F;
	m_TextDrawData.field_4F = TextDrawTransmit->field_33;
	m_TextDrawData.field_53 = TextDrawTransmit->field_37;
	m_TextDrawData.field_57 = TextDrawTransmit->field_3B;
	m_TextDrawData.field_59 = TextDrawTransmit->field_3D;
	field_9C0 = 0;
	m_TextDrawData.field_40 = -1;

	SetText(szText);

	if(m_TextDrawData.field_24 == 4)
	{
		m_TextDrawData.field_40 = unnamed_100B2B20();
		FUNC_100B2CD0();
	}

	field_9BF = 0;
	field_9C1 = 0;
	field_9C9 = 0;
	field_9C5 = 0;
	field_9CD = 0;
	field_9D1 = 0;
	field_9D2 = 0;
}

CTextDraw::~CTextDraw()
{
	unnamed_100B2B50(m_TextDrawData.field_40);
}

void CTextDraw::SetText(char* szText)
{
	// TODO: CTextDraw::SetText .text:100B2F60
}

void CTextDraw::FUNC_100B2CD0()
{
	// TODO: CTextDraw::FUNC_100B2CD0 .text:100B2CD0
}












































