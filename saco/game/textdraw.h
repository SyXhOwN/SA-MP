
#pragma once

#define MAX_TEXT_DRAW_LINE 800

//-----------------------------------------------------------

typedef struct _TEXT_DRAW_DATA
{
	int field_0;
	int field_4;
	int field_8;
	char field_C;
	char field_D;
	char field_E;
	int field_F;
	int field_13;
	int field_17;
	char field_1B;
	int field_1C;
	char field_20;
	char field_21;
	char field_22;
	char field_23;
	int field_24;
	int field_28;
	int field_2C;

	char _gap30[8];

	int field_38;
	int field_3C;
	int field_40;
	char field_44;
	short field_45;
	int field_47;
	int field_4B;
	int field_4F;
	int field_53;
	short field_57;
	short field_59;
} TEXT_DRAW_DATA;

//-----------------------------------------------------------

typedef struct _TEXT_DRAW_TRANSMIT
{
	char field_0;
	int field_1;
	int field_5;
	int field_9;
	int field_D;
	int field_11;
	int field_15;
	char field_19;
	char field_1A;
	int field_1B;
	char field_1F;
	char field_20;
	int field_21;
	int field_25;
	short field_29;
	int field_2B;
	int field_2F;
	int field_33;
	int field_37;
	short field_3B;
	short field_3D;
} TEXT_DRAW_TRANSMIT;

//----------------------------------------------------

class CTextDraw
{
private:
	CHAR			m_szText[MAX_TEXT_DRAW_LINE];

	TEXT_DRAW_DATA  m_TextDrawData;

	char field_9BF;
	char field_9C0;
	int field_9C1;
	int field_9C5;
	int field_9C9;
	int field_9CD;
	char field_9D1;
	int field_9D2;

public:
	CTextDraw(TEXT_DRAW_TRANSMIT *TextDrawTransmit, PCHAR szText);
	~CTextDraw();

	void SetText(char* szText);

	void FUNC_100B2CD0();
};

//----------------------------------------------------
