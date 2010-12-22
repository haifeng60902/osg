
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

	/// 윈도우 클래스 등록
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, g_szWindowClassName, NULL };
	RegisterClassEx( &wc );

	/// 윈도우 생성
	HWND hWnd = CreateWindow( g_szWindowClassName, g_szApplicationName, WS_POPUPWINDOW, 0, 0, 1024,768, GetDesktopWindow(), NULL, wc.hInstance, NULL );
//WS_OVERLAPPEDWINDOW,WS_POPUP

	/// HiViewer 생성
	HiKernel::HiViewer * _viewer = HiKernel::HiViewer::getInstance("HiFramework.xml"); // chpark 090413 : xml 파일명을 명령어 인자로 처리
	HiKernel::HiViewer::getInstance()->Config(hWnd, hInst);  // chpark 090413 : 윈도우 생성 전후에 해야 할지 결정 필요

	
	/// 윈도우 출력
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

	// HiViewer 소멸
	HiKernel::HiViewer::getInstance()->Terminate();
	delete _viewer;

	// 윈도우 클래스 등록 해지
	UnregisterClass( "HiClient", wc.hInstance );

	return 0;
}


void main()
{
	WinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOW);
}
