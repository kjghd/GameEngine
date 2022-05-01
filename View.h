#pragma once

#include "Component_Base.h"
#include "Collision_Base.h"
#include "Collision_Box.h"
#include "Sprite.h"
#include "Graphics.h"

class View : public Component_Base
{
	float m_pixelsPerUnit;
	Size m_screenSize_px;
	Graphics* m_pGraphics;
	ID2D1SolidColorBrush* m_pDebugBrush;

public:
	View(Location* pOrigin, Location offset, float pixelsPerUnit, Size screenSize_px, Graphics* pGraphics)
		:
		Component_Base(pOrigin, offset),
		m_pixelsPerUnit(pixelsPerUnit),
		m_screenSize_px(screenSize_px),
		m_pGraphics(pGraphics),
		m_pDebugBrush(NULL)
	{
		pGraphics->CreateBrush(&m_pDebugBrush, D2D1::ColorF(1.0f, 0.0f, 0.0f));
	}

	float WUToPx(float worldUnits)
	{
		return worldUnits * m_pixelsPerUnit;
	}
	float PxToWU(float pixels)
	{
		return pixels / m_pixelsPerUnit;
	}

	D2D1_POINT_2F WorldUnit_To_ScreenPoint(Location point)
	{
		Location cornerLocation_wu{
			m_pOrigin->x - PxToWU(m_screenSize_px.width / 2.f),
			m_pOrigin->y + PxToWU(m_screenSize_px.height / 2.f)
		};

		return {
			WUToPx(-cornerLocation_wu.x + point.x),
			WUToPx(cornerLocation_wu.y + -point.y)
		};
	}
	Location ScreenPoint_To_WorldUnit(D2D1_POINT_2F point)
	{
		Location cornerLocation_wu{
			m_pOrigin->x - PxToWU(m_screenSize_px.width / 2.f),
			m_pOrigin->y + PxToWU(m_screenSize_px.height / 2.f)
		};

		return {
			cornerLocation_wu.x + PxToWU(point.x),
			cornerLocation_wu.y + PxToWU(-point.y)
		};
	}

	void Render(Sprite* pSprite)
	{
		if (pSprite->m_pBitmap)
		{
			D2D1_POINT_2F location_px{
				WorldUnit_To_ScreenPoint(
					{
						pSprite->m_pOrigin->x + pSprite->m_offset.x - m_offset.x,
						pSprite->m_pOrigin->y + pSprite->m_offset.y - m_offset.y
					}
				)
			};
			D2D1_POINT_2F size_px{
				WUToPx(pSprite->m_size.width),
				WUToPx(pSprite->m_size.height)
			};

			m_pGraphics->FillBitmap(
				pSprite->m_pBitmap,
				{
					location_px.x - size_px.x / 2.f,
					location_px.y - size_px.y / 2.f,
					location_px.x + size_px.x / 2.f,
					location_px.y + size_px.y / 2.f
				},
				pSprite->m_rotation
			);
		}
	}
	void RenderCollision(Collision_Base* pCollision)
	{
		D2D1_POINT_2F location_px{
			WorldUnit_To_ScreenPoint(
				{
					pCollision->m_pOrigin->x + pCollision->m_offset.x - m_offset.x,
					pCollision->m_pOrigin->y + pCollision->m_offset.y - m_offset.y
				}
			)
		};

		if (dynamic_cast<Collision_Box*>(pCollision))
		{
			Collision_Box* pBox{ dynamic_cast<Collision_Box*>(pCollision) };

			D2D1_POINT_2F size_px{
				WUToPx(pBox->m_size.width),
				WUToPx(pBox->m_size.height)
			};

			m_pGraphics->DrawRect(
				{
					location_px.x - size_px.x / 2.f,
					location_px.y - size_px.y / 2.f,
					location_px.x + size_px.x / 2.f,
					location_px.y + size_px.y / 2.f
				},
				m_pDebugBrush
			);
		}

	}
	void RenderWorldUnits()
	{
		D2D1_POINT_2F worldOrigin{ WorldUnit_To_ScreenPoint({0,0}) };

		m_pGraphics->DrawLine(
			D2D1::Point2F(0, worldOrigin.y),
			D2D1::Point2F(m_screenSize_px.width, worldOrigin.y),
			m_pDebugBrush
		);

		m_pGraphics->DrawLine(
			D2D1::Point2F(worldOrigin.x, 0),
			D2D1::Point2F(worldOrigin.x, m_screenSize_px.height),
			m_pDebugBrush
		);
	}
};
