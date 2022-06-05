#pragma once

#include "GameObject.h"
#include <vector>


bool Collision_CircleVsCircle(Collision_Circle* pA, Collision_Circle* pB)
{
	Location distance{
		(pB->m_pOrigin->x + pB->m_offset.x) - (pA->m_pOrigin->x + pA->m_offset.x),
		(pB->m_pOrigin->y + pB->m_offset.y) - (pA->m_pOrigin->y + pA->m_offset.y)
	};


	if (distance.x + distance.y <= pA->m_radius + pB->m_radius)
	{


		return true;
	}
	else
		return false;
}
bool Collision_AABB(Collision_Box* pA, Collision_Box* pB)
{
	// B.r - A.l
	float overlapL{
		(pB->m_pOrigin->x + pB->m_offset.x + pB->m_size.width / 2.f) -
		(pA->m_pOrigin->x + pA->m_offset.x - pA->m_size.width / 2.f)
	};

	// A.r - B.l
	float overlapR{
		(pA->m_pOrigin->x + pA->m_offset.x + pA->m_size.width / 2.f) -
		(pB->m_pOrigin->x + pB->m_offset.x - pB->m_size.width / 2.f)
	};

	// B.t - A.b
	float overlapB{
	   (pB->m_pOrigin->y + pB->m_offset.y + pB->m_size.height / 2.f) -
	   (pA->m_pOrigin->y + pA->m_offset.y - pA->m_size.height / 2.f)
	};

	// A.t - B.b
	float overlapT{
		(pA->m_pOrigin->y + pA->m_offset.y + pA->m_size.height / 2.f) -
		(pB->m_pOrigin->y + pB->m_offset.y - pB->m_size.height / 2.f)
	};

	if (overlapL >= 0 && overlapR >= 0 && overlapB >= 0 && overlapT >= 0)
	{
		// block movement
		if (pA->m_block && pB->m_block)
		{
			if (overlapL < overlapR &&
				overlapL < overlapB &&
				overlapL < overlapT)
			{
				pA->m_pOrigin->x += overlapL;
			}
			if (overlapR < overlapL &&
				overlapR < overlapB &&
				overlapR < overlapT)
			{
				pA->m_pOrigin->x -= overlapR;
			}
			if (overlapB < overlapL &&
				overlapB < overlapR &&
				overlapB < overlapT)
			{
				pA->m_pOrigin->y += overlapB;
			}
			if (overlapT < overlapL &&
				overlapT < overlapR &&
				overlapT < overlapB)
			{
				pA->m_pOrigin->y -= overlapT;
			}
		}

		return true;
	}
	else
		return false;
}
/*bool Collision_CircleVsAABB(Collision_Circle* pC, Collision_Box* pR)
{
	float nearestX = std::clamp(
		pC->m_pLocation->x + pC->m_offset.x,
		pR->m_pLocation->x - pR->m_size.x / 2.f,
		pR->m_pLocation->x + pR->m_size.x / 2.f
	);

	float nearestY = std::clamp(
		pC->m_pLocation->y + pC->m_offset.y,
		pR->m_pLocation->y - pR->m_size.y / 2.f,
		pR->m_pLocation->y + pR->m_size.y / 2.f
	);

	XY vector{
		nearestX - (pC->m_pLocation->x + pC->m_offset.x),
		nearestY - (pC->m_pLocation->y + pC->m_offset.y)
	};

	float overlapX = vector.x - pC->m_radius;
	float overlapY = vector.y - pC->m_radius;

	if (overlapX < 0 && overlapY < 0)
	{
		return true;
	}
	else
		return false;
}*/


class Scene
{
public:
	std::vector<GameObject*> m_vpSceneObjects;
	std::vector<GameObject*> m_vpSpawnQueue;

	View* pCurrentView;

	template <class T>
	void Spawn(T* pObject, Location location = { 0,0 })
	{
		m_vpSpawnQueue.push_back(new T(*pObject));
		m_vpSpawnQueue.back()->m_location = location;
	}


	void Update()
	{
		// Spawn
		for (auto& pObject : m_vpSpawnQueue)
			m_vpSceneObjects.push_back(std::move(pObject));
		m_vpSpawnQueue.clear();

		// Collision Check
		for (auto& pObjectA : m_vpSceneObjects)
			if (pObjectA->m_dynamic)
				for (auto& pComponentA : pObjectA->m_vpComponents)
					if (dynamic_cast<Collision_Base*>(pComponentA))

						for (auto& pObjectB : m_vpSceneObjects)
							if (&pObjectB != &pObjectA)
								for (auto& pComponentB : pObjectB->m_vpComponents)
									if (dynamic_cast<Collision_Base*>(pComponentB))
									{
										if (dynamic_cast<Collision_Box*>(pComponentA) && dynamic_cast<Collision_Box*>(pComponentB))
											Collision_AABB(dynamic_cast<Collision_Box*>(pComponentA), dynamic_cast<Collision_Box*>(pComponentB));
									}



		// Object update
		for (auto& pObject : m_vpSceneObjects)
			pObject->Update();
	}

	void Render()
	{
		if (!pCurrentView)
		{
			for (const auto& pObject : m_vpSceneObjects)
				for (const auto& pComponent : pObject->m_vpComponents)
					if (dynamic_cast<View*>(pComponent)) pCurrentView = dynamic_cast<View*>(pComponent);
		}

		pCurrentView->RenderWorldUnits();

		for (const auto& pObject : m_vpSceneObjects)
			for (const auto& pComponent : pObject->m_vpComponents)
				if (dynamic_cast<SpriteSheet*>(pComponent))
				{
					dynamic_cast<SpriteSheet*>(pComponent)->NextFrame();
					pCurrentView->Render(dynamic_cast<SpriteSheet*>(pComponent));
				}
				else if (dynamic_cast<Sprite*>(pComponent)) pCurrentView->Render(dynamic_cast<Sprite*>(pComponent));


		for (const auto& pObject : m_vpSceneObjects)
			for (const auto& pComponent : pObject->m_vpComponents)
				if (dynamic_cast<Collision_Base*>(pComponent)) pCurrentView->RenderCollision(dynamic_cast<Collision_Base*>(pComponent));
	}

};
