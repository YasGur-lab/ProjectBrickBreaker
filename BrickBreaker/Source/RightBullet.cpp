#include <iostream>
#include "Bullet.h"
#include "Paddle.h"
#include "RightBullet.h"


//const
const float RightBullet::MAX_VELOCITY = 200.0f;

RightBullet::RightBullet(Paddle* paddle)
	: m_Active(false)
	, m_Position(100, 400)
	, m_Velocity(0, 0)
	, accelerationRate(0.5f)
	, m_Angle()
	, m_Speed(3.5, 3.5)
	, m_Radius(10.0f)
	, m_TimeAlive(0)
{
	m_Paddle = paddle;
}

RightBullet::~RightBullet()
{
}

void RightBullet::OnUpdate(float delta)
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
}

void RightBullet::OnRender(BatchRenderer& batchRenderer)
{
	if (m_Active == false) return;

	batchRenderer.RenderCircle(m_Position, m_Radius, ColorList::White);
}

void RightBullet::OnKeyEvent(KeyCode keyCode, KeyState keyState)
{
}


//Switches

bool RightBullet::IsActive()
{
	return m_Active;
}

void RightBullet::Activate(Vector2 position)
{
	m_Active = true;
	m_Position = position;
}

void RightBullet::Deactivate()
{
	m_Active = false;
}


//Setters

void RightBullet::SetVelocity(Vector2 Velocity)
{
	m_Velocity = Velocity;
}

void RightBullet::SetPosition(Vector2 position)
{
	m_Position = position;
}


//Getters

Vector2 RightBullet::GetBulletPosition()
{
	return m_Position;
}

float RightBullet::GetBulletRadius()
{
	return m_Radius;
}


