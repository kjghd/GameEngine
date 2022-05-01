#include "GameStructures.h"
#include "GameFunctions.h"
#include "Input.h"
#include "Graphics.h"
#include "Audio.h"
#include "Timer.h"

// Game Objects
#include "GameObject.h"
#include "Player.h"

// Scene
#include "Scene.h"

#include <vector>
#include <memory>

Graphics graphics;
Audio audio;
Input input;
Scene scene;

// Preset Objects.

Player* player;
GameObject* block;


void Game_Setup(HWND hWnd)
{
	graphics.Setup(hWnd);
	audio.Setup();
	g_timer.Start();

	player = new Player("player", { 0,0 }, 0.01f, 100.f, &input);
	player->AddComponent(View(&player->m_location, { 0,0 }, 64, { 640,480 }, &graphics));
	player->AddComponent(Sprite(&player->m_location, { 0,0 }, { 1,1 }, 0, &graphics, L"Assets/Images/human.png"));
	player->AddComponent(Collision_Box(&player->m_location, { 0,0 }, true, { 1,1 }));

	block = new GameObject("block", { 0,0 }, false);
	block->AddComponent(Sprite(&block->m_location, { 0,0 }, { 1,1 }, 0, &graphics, L"Assets/Images/block.png"));
	block->AddComponent(Collision_Box(&block->m_location, { 0,0 }, true, { 1,1 }));



	scene.Spawn(player);
	//scene.Spawn(block, { 2,1 });
	//scene.Spawn(block, { -1,-1 });
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
	case WM_MOUSEWHEEL:	 input.scroll = GET_WHEEL_DELTA_WPARAM(lpMsg->wParam); break;
	}
}

void Game_UpdateLogic()
{
	scene.Update();
}

void Game_RenderGraphics()
{
	graphics.BeginDraw();
	graphics.ClearScreen(0.09f, 0.09f, 0.32f);

	scene.Render();

	graphics.EndDraw();
}

void Game_PlayAudio()
{

}

void Game_ResetTimer()
{
	g_timer.Reset();
}
