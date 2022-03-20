#pragma once
#include <Windows.h>

void Game_Setup(HWND hWnd);

void Game_UpdateInput(LPMSG lpMsg);

void Game_UpdateLogic();

void Game_RenderGraphics();

void Game_PlayAudio();

void Game_ResetTimer();

