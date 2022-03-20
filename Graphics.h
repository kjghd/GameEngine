#pragma once
#include "DIPs.h"
#include<atlcomcli.h> // CComPtr
#include <d2d1.h>
#include <wincodec.h>
#include <random>
#pragma comment(lib, "d2d1")


class Graphics
{
	

	//CComPtr<ID2D1Factory> pFactory;
	//CComPtr<ID2D1HwndRenderTarget> pRenderTarget;
	//
	//CComPtr<IWICImagingFactory> pImagingFactory;

	// change to CComPtrs, these don't get released.
	ID2D1Factory* pFactory;
	ID2D1HwndRenderTarget* pRenderTarget;

	IWICImagingFactory* pImagingFactory;


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

		if (SUCCEEDED(hr))
		{
			hr = CoCreateInstance(CLSID_WICImagingFactory, pImagingFactory, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pImagingFactory));
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

	void TransformRenderTarget(D2D1_MATRIX_3X2_F transform)
	{
		pRenderTarget->SetTransform(transform);
	}

	void LoadBitmapFromFile(LPCWSTR lpFileName, ID2D1Bitmap** ppBitmap)
	{
		CComPtr<IWICBitmapDecoder> pDecoder;
		HRESULT hr = pImagingFactory->CreateDecoderFromFilename(lpFileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
		if (SUCCEEDED(hr))
		{
			CComPtr<IWICBitmapFrameDecode> pFrame;
			hr = pDecoder->GetFrame(0, &pFrame);
			if (SUCCEEDED(hr))
			{
				CComPtr<IWICFormatConverter> pFormatConverter;
				hr = pImagingFactory->CreateFormatConverter(&pFormatConverter);
				if (SUCCEEDED(hr))
				{
					pFormatConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeMedianCut);
					if (SUCCEEDED(hr))
					{
						hr = pRenderTarget->CreateBitmapFromWicBitmap(pFormatConverter, ppBitmap);
					}
				}
			}
		}
		//if (SUCCEEDED(hr))
		//{
		//	OutputDebugString(L"Loaded bitmap from file: ");
		//	OutputDebugString(lpFileName);
		//	OutputDebugString(L"\n");
		//}
		//else
		//{
		//	OutputDebugString(L"Failed to load bitmap from file: ");
		//	OutputDebugString(lpFileName);
		//	OutputDebugString(L"\n");
		//}
	}

	void CreateBrush(ID2D1SolidColorBrush** ppBrush, D2D1_COLOR_F color)
	{
		pRenderTarget->CreateSolidColorBrush(color, ppBrush);
	}

	void ClearScreen(float red = 0, float green = 0, float blue = 0)
	{
		pRenderTarget->Clear(D2D1::ColorF(red, green, blue));
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
	void DrawRect(D2D1_RECT_F rect, ID2D1Brush* pBrush, FLOAT strokeWidth = 1.0f)
	{
		rect = D2D1::RectF(
			FromDIPs(rect.left),
			FromDIPs(rect.top),
			FromDIPs(rect.right),
			FromDIPs(rect.bottom)
		);
		pRenderTarget->DrawRectangle(rect, pBrush, strokeWidth);
	}
	void FillRect(D2D1_RECT_F rect, ID2D1Brush* pBrush)
	{
		rect = D2D1::RectF(
			FromDIPs(rect.left),
			FromDIPs(rect.top),
			FromDIPs(rect.right),
			FromDIPs(rect.bottom)
		);
		pRenderTarget->FillRectangle(rect, pBrush);
	}

	void DrawLine(D2D1_POINT_2F startPoint, D2D1_POINT_2F endPoint, ID2D1Brush* pBrush, FLOAT strokeWidth = 1.0f)
	{
		startPoint = { FromDIPs(startPoint.x), FromDIPs(startPoint.y) };
		endPoint = { FromDIPs(endPoint.x), FromDIPs(endPoint.y) };

		pRenderTarget->DrawLine(startPoint, endPoint, pBrush, strokeWidth);
	}
	void FillBitmap(ID2D1Bitmap* pBitmap, D2D1_RECT_F rect, float rotation)
	{
		rect = {
			FromDIPs(rect.left),
			FromDIPs(rect.top),
			FromDIPs(rect.right),
			FromDIPs(rect.bottom)
		};

			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(rotation, D2D1::Point2F(rect.left + (rect.right - rect.left) / 2.f, rect.top + (rect.bottom - rect.top) / 2.f)));
			pRenderTarget->DrawBitmap(pBitmap, rect, 1.f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}
};
