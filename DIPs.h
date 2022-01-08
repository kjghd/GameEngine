#pragma once

inline float DIPs_scale;

static void InitDIPs(float dpi)
{
	DIPs_scale = dpi / 96.f;
}

static float ToDIPs(float value)
{
	return value / DIPs_scale;
}

static float FromDIPs(float value)
{
	return value * DIPs_scale;
}