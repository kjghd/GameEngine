#pragma once

#include "Collision_Base.h"

class Collision_Circle : public Collision_Base
{
public:
	float m_radius;

	Collision_Circle(Location* pOrigin, Location offset, bool block, float radius)
		:
		Collision_Base(pOrigin, offset, block),
		m_radius(radius)
	{
	}
};
