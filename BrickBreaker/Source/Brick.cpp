#include "Brick.h"
#include <iostream>
#include "Tweening.h"
#include<windows.h>

using namespace GameDev2D;

Brick::Brick()
	: m_Color4(ColorList::Red)
	, m_Color3(ColorList::Orange)
	, m_Color2(ColorList::Yellow)
	, m_Color1(ColorList::Green)
	, m_IsActive(true)
	, m_Position(255, GetScreenHeight()-100)
	, m_Size(0, 0)
	, m_Lives(4.0f)
	, m_SizeTween(m_Size)
	, timer(0.0f)
{
	m_SizeTween.Start(Vector2(50, 30), 1.0f, Vector2(0,0));
}

Brick::~Brick()
{
}

void Brick::OnUpdate(float delta)
{
	timer += delta;

	m_SizeTween.OnUpdate(delta);


	//Log::Message("Ball Speed: (%0.2f)", timer);
}

void Brick::OnRender(BatchRenderer& batchRenderer)
{
	//Each health level should be represented with a different color
	if (m_Lives == 4)
	{
		batchRenderer.RenderQuad(m_Position, m_Size, m_Color4);
	}
	else if (m_Lives == 3)
	{
		batchRenderer.RenderQuad(m_Position, m_Size, m_Color3);
	}
	else if (m_Lives == 2)
	{
		batchRenderer.RenderQuad(m_Position, m_Size, m_Color2);
	}
	else if (m_Lives == 1)
	{
		batchRenderer.RenderQuad(m_Position, m_Size, m_Color1);
	}
}

void Brick::OnKeyEvent(KeyCode keyCode, KeyState keyState)
{
}


//Switches

void GameDev2D::Brick::Deactivate()
{
	m_IsActive = false;
}

void GameDev2D::Brick::Activate()
{
	m_IsActive = true;
}

bool GameDev2D::Brick::IsActive()
{
	return m_IsActive;
}


//Setters

void GameDev2D::Brick::SetBrickPosition(Vector2 position)
{
	m_Position = position;
}

void GameDev2D::Brick::SetBrickLives(float lives)
{
	m_Lives = lives;
}


//Getters

Vector2 GameDev2D::Brick::GetBrickPosition()
{
	return m_Position;
}

Vector2 GameDev2D::Brick::GetSize()
{
	return m_Size;
}


//Lives

void GameDev2D::Brick::MinusLife()
{
	bool isLastAnimationOver = false;
	if (m_Lives > 0.0f)
	{
		m_SizeTween.Start(Vector2(50, 30), 0.2f, Vector2(m_Size/1.5));
		m_Lives -= 1.0f;
	}
	else if (m_Lives == 0.0f)
	{
		m_Lives = 0.0f;
	}
}

float GameDev2D::Brick::Lives()
{
	return m_Lives;
}