
#pragma once

class CUnkClass3
{
private:
	IDirect3DDevice9* m_pD3DDevice;
	char _gap4[8];
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	CDXUTDialog *m_pDialog;
	CDXUTListBox *m_pListBox;
	CDXUTIMEEditBox *m_pEditControl;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	char _gap38[81];
	char field_89[516];
	char _gap28D[16];

public:
	CUnkClass3(IDirect3DDevice9 *pD3DDevice);

	int MsgProc(UINT uMsg, DWORD wParam, DWORD lParam);
	bool FUNC_1006F4F0();
	void ResetDialogControls(CDXUTDialog *pDialog);
};
