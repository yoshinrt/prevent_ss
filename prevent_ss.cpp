/*****************************************************************************

		windows application skelton  v1.00
		Copyright (C) 2000 by Deyu Deyu Software

*****************************************************************************/

//#define DEBUG
#include <windows.h>
#include <tchar.h>
#include <wtsapi32.h>
#include <stdio.h>

/*** constant definition ****************************************************/

#define WINDOW_TITLE	_T( "Sample window" )		// NULL 可
#define WINDOW_CLASS	_T( "SampleAppClass" )

/*** gloval var definition **************************************************/

const TCHAR	*szWinClassName = WINDOW_CLASS;

HINSTANCE	g_hInst;
HWND		g_hWnd;
POINT		g_Point;

#ifdef DEBUG
const char *g_szWtsMsg[] = {
	"WTS_0",						// 0x0
	"WTS_CONSOLE_CONNECT",			// 0x1
	"WTS_CONSOLE_DISCONNECT",		// 0x2
	"WTS_REMOTE_CONNECT",			// 0x3
	"WTS_REMOTE_DISCONNECT",		// 0x4
	"WTS_SESSION_LOGON",			// 0x5
	"WTS_SESSION_LOGOFF",			// 0x6
	"WTS_SESSION_LOCK",				// 0x7
	"WTS_SESSION_UNLOCK",			// 0x8
	"WTS_SESSION_REMOTE_CONTROL",	// 0x9
	"WTS_SESSION_CREATE",			// 0xA
	"WTS_SESSION_TERMINATE",		// 0xB
};
#endif

/*** Debug log **************************************************************/

#ifdef DEBUG
void Log( const char *szMsg ){
	SYSTEMTIME Time;
	GetLocalTime( &Time );
	printf("%04d/%02d/%02d %02d:%02d:%02d %s\n",
		Time.wYear, Time.wMonth, Time.wDay,
		Time.wHour, Time.wMinute, Time.wSecond,
		szMsg
	);
}
#else
	#define Log( msg )
#endif


/*** window procedure *******************************************************/

LRESULT CALLBACK WindowProc(
	HWND	hWnd,
	UINT	Message,
	WPARAM	wParam,
	LPARAM	lParam ){
	
	POINT	Point;
	static BOOL bEnableMouseMove = TRUE;
	
	switch( Message ){
	  case WM_CREATE:
		WTSRegisterSessionNotification( hWnd, NOTIFY_FOR_ALL_SESSIONS );
		
		#ifdef DEBUG
			AllocConsole();
			freopen ( "CONOUT$", "w", stdout );
			freopen ( "CONIN$", "r", stdin );
		#endif
		break;
		
	  case WM_DESTROY:				/* terminated by user					*/
		WTSUnRegisterSessionNotification( hWnd );
		PostQuitMessage( 1 );
		break;
		
	  case WM_WTSSESSION_CHANGE:
		Log( g_szWtsMsg[ wParam ]);
		
		// ログインタイマー起動
		if( wParam == WTS_SESSION_LOCK ){
			Log( "MM disabled");
			bEnableMouseMove = FALSE;
		}else if( wParam == WTS_SESSION_UNLOCK ){
			Log( "MM enabled");
			bEnableMouseMove = TRUE;
		}
		break;
		
	  case WM_TIMER:
		if( bEnableMouseMove ){
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
		}
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
