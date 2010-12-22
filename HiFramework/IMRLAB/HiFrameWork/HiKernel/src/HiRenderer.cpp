//#include"HiKernel/HiDXRenderer.h"
//
//
//HiDXRenderer::HiDXRenderer(HWND hWnd) : m_hWnd(hWnd), m_pD3DInterface(NULL), 
//m_pD3DDevice(NULL), m_bWindowed(false), m_iWidth(640), m_iHeight(480)
//{
//}
//
//HiDXRenderer::~HiDXRenderer()
//{
//	Terminate();
//}
//
//void HiDXRenderer::SetScreen(bool bWindowed, int iWidth, int iHeight)
//{
//	m_bWindowed = bWindowed;
//	m_iWidth = iWidth;
//	m_iHeight = iHeight;
//}
//
//HRESULT HiDXRenderer::Create()
//{
//	D3DDISPLAYMODE d3ddm;
//
//	if( NULL == (m_pD3DInterface = Direct3DCreate9(D3D_SDK_VERSION)))
//		return E_FAIL;
//	if( FAILED(m_pD3DInterface->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
//		return E_FAIL;
//
//	D3DPRESENT_PARAMETERS d3dpp;
//	ZeroMemory( &d3dpp, sizeof(d3dpp) );
//	d3dpp.Windowed = TRUE;
//	d3dpp.BackBufferCount        = 2;
//	d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;
//	d3dpp.MultiSampleQuality     = 0;
//	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
//	d3dpp.BackBufferFormat = d3ddm.Format;
//	d3dpp.EnableAutoDepthStencil = TRUE;
//	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
//	d3dpp.Flags              = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
//	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
//	d3dpp.EnableAutoDepthStencil = D3DFMT_D24S8;
//
//	D3DCAPS9 Caps;
//	m_pD3DInterface->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps);
//
//	if(Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
//	{
//		if( FAILED( m_pD3DInterface->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
//			D3DCREATE_HARDWARE_VERTEXPROCESSING,
//			&d3dpp, &m_pD3DDevice ) ) )
//			return E_FAIL;
//	}
//	else
//	{
//		if( FAILED( m_pD3DInterface->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
//			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
//			&d3dpp, &m_pD3DDevice ) ) )
//			return E_FAIL;
//	}
//
//	return S_OK;
//}
//
//void HiDXRenderer::BeginScene()
//{
//	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER  , D3DCOLOR_XRGB(0, 0, 77), 1.0f, 0);		
//	m_pD3DDevice->BeginScene();
//
//	D3DLIGHT9 light;
//	ZeroMemory( &light, sizeof(D3DLIGHT9) );
//	light.Type       = D3DLIGHT_DIRECTIONAL;
//	light.Ambient.r=0.3f;light.Ambient.g=0.3f;light.Ambient.b=0.3f;light.Ambient.a=1.0f;
//	light.Diffuse.r=0.9f;light.Diffuse.g=0.9f;light.Diffuse.b=0.9f;light.Diffuse.a=1.0f;
//	light.Specular.r=0.1f;light.Specular.g=0.1f;light.Specular.b=0.1f;light.Specular.a=1.0f;
//
//	light.Direction= D3DXVECTOR3(0.0f,0.70f,0.70f);
//
//	m_pD3DDevice->SetLight( 0, &light );
//	m_pD3DDevice->LightEnable( 0, TRUE );  
//	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
//
//}
//
//void HiDXRenderer::EndScene()
//{
//	m_pD3DDevice->EndScene();
//	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
//}
//
//
//void HiDXRenderer::Terminate()
//{
//	if(m_pD3DDevice != NULL)
//	{
//		m_pD3DDevice->Release();
//		m_pD3DDevice = NULL;
//	}
//	if(m_pD3DInterface != NULL)
//	{
//		m_pD3DInterface->Release();
//		m_pD3DInterface = NULL;
//	}
//}