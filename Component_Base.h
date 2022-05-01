#pragma once
#include "GameStructures.h"

class Component_Base
{
public:
	Location* m_pOrigin;
	Location m_offset;

	Component_Base(Location* pOrigin, Location offset)
		:
		m_pOrigin(pOrigin),
		m_offset(offset)
	{
	}

	virtual void Function()
	{
		// Does nothing.
	}
};