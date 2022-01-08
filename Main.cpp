#include <Windows.h>
#include <sstream>
#include "DIPs.h"
#include "GameFunctions.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY)
	{
		PostQuitMessage(0);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// Window
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"MainWindow";
	wc.lpfnWndProc = WndProc;
	RegisterClassEx(&wc);

	RECT rc{ 0,0,640,480 };
	AdjustWindowRect(&rc, WS_CAPTION, FALSE);

	HWND hWnd = CreateWindowEx(
		NULL,
		L"MainWindow",
		L"Window",
		WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hWnd, nCmdShow);

	// Device independant pixels
	InitDIPs(static_cast<float>(GetDpiForWindow(hWnd)));

	// Game Setup.
	Game_Setup(hWnd);

	// Main loop.
	MSG msg{NULL};
	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Game Functions
		Game_UpdateInput(&msg);
		Game_UpdateLogic();
		Game_RenderGraphics();
		Game_PlayAudio();
		Game_ResetTimer();
	}
	return 0;
}