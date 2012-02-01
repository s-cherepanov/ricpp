#include <Windows.h>


void drawMyWindow(
	HWND hwnd,
	HDC hdc,
	LPPAINTSTRUCT lpPaint,
	LPRECT lpClientRect)
{
	DrawText(hdc, TEXT("Hello, Windows!"), -1, lpClientRect, 
		DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
}


bool doIdle(HINSTANCE hInstance)
{
	return false;
}


void cleanUp(HINSTANCE hInstance, WPARAM rval)
{
}


LRESULT CALLBACK wndProc(
  __in  HWND hwnd,
  __in  UINT uMsg,
  __in  WPARAM wParam,
  __in  LPARAM lParam)
{
	switch ( uMsg ) {
		case WM_CREATE: {
			return 0;
		}

		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			RECT rect = {0, 0, 0, 0};

			GetClientRect(hwnd, &rect) ;		
			SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
			SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));

			drawMyWindow(hwnd, hdc, &ps, &rect);

			EndPaint(hwnd, &ps);
			return 0;
		}

		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}

		default: {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}


HWND createMainWnd(HINSTANCE hInstance)
{
	static const WCHAR *myWndClassname = TEXT("MyWndClassName");
	static const WNDCLASSEX myWndClass = {
		sizeof(WNDCLASSEX),    // cbSize
		CS_VREDRAW|CS_HREDRAW, // style
		(WNDPROC) wndProc,     // lpfnWndProc
		0,                     // cbClsExtra
		0,                     // cbWndExtra
		hInstance,             // hInstance
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)), // hIcon
		LoadCursor(0, IDC_ARROW),       // hCursor
		GetSysColorBrush(COLOR_WINDOW), // hbrBackground
		0,                     // lpszMenuName
		myWndClassname,        // lpszClassName
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION))  // hIconSm
	};

	if ( !RegisterClassEx(&myWndClass) ) {
		return 0;
	}

	const int defWinPosX = 50;
	const int defWinPosY = 50;
	const int defWinWid  = 640;
	const int defWinHt   = 400;

	return CreateWindowEx(
		0,
		myWndClassname,
		TEXT("My Window"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		defWinPosX,
		defWinPosY,
		defWinWid,
		defWinHt,
		0,
		0,
		hInstance,
		0);
}


LRESULT treatMessage(const MSG *lpMsg)
{
	TranslateMessage(lpMsg);
	return DispatchMessage(lpMsg);
}


WPARAM mainLoop(HINSTANCE hInstance)
{
	MSG msg;
	while ( GetMessage(&msg, 0, 0, 0) ) {
		treatMessage(&msg);
	}

	return msg.wParam;
}


WPARAM pollingMainLoop(HINSTANCE hInstance)
{
	MSG msg;
	for (;;) {
		while ( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) ) {
			if ( msg.message == WM_QUIT ) {
				return msg.wParam;
			}
			treatMessage(&msg);
		}
		doIdle(hInstance);
	}
}


WPARAM idleMainLoop(HINSTANCE hInstance)
{
	MSG msg;
	for (;;) {
		do {
			while ( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) ) {
				if ( msg.message == WM_QUIT ) {
					return msg.wParam;
				}
				treatMessage(&msg);
			}
		} while ( doIdle(hInstance) );
		if ( !GetMessage(&msg, 0, 0, 0) ) {
			return msg.wParam;
		}
		treatMessage(&msg);
	}
}


int CALLBACK WinMain(
  __in  HINSTANCE hInstance,
  __in  HINSTANCE hPrevInstance,
  __in  LPSTR lpCmdLine,
  __in  int nCmdShow)
{
	if ( !createMainWnd(hInstance) ) {
		return 0;
	}

	WPARAM rval = idleMainLoop(hInstance);

	cleanUp(hInstance, rval);

	return rval;
}
