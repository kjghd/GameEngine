#include <Windows.h>
#include "Graphics.h"
#include "Audio.h"
#include "Input.h"

Input input;
Audio audio;
Graphics graphics;

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
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"MainWindow";
	wc.lpfnWndProc = WndProc;
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(
		NULL,
		L"MainWindow",
		L"Window",
		WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);

	graphics.Setup(hWnd);
	audio.Setup();

	float colour{};

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
		graphics.ClearScreen(colour);
		graphics.EndDraw();
	}

	return 0;
}