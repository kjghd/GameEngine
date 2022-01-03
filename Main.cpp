#include "Graphics.h"
#include "Audio.h"
#include "Input.h"
#include "Timer.h"
#include <Windows.h>
#include <sstream>

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
	HRESULT hr;

	// Window
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"MainWindow";
	wc.lpfnWndProc = WndProc;
	RegisterClassEx(&wc);

	RECT rc{ 0,0,640,480 };
	if (0 == AdjustWindowRect(&rc, WS_CAPTION, FALSE))
		hr = GetLastError();

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

	// Input
	Input input;

	// Graphics
	Graphics graphics;
	graphics.Setup(hWnd);

	// Audio
	Audio audio;
	audio.Setup();

	// Time setup
	Timer timer;
	timer.Start();

	MSG msg{NULL};
	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Input
		input.SetUnchanged();
		if (msg.message == WM_KEYDOWN)
			switch (msg.wParam)
			{
			case 0x41: input.SetOn(BID_A); break;
			case 0x44: input.SetOn(BID_D); break;
			case 0x53: input.SetOn(BID_S); break;
			case 0x57: input.SetOn(BID_W); break;
			}
		if (msg.message == WM_KEYUP)
			switch (msg.wParam)
			{
			case 0x41: input.SetOff(BID_A); break;
			case 0x44: input.SetOff(BID_D); break;
			case 0x53: input.SetOff(BID_S); break;
			case 0x57: input.SetOff(BID_W); break;
			}

		// Logic
		

		// Graphics
		graphics.BeginDraw();
		graphics.ClearScreen();
		graphics.EndDraw();

		// Sound


		// Reset timer
		timer.Reset();
	}

	return 0;
}