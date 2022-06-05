#pragma once

#include "GameStructures.h"
#include "Collision_Base.h"
#include "Collision_Box.h"
#include "Collision_Circle.h"
#include "Sprite.h"
#include "View.h"

class GameObject
{
public:
	const char* m_id;
	Location m_location;
	std::vector<Component_Base*> m_vpComponents;
	bool m_dynamic;

	GameObject(const char* id, Location location, bool dynamic)
		:
		m_id(id),
		m_location(location),
		m_dynamic(dynamic)
	{
	}

	GameObject(const GameObject& gameObject)
		:
		m_id(gameObject.m_id),
		m_location(gameObject.m_location),
		m_dynamic(gameObject.m_dynamic)
	{
		for (auto& pComponent : gameObject.m_vpComponents)
		{
			if (dynamic_cast<SpriteSheet*>(pComponent)) AddComponent(*dynamic_cast<SpriteSheet*>(pComponent));
			else if (dynamic_cast<Sprite*>(pComponent)) AddComponent(*dynamic_cast<Sprite*>(pComponent));
			else if (dynamic_cast<Collision_Box*>(pComponent)) AddComponent(*dynamic_cast<Collision_Box*>(pComponent));
			else if (dynamic_cast<Collision_Circle*>(pComponent)) AddComponent(*dynamic_cast<Collision_Circle*>(pComponent));
			else if (dynamic_cast<View*>(pComponent)) AddComponent(*dynamic_cast<View*>(pComponent));

			m_vpComponents.back()->m_pOrigin = &m_location;
		}
	}

	template <class T>
	void AddComponent(T component)
	{
		m_vpComponents.push_back(new T(component));
	}
	void UpdateComponents()
	{
		for (auto& component : m_vpComponents)
			component->m_pOrigin = &m_location;


	}

	virtual void Update()
	{
		//OutputDebugString(L"Update\n");
	}

	virtual void OnCollision(GameObject* pGameObject)
	{
		//OutputDebugString(L"On Collision\n");
	}
};
