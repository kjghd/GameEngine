#include "GameFunctions.h"
#include "Input.h"
#include "Graphics.h"
#include "Audio.h"
#include "Timer.h"
#include <random>

// ToDo
// Create land
// make thing walk accross it

static Input input;
static Graphics graphics;
static Audio audio;
static Timer timer;

ID2D1SolidColorBrush* pBrush;
ID2D1GeometryGroup* pTerrain;
D2D1_ELLIPSE cursor;
float cursorSize;

void Game_Setup(HWND hWnd)
{
	graphics.Setup(hWnd);
	audio.Setup();
	timer.Start();
	
	graphics.CreateBrush(&pBrush, D2D1::ColorF(1.f, 1.f, 1.f));
	cursorSize = 20.f;
	cursor.radiusX = cursorSize;
	cursor.radiusY = cursorSize;
}

void Game_UpdateInput(LPMSG lpMsg)
{
	input.SetUnchanged();

	switch (lpMsg->message)
	{
	case WM_MOUSEMOVE:	 input.UpdateMousePosition(lpMsg->lParam); break;
	case WM_LBUTTONDOWN: input.SetMouseDown();					   break;
	case WM_LBUTTONUP:	 input.SetMouseUp();					   break;
	case WM_KEYDOWN:	 input.SetButtonsOn(lpMsg->wParam);		   break;
	case WM_KEYUP:		 input.SetButtonsOff(lpMsg->wParam);	   break;
	}
}

void Game_UpdateLogic()
{
	cursor.point = input.mouse;
	if (input.CheckPressed(BID_BRACKET_SQR_L))
	{
		cursorSize /= 1.2f;
		cursor.radiusX = cursorSize;
		cursor.radiusY = cursorSize;
	}
	if (input.CheckPressed(BID_BRACKET_SQR_R))
	{
		cursorSize *= 1.2f;
		cursor.radiusX = cursorSize;
		cursor.radiusY = cursorSize;
	}
	if (input.CheckHeld(BID_LMB))
	{
		if (input.CheckHeld(BID_CTRL_L))
			graphics.EraseTerrain(&pTerrain, input.mouse, cursorSize);
		else
			graphics.AddTerrain(&pTerrain, input.mouse, cursorSize);
	}
}

void Game_RenderGraphics()
{
	graphics.BeginDraw();
	graphics.ClearScreen();
	graphics.FillGeometry(pTerrain, pBrush);
	graphics.DrawEllipse(cursor, pBrush);
	graphics.EndDraw();
}

void Game_PlayAudio()
{

}

void Game_ResetTimer()
{
	timer.Reset();
}