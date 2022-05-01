#pragma once

#include "GameObject.h"
#include "Timer.h"

class Character : public GameObject
{
public:
	float m_moveSpeed;
	float m_health;
	bool m_alive;

	Character(const char* id, Location location, float moveSpeed, float health, bool alive = true)
		:
		GameObject(id, location, true),
		m_moveSpeed(moveSpeed),
		m_health(health),
		m_alive(alive)
	{
	}

	void Move(bool left, bool right, bool up, bool down)
	{
		if (left ) m_location.x -= m_moveSpeed * g_timer.deltaTime;
		if (right) m_location.x += m_moveSpeed * g_timer.deltaTime;
		if (up	 ) m_location.y += m_moveSpeed * g_timer.deltaTime;
		if (down ) m_location.y -= m_moveSpeed * g_timer.deltaTime;
	}

	virtual void Update() override
	{
		if (m_health <= 0) m_alive = false;
	}
};
