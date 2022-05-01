#pragma once

#include "Collision_Base.h"

class Collision_Box : public Collision_Base
{
public:
	Size m_size;

	Collision_Box(Location* pOrigin, Location offset, bool block, Size size)
		:
		Collision_Base(pOrigin, offset, block),
		m_size(size)
	{
	}
};
