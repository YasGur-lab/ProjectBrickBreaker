#include "Bullet.h"
#include <iostream>
#include "Paddle.h"

//const
const float Bullet::MAX_VELOCITY = 200.0f;

Bullet::Bullet(Paddle* paddle)
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

Bullet::~Bullet()
{
}

void Bullet::OnUpdate(float delta)
{

	m_TimeAlive += delta;

	if (m_TimeAlive > 3)
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

void Bullet::OnRender(BatchRenderer& batchRenderer)
{
    if (m_Active == false) return;

    batchRenderer.RenderCircle(m_Position, m_Radius, ColorList::White);
}

void Bullet::OnKeyEvent(KeyCode keyCode, KeyState keyState)
{
}


//Switches

bool Bullet::IsActive()
{
    return m_Active;
}

void Bullet::Activate(Vector2 position)
{
	m_Active = true;
	m_Position = position;
}

void Bullet::Deactivate()
{
    m_Active = false;
}


//Setters

void Bullet::SetVelocity(Vector2 Velocity)
{
	m_Velocity = Velocity;
}

void Bullet::SetPosition(Vector2 position)
{
	m_Position = position;
}


//Getters

Vector2 Bullet::GetBulletPosition()
{
	return m_Position;
}

float Bullet::GetBulletRadius()
{
	return m_Radius;
}


