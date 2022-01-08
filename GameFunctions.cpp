#include "GameFunctions.h"
#include "Input.h"
#include "Graphics.h"
#include "Audio.h"
#include "Timer.h"
#include <random>

static Input input;
static Graphics graphics;
static Audio audio;
static Timer timer;

ID2D1PathGeometry* pPath;
ID2D1EllipseGeometry* pEllipse;
ID2D1SolidColorBrush* pBrush;


void Game_Setup(HWND hWnd)
{
	graphics.Setup(hWnd);
	audio.Setup();
	timer.Start();
	graphics.GenerateTerrain(2, &pPath, hWnd);
	graphics.CreateBrush(&pBrush, { 1,1,1,1 });
	graphics.CreateEllipse(&pEllipse, { 0,0 }, 50);
}

void Game_UpdateInput(LPMSG lpMsg)
{
	input.SetUnchanged();

	switch (lpMsg->message)
	{
	case WM_KEYDOWN: input.SetButtonsOn(lpMsg->wParam);  break;
	case WM_KEYUP:	 input.SetButtonsOff(lpMsg->wParam); break;
	}
}

void Game_UpdateLogic()
{
}

void Game_RenderGraphics()
{
	graphics.BeginDraw();
	graphics.ClearScreen();
	graphics.RemoveAlpha(&pPath, pEllipse, { 100,210 });
	//graphics.DrawGeometry(pEllipse, pBrush);
	graphics.DrawGeometry(pPath, pBrush);
	graphics.EndDraw();
}

void Game_PlayAudio()
{

}

void Game_ResetTimer()
{
	timer.Reset();
}