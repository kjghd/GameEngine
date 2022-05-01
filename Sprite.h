#pragma once

#include "Component_Base.h"
#include "Graphics.h"

class Sprite : public Component_Base
{
public:
	ID2D1Bitmap* m_pBitmap;

	Size m_size;
	float m_rotation;

	Sprite(Location* pOrigin, Location offset, Size size, float rotation, Graphics* pGraphics, LPCWSTR lpFileName)
		:
		Component_Base(pOrigin, offset),
		m_pBitmap(NULL),
		m_size(size),
		m_rotation(rotation)
	{
		pGraphics->LoadBitmapFromFile(lpFileName, &m_pBitmap);
	}
};
