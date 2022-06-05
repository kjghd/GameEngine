#pragma once

#include "Component_Base.h"
#include "Graphics.h"
#include <vector>

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

	virtual ID2D1Bitmap* GetBitmap()
	{
		return m_pBitmap;
	}
};

class SpriteSheet : public Sprite
{
public:
	std::vector<D2D1_RECT_F> m_vSpriteRects;
	unsigned int m_currentSprite;
	float m_timeBetweenFrames;
	float m_timeOnFrame;

	SpriteSheet(Location* pOrigin, Location offset, Size size, float rotation, float frameTime, UINT uSpriteCount, std::vector<D2D1_RECT_F>* pvSpriteRects, Graphics* pGraphics, LPCWSTR lpFileName)
		:
		Sprite(pOrigin, offset, size, rotation, pGraphics, lpFileName),
		m_timeBetweenFrames(frameTime),
		m_timeOnFrame(0),
		m_currentSprite(0)
	{
		m_vSpriteRects.swap(*pvSpriteRects);
	}

	D2D1_RECT_F GetCurrentRect()
	{
		return m_vSpriteRects.at(m_currentSprite);
	}

	void NextFrame()
	{
		m_timeOnFrame += g_timer.deltaTime;

		if (m_timeOnFrame >= m_timeBetweenFrames)
		{
			m_timeOnFrame = 0;
			if (m_currentSprite < m_vSpriteRects.size() - 1)
				++m_currentSprite;
			else
				m_currentSprite = 0;
		}
	}


};

class MultiSpriteSheet : public SpriteSheet
{

};