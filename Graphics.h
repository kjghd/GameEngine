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

	float windowDPI;

	float ToDIPs(float value)
	{
		return value * (windowDPI / 96.f);
	}

	float FromDIPs(float value)
	{
		return value / (windowDPI / 96.f);
	}
	
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

		// DPI
		windowDPI = static_cast<float>(GetDpiForWindow(hWnd));
		pRenderTarget->SetDpi(windowDPI, windowDPI);
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

	void FillGeometry(ID2D1Geometry* pGeometry, ID2D1Brush* pBrush)
	{
		if (pGeometry) pRenderTarget->FillGeometry(pGeometry, pBrush);
	}

	void DrawEllipse(D2D1_ELLIPSE ellipse, ID2D1Brush* pBrush)
	{
		ellipse = D2D1::Ellipse(
			{ FromDIPs(ellipse.point.x), FromDIPs(ellipse.point.y) },
			FromDIPs(ellipse.radiusX),
			FromDIPs(ellipse.radiusY)
		);
		pRenderTarget->DrawEllipse(ellipse, pBrush);
	}

	void AddTerrain(ID2D1GeometryGroup** ppGeoGroup, D2D1_POINT_2F location, float size)
	{
		location = D2D1::Point2F(FromDIPs(location.x), FromDIPs(location.y));
		size = FromDIPs(size);

		D2D1_ELLIPSE ellipse{ D2D1::Ellipse(location, size, size) };
		ID2D1EllipseGeometry* pEllipseGeo{};
		pFactory->CreateEllipseGeometry(ellipse, &pEllipseGeo);
		
		if (*ppGeoGroup)
		{
			ID2D1PathGeometry* pPath{};
			pFactory->CreatePathGeometry(&pPath);
			ID2D1GeometrySink* pSink{};
			pPath->Open(&pSink);
			(*ppGeoGroup)->CombineWithGeometry(pEllipseGeo, D2D1_COMBINE_MODE_UNION, NULL, pSink);
			pSink->Close();

			ID2D1Geometry* paGeos[] = { pPath };
			(*ppGeoGroup)->Release();
			pFactory->CreateGeometryGroup(D2D1_FILL_MODE_WINDING, paGeos, 1, ppGeoGroup);
		}
		else
		{
			ID2D1Geometry* paGeos[] = { pEllipseGeo };
			pFactory->CreateGeometryGroup(D2D1_FILL_MODE_WINDING, paGeos, 1, ppGeoGroup);
		}


	}

	void EraseTerrain(ID2D1GeometryGroup** ppGeoGroup, D2D1_POINT_2F location, float size)
	{
		location = D2D1::Point2F(FromDIPs(location.x), FromDIPs(location.y));
		size = FromDIPs(size);

		D2D1_ELLIPSE ellipse{ D2D1::Ellipse(location, size, size) };
		ID2D1EllipseGeometry* pEllipseGeo{};
		pFactory->CreateEllipseGeometry(ellipse, &pEllipseGeo);

		if (*ppGeoGroup)
		{
			ID2D1PathGeometry* pPath{};
			pFactory->CreatePathGeometry(&pPath);
			ID2D1GeometrySink* pSink{};
			pPath->Open(&pSink);
			(*ppGeoGroup)->CombineWithGeometry(pEllipseGeo, D2D1_COMBINE_MODE_EXCLUDE, NULL, pSink);
			pSink->Close();

			ID2D1Geometry* paGeos[] = { pPath };
			(*ppGeoGroup)->Release();
			pFactory->CreateGeometryGroup(D2D1_FILL_MODE_WINDING, paGeos, 1, ppGeoGroup);
		}
	}

};