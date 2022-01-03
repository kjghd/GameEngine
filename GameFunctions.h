#pragma once
#include <Windows.h>

// Set up
void Game_Setup(HWND hWnd);

// Recieve input.
void Game_UpdateInput(LPMSG lpMsg);

// Update logic.
void Game_UpdateLogic();

// Render graphics.
void Game_RenderGraphics();

// Play sound
void Game_PlayAudio();

void Game_ResetTimer();

