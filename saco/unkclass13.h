
#pragma once

class CUnkClass13
{
private:
	IDirect3DDevice9 *m_pD3DDevice;
	ID3DXRenderToSurface *m_pD3DRenderToSurface;
	IDirect3DTexture9 *m_pD3DTexture;
	IDirect3DSurface9 *m_pD3DSurface;
	D3DDISPLAYMODE m_DisplayMode;
	IDirect3DTexture9 *field_20;

public:
	CUnkClass13(IDirect3DDevice9 *pD3DDevice);
	~CUnkClass13();

	void DeleteDeviceObjects();
	void RestoreDeviceObjects();

	IDirect3DTexture9* FUNC_1006D880(char *szString);
};
