#pragma once
#include <Windows.h>

class MainWindow
{
	HINSTANCE* phInstance;
	LPCWSTR lpClassName;
	WNDCLASSEX wndClass;
	HWND hWnd;

	MainWindow(HINSTANCE* hInstance)
		:
		phInstance(hInstance),
		lpClassName(L"MainWindowClass"),
		wndClass(),
		hWnd()
	{
	}

	void Create(DWORD dwExStyle, DWORD dwStyle, LPCWSTR lpWindowName, int nWidth, int nHeight)
	{
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.lpszClassName = lpClassName;
		wndClass.hInstance = *phInstance;

		RegisterClassEx(&wndClass);

		RECT rc{ 0, 0, nWidth, nHeight };
		AdjustWindowRect(&rc, dwStyle, FALSE);

		hWnd = CreateWindowEx(
			dwExStyle,
			lpClassName,
			lpWindowName,
			dwStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rc.right - rc.left,
			rc.bottom - rc.top,
			NULL,
			NULL,
			*phInstance,
			0);
	}
};