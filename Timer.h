#pragma once
#include <Windows.h>

class Timer
{
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;
	LARGE_INTEGER difference;
	LARGE_INTEGER frequency;

public:
	float deltaTime;

	void Start()
	{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&startTime);
	}

	void Reset()
	{
		QueryPerformanceCounter(&endTime);
		difference.QuadPart = endTime.QuadPart - startTime.QuadPart;
		QueryPerformanceCounter(&startTime);
		deltaTime = static_cast<float>(difference.QuadPart) / static_cast<float>(frequency.QuadPart) * 1000.f; // 1000 gives time in miliseconds.
	}
};

static Timer g_timer;