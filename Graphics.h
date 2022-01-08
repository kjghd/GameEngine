#pragma once
#include "DIPs.h"
#include<atlcomcli.h> // CComPtr
#include <d2d1.h>
#include <random>
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


				float dpi{ static_cast<float>(GetDpiForWindow(hWnd)) };
				pRenderTarget->SetDpi(dpi,dpi);

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

	void CreateBrush(ID2D1SolidColorBrush** ppBrush, D2D1_COLOR_F color)
	{
		pRenderTarget->CreateSolidColorBrush(color, ppBrush);
	}

	void ClearScreen(float red = 0, float green = 0, float blue = 0)
	{
		pRenderTarget->Clear(D2D1::ColorF(red, green, blue));
	}

	void DrawGeometry(ID2D1Geometry* pGeometry, ID2D1Brush* pBrush)
	{
		pRenderTarget->FillGeometry(pGeometry, pBrush);
	}

	void RemoveAlpha(ID2D1PathGeometry** ppPath, ID2D1Geometry* pGeometry, D2D1_SIZE_F location)
	{
		ID2D1GeometrySink* pSink{ NULL };

		ID2D1PathGeometry* pPath{ NULL };
		pFactory->CreatePathGeometry(&pPath);
		pPath->Open(&pSink);

		(*ppPath)->CombineWithGeometry(pGeometry, D2D1_COMBINE_MODE_EXCLUDE, D2D1::Matrix3x2F::Translation(location), pSink);

		pSink->Close();

		(*ppPath)->Release();
		*ppPath = pPath;
	}

	void CreateEllipse(ID2D1EllipseGeometry** ppGeometry, D2D1_POINT_2F location, float size)
	{
		pFactory->CreateEllipseGeometry(D2D1::Ellipse(location, size, size), ppGeometry);
	}

	void GenerateTerrain(unsigned int seed, ID2D1PathGeometry** ppPath, HWND hWnd)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		D2D1_RECT_F screen{
			static_cast<float>(rc.left),
			static_cast<float>(rc.top),
			static_cast<float>(rc.right),
			static_cast<float>(rc.bottom)
		};

		srand(seed);

		ID2D1GeometrySink* pSink{ NULL };
		pFactory->CreatePathGeometry(ppPath);
		(*ppPath)->Open(&pSink);

		// Start
		pSink->BeginFigure(
			D2D1::Point2F(ToDIPs(screen.left), ToDIPs(screen.bottom / 2.f)),
			D2D1_FIGURE_BEGIN_FILLED
		);

		long prev = screen.bottom / 2;
		long range = 3;
		long rangeStart = prev - range / 2;

		float num = rand() % range + rangeStart;

		// Randomise
		for (size_t i = 0; i < screen.right; i++)
		{
			pSink->AddLine({ToDIPs(i),ToDIPs( num ) });
			prev = num;
			rangeStart = prev - range / 2;
			num = rand() % range + rangeStart;
		}

		// End
		pSink->AddLine(D2D1::Point2F(ToDIPs((float)screen.right), ToDIPs((float)screen.bottom / 2.f)));
		pSink->AddLine(D2D1::Point2F(ToDIPs((float)screen.right), ToDIPs((float)screen.bottom )));
		pSink->AddLine(D2D1::Point2F(ToDIPs((float)screen.left ), ToDIPs((float)screen.bottom )));
		pSink->AddLine(D2D1::Point2F(ToDIPs((float)screen.left ), ToDIPs((float)screen.bottom / 2.f)));
		pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

		pSink->Close();

	}
};