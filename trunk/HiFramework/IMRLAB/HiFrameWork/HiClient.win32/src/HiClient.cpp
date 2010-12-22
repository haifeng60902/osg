
#include "HiKernel/HiFramework.h"
#include "HiKernel/HiViewer.h"

const char* g_szApplicationName = "HiClient";
const char*	g_szWindowClassName = "HiClient";


LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_DESTROY :
			PostQuitMessage( 0 );
			return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}



INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{

	/// ������ Ŭ���� ���
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, g_szWindowClassName, NULL };
	RegisterClassEx( &wc );

	/// ������ ����
	HWND hWnd = CreateWindow( g_szWindowClassName, g_szApplicationName, WS_POPUPWINDOW, 0, 0, 1024,768, GetDesktopWindow(), NULL, wc.hInstance, NULL );
//WS_OVERLAPPEDWINDOW,WS_POPUP

	/// HiViewer ����
	HiKernel::HiViewer * _viewer = HiKernel::HiViewer::getInstance("HiFramework.xml"); // chpark 090413 : xml ���ϸ��� ��ɾ� ���ڷ� ó��
	HiKernel::HiViewer::getInstance()->Config(hWnd, hInst);  // chpark 090413 : ������ ���� ���Ŀ� �ؾ� ���� ���� �ʿ�

	
	/// ������ ���
	//ShowWindow( hWnd, SW_SHOWDEFAULT );
	//UpdateWindow( hWnd );


	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while( msg.message!=WM_QUIT  )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if ( HiKernel::HiViewer::getInstance()->Run() ) 
				break;

		}
	}

	// HiViewer �Ҹ�
	HiKernel::HiViewer::getInstance()->Terminate();
	delete _viewer;

	// ������ Ŭ���� ��� ����
	UnregisterClass( "HiClient", wc.hInstance );

	return 0;
}


void main()
{
	WinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOW);
}
