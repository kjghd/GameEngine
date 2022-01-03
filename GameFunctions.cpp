#include "GameFunctions.h"
#include "Input.h"
#include "Graphics.h"
#include "Audio.h"
#include "Timer.h"

Input input;
Graphics graphics;
Audio audio;
Timer timer;


void Game_Setup(HWND hWnd)
{
	graphics.Setup(hWnd);
	audio.Setup();
	timer.Start();
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
	graphics.EndDraw();
}

void Game_PlayAudio()
{

}

void Game_ResetTimer()
{
	timer.Reset();
}