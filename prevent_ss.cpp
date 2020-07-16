/*****************************************************************************

		windows application skelton  v1.00
		Copyright (C) 2000 by Deyu Deyu Software

*****************************************************************************/

#include <windows.h>
#include <tchar.h>

/*** constant definition ****************************************************/

#define WINDOW_TITLE	_T( "Sample window" )		// NULL 可
#define WINDOW_CLASS	_T( "SampleAppClass" )

/*** gloval var definition **************************************************/

const TCHAR	*szWinClassName = WINDOW_CLASS;

HINSTANCE	g_hInst;
HWND		g_hWnd;
POINT		g_Point;

/*** window procedure *******************************************************/

LRESULT CALLBACK WindowProc(
	HWND	hWnd,
	UINT	Message,
	WPARAM	wParam,
	LPARAM	lParam ){
	
	POINT	Point;
	
	switch( Message ){
	  case WM_TIMER:
		GetCursorPos( &Point );
		
		if( Point.x == g_Point.x && Point.y == g_Point.y ){
			mouse_event(
				MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,
				Point.x * 65535 / ( GetSystemMetrics( SM_CXSCREEN ) - 1 ),
				Point.y * 65535 / ( GetSystemMetrics( SM_CYSCREEN ) - 1 ),
				0, 0
			);
		}
		g_Point = Point;
		
		SetThreadExecutionState( ES_DISPLAY_REQUIRED );
		break;
		
	  case WM_DESTROY:				/* terminated by user					*/
		PostQuitMessage( 1 );
		break;
		
	  default:
		return( DefWindowProc( hWnd, Message, wParam, lParam ));
	}
	return( 0 );
}

/*** main procedure *********************************************************/

int WINAPI _tWinMain(
	HINSTANCE	hInst,
	HINSTANCE	hPrevInst,
	LPTSTR		lpCmdLine,
	int			iCmdShow ){
	
	/* for creating window */
	
	HWND		hWnd;
	MSG			Msg;
	WNDCLASS	wcl;
	
	/*** cleate a window ****************************************************/
	
	g_hInst = hInst;
	
	/* define a window class */
	
	wcl.hInstance		= hInst;
	wcl.lpszClassName	= szWinClassName;
	wcl.lpfnWndProc		= WindowProc;
	wcl.style			= 0;
	
	wcl.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wcl.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wcl.lpszMenuName	= NULL;
	
	wcl.cbClsExtra		= 0;
	wcl.cbWndExtra		= 0;
	
	wcl.hbrBackground	=( HBRUSH )GetStockObject( WHITE_BRUSH );
	
	/* register the window class */
	
	if( !RegisterClass( &wcl )) return( 1 );
	
	g_hWnd = hWnd = CreateWindow(
		szWinClassName,
		WINDOW_TITLE,			// window title
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,			// x
		CW_USEDEFAULT,			// y
		CW_USEDEFAULT,			// w
		CW_USEDEFAULT,			// h
		HWND_DESKTOP,
		NULL,
		hInst,
		NULL
	);
	
	/* display the window */
	
	//ShowWindow( hWnd, iCmdShow );
	UpdateWindow( hWnd );
	
	// タイマ設定
	GetCursorPos( &g_Point );
	SetTimer( g_hWnd, 0/*ID_TIMER*/, 60 * 1000, NULL );
	
	/* create the message loop */
	
	while( GetMessage( &Msg, NULL, 0, 0 )){
		TranslateMessage( &Msg );
		DispatchMessage( &Msg );
	}
	
	return( 0 );
}
