#include "Paddle.h"
#include <iostream>

using namespace GameDev2D;

//const
const float Paddle::MAX_VELOCITY = 150.0f;

GameDev2D::Paddle::Paddle()
	: m_Position(GetHalfScreenWidth()-50, GetScreenHeight()*0.1)
	, m_Size(100, 20)
	, m_Color(ColorList::Orange)
	, m_MovingLeft(false)
	, m_MovingRight(false)
	, m_IsActive(true)
	, m_Velocity(0, 0)
{
}

GameDev2D::Paddle::~Paddle()
{
}

void GameDev2D::Paddle::OnUpdate(float delta)
{
	const float DEFAULT_DRAG = 0.2f;
	const float MAX_SPEED = 800.0f;
	const float DEFAULT_ACCELERATION = 20.0f;

	float drag = -DEFAULT_DRAG;
	float acceleration = m_Position.x * DEFAULT_ACCELERATION;

	if (m_MovingLeft == true)
	{
		if (acceleration != 0.0f)
		{
			drag = (acceleration - (acceleration * DEFAULT_DRAG)) / acceleration;
		}

		m_Velocity.x += acceleration + (m_Velocity.x * drag) * powf((float)delta, 0.5f);

		//clamp paddle speed
		m_Velocity.x = Math::Clamp(m_Velocity.x, -MAX_SPEED, MAX_SPEED);

		m_Position.x -= m_Velocity.x * delta;
	}

	if (m_MovingRight == true)
	{
		if (acceleration != 0.0f)
		{
			drag = (acceleration - (acceleration * DEFAULT_DRAG)) / acceleration;
		}

		m_Velocity.x += acceleration + (m_Velocity.x * drag) * powf((float)delta, 0.5f);

		//clamp paddle speed
		m_Velocity.x = Math::Clamp(m_Velocity.x, -MAX_SPEED, MAX_SPEED);

		m_Position.x += m_Velocity.x * delta;
	}

	//screen limits
	if (m_Position.x <= 0)
	{
		m_Position.x = 0;
	}

	if (m_Position.x + m_Size.x >= GetScreenWidth())
	{
		m_Position.x = GetScreenWidth() - m_Size.x;
	}
}

void GameDev2D::Paddle::OnRender(BatchRenderer& batchRenderer)
{
	batchRenderer.RenderQuad(m_Position, m_Size, m_Color);
}

void GameDev2D::Paddle::OnKeyEvent(KeyCode keyCode, KeyState keyState)
{
	if (keyCode == KeyCode::A && keyState == KeyState::Down || keyCode == KeyCode::Left && keyState == KeyState::Down)
	{
		m_MovingLeft = true;
	}

	if (keyCode == KeyCode::A && keyState == KeyState::Up || keyCode == KeyCode::Left && keyState == KeyState::Up)
	{
		m_MovingLeft = false;
	}

	if (keyCode == KeyCode::D && keyState == KeyState::Down || keyCode == KeyCode::Right && keyState == KeyState::Down)
	{
		m_MovingRight = true;
	}

	if (keyCode == KeyCode::D && keyState == KeyState::Up || keyCode == KeyCode::Right && keyState == KeyState::Up)
	{
		m_MovingRight = false;
	}
}


//Getters

Vector2 GameDev2D::Paddle::GetPaddlePosition()
{
	return m_Position;
}

Vector2 GameDev2D::Paddle::GetPaddleSize()
{
	return m_Size;
}

Vector2 GameDev2D::Paddle::GetPaddleSpeed()
{
	return m_Position;
}


//Switch

bool GameDev2D::Paddle::IsActive()
{
	return m_IsActive;
}


//PowerUps

void GameDev2D::Paddle::GrowPaddleSize()
{
	m_Size.x = m_Size.x + 10;
}

void GameDev2D::Paddle::ReducedPaddleSize()
{
	m_Size.x = m_Size.x - 10;
}

void GameDev2D::Paddle::ResetSize()
{
	m_Size.x = 100;
}

