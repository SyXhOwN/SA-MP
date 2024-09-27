
#include "main.h"

CUnkClass14::CUnkClass14(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	m_pD3DSprite1 = NULL;
	m_pD3DSprite2 = NULL;

	RestoreDeviceObjects();
}

CUnkClass14::~CUnkClass14()
{
	DeleteDeviceObjects();
}

void CUnkClass14::DeleteDeviceObjects()
{
	SAFE_RELEASE(m_pD3DSprite2);
	SAFE_RELEASE(m_pD3DSprite1);
}

void CUnkClass14::RestoreDeviceObjects()
{
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite1);
	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite2);
}

void CUnkClass14::FUNC_100708B0()
{
	//D3DXCreateFontA();

	//LPD3DXRENDERTOSURFACE;
	//D3DXCreateRenderToSurface();
	//D3DXCreateTexture();
	//D3DXCreateFont();
	//m_pD3DDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
}
