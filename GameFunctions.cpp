#include "GameFunctions.h"
#include "Input.h"
#include "Graphics.h"
#include "Audio.h"
#include "Timer.h"
#include <vector>
#include <memory>

Graphics graphics;
Audio audio;
Timer timer;
Input input;

// Game Functions
struct Location
{
	float x, y;
};
struct Size
{
	float width, height;
};

class Component
{
public:
	Location* m_pOrigin;
	Location m_offset;

	Component(Location* pOrigin, Location offset)
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

class Sprite : public Component
{
public:
	ID2D1Bitmap* m_pBitmap;
	
	Size m_size;
	float m_rotation;

	Sprite(Location* pOrigin, Location offset, Size size, float rotation, Graphics* pGraphics, LPCWSTR lpFileName)
		:
		Component(pOrigin, offset),
		m_pBitmap(NULL),
		m_size(size),
		m_rotation(rotation)
	{
		pGraphics->LoadBitmapFromFile(lpFileName, &m_pBitmap);
	}
};



class Collision_Base : public Component
{
public:
	bool m_block;

	Collision_Base(Location* pOrigin, Location offset, bool block)
		:
		Component(pOrigin, offset),
		m_block(block)
	{
	}
};
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

class View : public Component
{
	float m_pixelsPerUnit;
	Size m_screenSize_px;
	Graphics* m_pGraphics;
	ID2D1SolidColorBrush* m_pDebugBrush;

public:
	View(Location* pOrigin, Location offset, float pixelsPerUnit, Size screenSize_px, Graphics* pGraphics)
		:
		Component(pOrigin, offset),
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

		graphics.DrawLine(
			D2D1::Point2F(0,					 worldOrigin.y),
			D2D1::Point2F(m_screenSize_px.width, worldOrigin.y),
			m_pDebugBrush
		);

		graphics.DrawLine(
			D2D1::Point2F(worldOrigin.x,					  0),
			D2D1::Point2F(worldOrigin.x, m_screenSize_px.height),
			m_pDebugBrush
		);
	}
};


class GameObject
{
public:
	const char* m_id;
	Location m_location;
	std::vector<Component*> m_vpComponents;
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
			if (dynamic_cast<Sprite*>(pComponent)) AddComponent(*dynamic_cast<Sprite*>(pComponent));
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

class Player : public GameObject
{
public:
	Input* m_pInput;
	float m_moveSpeed;

	Player(const char* id, Location location, bool dynamic, Input* pInput, float moveSpeed)
		:
		GameObject(id, location, dynamic),
		m_pInput(pInput),
		m_moveSpeed(moveSpeed)
	{
	}

	virtual void Update() override
	{
		if (m_pInput->CheckHeld(BID_W)) m_location.y += m_moveSpeed * timer.deltaTime;
		if (m_pInput->CheckHeld(BID_S)) m_location.y -= m_moveSpeed * timer.deltaTime;
		if (m_pInput->CheckHeld(BID_D)) m_location.x += m_moveSpeed * timer.deltaTime;
		if (m_pInput->CheckHeld(BID_A)) m_location.x -= m_moveSpeed * timer.deltaTime;
	}
};

Player* player;
GameObject* block;

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
		if (input.CheckPressed(BID_LMB))
		{
			Spawn(block, pCurrentView->ScreenPoint_To_WorldUnit(input.mouse));
		}
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
				if (dynamic_cast<Sprite*>(pComponent)) pCurrentView->Render(dynamic_cast<Sprite*>(pComponent));

		for (const auto& pObject : m_vpSceneObjects)
			for (const auto& pComponent : pObject->m_vpComponents)
				if (dynamic_cast<Collision_Base*>(pComponent)) pCurrentView->RenderCollision(dynamic_cast<Collision_Base*>(pComponent));
	}

};

Scene scene;

// Preset Objects.


void Game_Setup(HWND hWnd)
{
	graphics.Setup(hWnd);
	audio.Setup();
	timer.Start();

	player = new Player("player", { 0,0 }, true, &input, 0.01f);
	player->AddComponent(View(&player->m_location, { 0,0 }, 64, { 640,480 }, &graphics));
	player->AddComponent(Sprite(&player->m_location, { 0,0 }, { 1,1 }, 0, &graphics, L"Assets/Images/human.png"));
	player->AddComponent(Collision_Box(&player->m_location, { 0,0 }, true, { 1,1 }));

	block = new GameObject("block", { 0,0 }, false);
	block->AddComponent(Sprite(&block->m_location, { 0,0 }, { 1,1 }, 0, &graphics, L"Assets/Images/block.png"));
	block->AddComponent(Collision_Box(&block->m_location, { 0,0 }, true, { 1,1 }));


	scene.Spawn(player);
	//scene.Spawn(block, { 2,1 });
	//scene.Spawn(block, { -1,-1 });
}

void Game_UpdateInput(LPMSG lpMsg)
{
	input.SetUnchanged();

	switch (lpMsg->message)
	{
	case WM_MOUSEMOVE:	 input.UpdateMousePosition(lpMsg->lParam); break;
	case WM_LBUTTONDOWN: input.SetMouseDown();					   break;
	case WM_LBUTTONUP:	 input.SetMouseUp();					   break;
	case WM_KEYDOWN:	 input.SetButtonsOn(lpMsg->wParam);		   break;
	case WM_KEYUP:		 input.SetButtonsOff(lpMsg->wParam);	   break;
	case WM_MOUSEWHEEL:	 input.scroll = GET_WHEEL_DELTA_WPARAM(lpMsg->wParam); break;
	}
}

void Game_UpdateLogic()
{
	scene.Update();
}

void Game_RenderGraphics()
{
	graphics.BeginDraw();
	graphics.ClearScreen(0.09f, 0.09f, 0.32f);

	scene.Render();

	graphics.EndDraw();
}

void Game_PlayAudio()
{

}

void Game_ResetTimer()
{
	timer.Reset();
}
