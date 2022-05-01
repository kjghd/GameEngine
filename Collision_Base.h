#pragma once

#include "Component_Base.h"

class Collision_Base : public Component_Base
{
public:
	bool m_block;

	Collision_Base(Location* pOrigin, Location offset, bool block)
		:
		Component_Base(pOrigin, offset),
		m_block(block)
	{
	}
};
