#pragma once

#include "Character.h"
#include "Input.h"
#include "Timer.h"

class Player : public Character
{
public:
	Input* m_pInput;

	Player(const char* id, Location location, float moveSpeed, float health, Input* pInput)
		:
		Character(id, location, moveSpeed, health),
		m_pInput(pInput)
	{
	}

	virtual void Update() override
	{
		if (m_health <= 0) m_alive = false;

		Move(
			m_pInput->CheckHeld(BID_A),
			m_pInput->CheckHeld(BID_D),
			m_pInput->CheckHeld(BID_W),
			m_pInput->CheckHeld(BID_S)
		);
	}
};
