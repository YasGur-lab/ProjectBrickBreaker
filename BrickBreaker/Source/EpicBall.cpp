#include <iostream>
#include "Paddle.h"
#include "EpicBall.h"


//const
const float EpicBall::MAX_VELOCITY = 200.0f;

EpicBall::EpicBall(Paddle* paddle)
	: m_Active(false)
	, m_Position(100, 400)
	, m_Velocity(0, 0)
	, accelerationRate(0.5f)
	, m_Angle()
	, m_Speed(3.5, 3.5)
	, m_Radius(30.0f)
	, m_TimeAlive(0)
	, m_Color(ColorList::White)
{
	m_Paddle = paddle;
}

EpicBall::~EpicBall()
{
}

void EpicBall::OnUpdate(float delta)
{

	m_TimeAlive += delta;

	if (m_TimeAlive > 2)
	{
		Deactivate();
		m_TimeAlive = 0;
	}

	float m_AngleRadians = 90 / 180.0f * (float)M_PI;
	Vector2 m_Direction = Vector2(cosf(m_AngleRadians), sinf(m_AngleRadians));

	m_Velocity += m_Direction * m_Paddle->GetPaddleSpeed().x * delta;
	Vector2 displacement = m_Velocity * m_Speed * delta;
	m_Position += displacement;

	//max speed
	float speed = m_Velocity.Length();
	if (m_Velocity.Length() > MAX_VELOCITY)
	{
		m_Velocity = m_Velocity.Normalized() * MAX_VELOCITY;
	}

	if (m_TimeAlive > 0 && m_TimeAlive < 0.1)
	{
		m_Color = ColorList::Coral;
	}
	else if (m_TimeAlive > 0.1 && m_TimeAlive < 0.2)
	{
		m_Color = ColorList::Blue;
	}
	else if (m_TimeAlive > 0.2 && m_TimeAlive < 0.3)
	{
		m_Color = ColorList::Green;
	}
	else if (m_TimeAlive > 0.3 && m_TimeAlive < 0.4)
	{
		m_Color = ColorList::Yellow;
	}
	else if (m_TimeAlive > 0.4 && m_TimeAlive < 0.5)
	{
		m_Color = ColorList::Pink;
	}
	else if (m_TimeAlive > 0.5 && m_TimeAlive < 0.6)
	{
		m_Color = ColorList::Orange;
	}
	else if (m_TimeAlive > 0.6 && m_TimeAlive < 0.7)
	{
		m_Color = ColorList::Red;
	}
	if (m_TimeAlive > 0.7 && m_TimeAlive < 0.8)
	{
		m_Color = ColorList::Coral;
	}
	else if (m_TimeAlive > 0.8 && m_TimeAlive < 0.9)
	{
		m_Color = ColorList::Blue;
	}
	else if (m_TimeAlive > 0.9 && m_TimeAlive < 1.0)
	{
		m_Color = ColorList::Green;
	}

}

void EpicBall::OnRender(BatchRenderer& batchRenderer)
{
	if (m_Active == false) return;

	batchRenderer.RenderCircle(m_Position.x, m_Position.y, m_Radius, m_Color);
}

void EpicBall::OnKeyEvent(KeyCode keyCode, KeyState keyState)
{
}


//Switches

bool EpicBall::IsActive()
{
	return m_Active;
}

void EpicBall::Activate(Vector2 position)
{
	m_Active = true;
	m_Position = position;
}

void EpicBall::Deactivate()
{
	m_Active = false;
}


//Setters

void EpicBall::SetVelocity(Vector2 Velocity)
{
	m_Velocity = Velocity;
}

void EpicBall::SetPosition(Vector2 position)
{
	m_Position = position;
}


//Getters

Vector2 EpicBall::GetEpicBallPosition()
{
	return m_Position;
}

float EpicBall::GetEpicBallRadius()
{
	return m_Radius;
}


