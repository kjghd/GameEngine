#pragma once
#include<atlcomcli.h> // CComPtr
#include <d2d1.h>
#pragma comment(lib, "d2d1")

class Graphics
{
	CComPtr<ID2D1Factory> pFactory;
	CComPtr<ID2D1HwndRenderTarget> pRenderTarget;

public:
	~Graphics()
	{
		CoUninitialize(); // Execute before CComPtr's release?
	}

	void Setup(HWND hWnd)
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if (SUCCEEDED(hr))
		{
			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
			if (SUCCEEDED(hr))
			{
				RECT rc;
				GetClientRect(hWnd, &rc);

				hr = pFactory->CreateHwndRenderTarget(
					D2D1::RenderTargetProperties(),
					D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rc.right, rc.bottom)),
					&pRenderTarget);
			}
		}
	}

	void BeginDraw()
	{
		pRenderTarget->BeginDraw();
	}

	void EndDraw()
	{
		HRESULT hr = pRenderTarget->EndDraw();
	}

	void ClearScreen(float red = 0, float green = 0, float blue = 0)
	{
		pRenderTarget->Clear(D2D1::ColorF(red, green, blue));
	}
};