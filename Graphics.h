#pragma once
#include <wrl.h>
#include <atlbase.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_3.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wincodec.h>
#include <random>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "D3D11")
#pragma comment(lib, "Dxgi")

class Graphics
{
	// D3D
	ID3D11Device* pD3DDevice;
	ID3D11DeviceContext* pD3DContext;

	// DXGI
	IDXGIDevice* pDXGIDevice;
	IDXGISwapChain1* pDXGISwapChain;

	// D2D
	
	ID2D1Factory1* pD2DFactory;
	ID2D1Device* pD2DDevice;
	ID2D1DeviceContext* pD2DContext;
	ID2D1Bitmap1* pBackBuffer;

	// WIC
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
			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
		}

		if (SUCCEEDED(hr))
		{
			hr = CoCreateInstance(CLSID_WICImagingFactory, pImagingFactory, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pImagingFactory));
		}

		if (SUCCEEDED(hr))
		{
			/**/
			// Create the D3D device.
			hr = D3D11CreateDevice(
				NULL,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				D3D11_CREATE_DEVICE_BGRA_SUPPORT,
				NULL,
				NULL,
				D3D11_SDK_VERSION,
				&pD3DDevice,
				NULL,
				&pD3DContext
			);

			// Obtain the DXGI device from the D3D device.
			hr = pD3DDevice->QueryInterface(&pDXGIDevice);

			// Create a D2D device associated with the DXGI device.
			hr = D2D1CreateDevice(pDXGIDevice, NULL, &pD2DDevice);

			// Create D2D fevice context.
			hr = pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &pD2DContext);



			// Get the factory to create a swap chain.
			IDXGIAdapter* pAdapter;
			pDXGIDevice->GetAdapter(&pAdapter);
			IDXGIFactory* pFactory;
			pAdapter->GetParent(IID_PPV_ARGS(&pFactory));

			// Description for the swapchain.
			DXGI_SWAP_CHAIN_DESC1 swapDesc;

			// Create swap chain for hWnd.
			reinterpret_cast<IDXGIFactory2*>(pFactory)->CreateSwapChainForHwnd(
				pDXGIDevice,
				hWnd,
				&swapDesc,
				NULL,
				NULL,
				&pDXGISwapChain
			);


		}
	}

	void BeginDraw()
	{
		pD2DContext->BeginDraw();
	}
	void EndDraw()
	{
		HRESULT hr = pD2DContext->EndDraw();

		hr = pDXGISwapChain->Present(1, 0);
	}

	void TransformRenderTarget(D2D1_MATRIX_3X2_F transform)
	{
		pD2DContext->SetTransform(transform);
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
						hr = pD2DContext->CreateBitmapFromWicBitmap(pFormatConverter, ppBitmap);
					}
				}
			}
		}
	}

	void CreateBrush(ID2D1SolidColorBrush** ppBrush, D2D1_COLOR_F color)
	{
		pD2DContext->CreateSolidColorBrush(color, ppBrush);
	}

	void ClearScreen(float red = 0, float green = 0, float blue = 0)
	{
		pD2DContext->Clear(D2D1::ColorF(red, green, blue));
	}

	void DrawEllipse(D2D1_ELLIPSE ellipse, ID2D1Brush* pBrush)
	{
		ellipse = D2D1::Ellipse(
			{ FromDIPs(ellipse.point.x), FromDIPs(ellipse.point.y) },
			FromDIPs(ellipse.radiusX),
			FromDIPs(ellipse.radiusY)
		);
		pD2DContext->DrawEllipse(ellipse, pBrush);
	}
	void DrawRect(D2D1_RECT_F rect, ID2D1Brush* pBrush, FLOAT strokeWidth = 1.0f)
	{
		rect = D2D1::RectF(
			FromDIPs(rect.left),
			FromDIPs(rect.top),
			FromDIPs(rect.right),
			FromDIPs(rect.bottom)
		);
		pD2DContext->DrawRectangle(rect, pBrush, strokeWidth);
	}
	void FillRect(D2D1_RECT_F rect, ID2D1Brush* pBrush)
	{
		rect = D2D1::RectF(
			FromDIPs(rect.left),
			FromDIPs(rect.top),
			FromDIPs(rect.right),
			FromDIPs(rect.bottom)
		);
		pD2DContext->FillRectangle(rect, pBrush);
	}

	void DrawLine(D2D1_POINT_2F startPoint, D2D1_POINT_2F endPoint, ID2D1Brush* pBrush, FLOAT strokeWidth = 1.0f)
	{
		startPoint = { FromDIPs(startPoint.x), FromDIPs(startPoint.y) };
		endPoint = { FromDIPs(endPoint.x), FromDIPs(endPoint.y) };

		pD2DContext->DrawLine(startPoint, endPoint, pBrush, strokeWidth);
	}
	void FillBitmap(ID2D1Bitmap* pBitmap, D2D1_RECT_F rect, float rotation)
	{
		rect = {
			FromDIPs(rect.left),
			FromDIPs(rect.top),
			FromDIPs(rect.right),
			FromDIPs(rect.bottom)
		};

		//pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(rotation, D2D1::Point2F(rect.left + (rect.right - rect.left) / 2.f, rect.top + (rect.bottom - rect.top) / 2.f)));
		pD2DContext->DrawBitmap(pBitmap, rect, 1.f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);


		//pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());




	}
};
