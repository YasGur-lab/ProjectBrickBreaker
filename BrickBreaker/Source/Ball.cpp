#include "Ball.h"

using namespace GameDev2D;

//const
const int Ball::NUM_EDGES = 4;
const int Ball::NUM_PASSES = 5;
const float Ball::SPEED = 450.0f;

GameDev2D::Ball::Ball()
	: m_Color(ColorList::Green)
	, m_Radius(7)
	, m_Position(0, 0)
	, m_IsActive(true)
	, m_IsPaused(false)
	, m_Angle(0)
	, m_Velocity(0, 0)
	, m_Speed(200.0f)
	, m_Outline(1.0f)
	, m_ScreenEdges{}
	, m_Lives(3)
	, m_TextLives("OpenSans-CondBold_72")
{

	Vector2 m_Screen(GetScreenWidth(), GetScreenHeight());
	m_ScreenEdges[0] = LineSegment(Vector2(0.0f, 0.0f), Vector2(0.0f, m_Screen.y)); // left
	m_ScreenEdges[1] = LineSegment(Vector2(m_Screen.x, 0.0f), Vector2(m_Screen.x, m_Screen.y)); // right
	m_ScreenEdges[2] = LineSegment(Vector2(0.0f, m_Screen.y), Vector2(m_Screen.x, m_Screen.y)); // top

	Reset();

	m_TextLives.SetPosition(10, GetScreenHeight() - GetScreenHeight() + 45);
	m_TextLives.SetScale(0.5, 0.5);
}

GameDev2D::Ball::~Ball()
{
}

void GameDev2D::Ball::OnUpdate(float delta)
{
	//Increase the speed of the ball every frame
	m_Speed = m_Speed + (m_Speed * 0.01 * delta);

	if (m_Speed >= SPEED)
	{
		m_Speed = SPEED;
	}

	//m_Velocity = m_Velocity + (m_Velocity * 0.001);

	std::string LivessinText = "Health (" + std::to_string(m_Lives) + "/5)";
	m_TextLives.SetText(LivessinText);
	IsBallOfEdge();
	
	if (m_IsPaused)
		return;

	Vector2 m_Screen(GetScreenWidth(), GetScreenHeight());

	Vector2 position = m_Position;
	Vector2 velocity = m_Velocity;

	Vector2 normals[NUM_EDGES] = { Vector2::Right, Vector2::Left, Vector2::Up };

	Vector2 displacementRemaining = m_Velocity * delta;

	bool collision = false;

	int numPasses = 0;
	LineSegment* ignoreEdge = nullptr;
	AABB* ignoreAABB = nullptr;

	do
	{
		numPasses++;
		collision = false;

		Vector2 start = position;
		Vector2 end = start + displacementRemaining;

		for (int i = 0; i < NUM_EDGES; i++)
		{
			//Ensure you dont check collision with the same edge twice
			if (&m_ScreenEdges[i] != ignoreEdge)
			{
				LineSegment path(start, end);
				Vector2 intersectionPoint;
				if (path.DoesIntersect(m_ScreenEdges[i], intersectionPoint) == true)
				{
					collision = true;
					//	//r = d-2(d*n)n
					velocity = velocity - 2.0f * (velocity.DotProduct(normals[i])) * normals[i];

					float distanceRemaining = (end - intersectionPoint).Length();
					displacementRemaining = velocity.Normalized() * distanceRemaining;

					position = intersectionPoint;

					//Ignore the edge we just collided with to ensure we dont collide with it again on the next pass
					ignoreEdge = &m_ScreenEdges[i];

					break;
				}
			}
		}

		if (collision == false)
		{
			position = start + displacementRemaining;
		}
	} 
	while (collision && numPasses < NUM_PASSES);

	m_Position = position;
	m_Velocity = velocity;

	////Increase the speed of the ball every frame
	//m_Velocity = m_Velocity + (m_Velocity * 0.001);

	GetBallVelocity();
	GetBallPosition();

	Log::Message("Ball Speed: (%0.2f, %0.2f)", m_Velocity.x, m_Velocity.y);
}

void GameDev2D::Ball::OnRender(BatchRenderer& batchRenderer)
{
	batchRenderer.RenderSpriteFont(m_TextLives);

	Color ballOutline = ColorList::Azure;
	Color ballFill = m_Color;
	ballFill.a = 0.6f;

	if (m_IsActive == true)
	{
		batchRenderer.RenderCircle(m_Position, m_Radius - m_Outline, ballFill, ballOutline, m_Outline);
	}
}

void GameDev2D::Ball::OnKeyEvent(KeyCode keyCode, KeyState keyState)
{
	if (keyCode == KeyCode::R && keyState == KeyState::Down)
	{
		Reset();
	}
}

void GameDev2D::Ball::OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY)
{
	if (button == MouseButton::Left && state == MouseButtonState::Pressed)
	{
		m_IsPaused = !m_IsPaused;
	}
}


//Game mechanics

bool Ball::IsBallOfEdge()
{//Spawn the ball at the center of the paddle in a random upwards direction when the user presses the left click
	if (m_Position.y <= 0)
	{
		MinusLife();
		Reset();
		return true;
	}
	else
	{
		return false;
	}
}

void GameDev2D::Ball::BounceBackUpper()
{
	float m_RandomAngle = Math::RandomFloat(45.0f, 135.0f);
	float m_AngleRadians = Math::DegreesToRadians(m_RandomAngle);
	m_Velocity = Vector2(cos(m_AngleRadians), sin(m_AngleRadians)) * m_Speed;
}

void GameDev2D::Ball::BounceBackLower()
{
	float m_RandomAngle = Math::RandomFloat(225.0f, 315.0f);
	float m_AngleRadians = Math::DegreesToRadians(m_RandomAngle);
	m_Velocity = Vector2(cos(m_AngleRadians), sin(m_AngleRadians)) * m_Speed;
}

void GameDev2D::Ball::BounceBackRight()
{
	float m_RandomAngle = Math::RandomFloat(45.0f, 135.0f) || Math::RandomFloat(225.0f, 315.0f);
	float m_AngleRadians = Math::DegreesToRadians(m_RandomAngle);
	m_Velocity = Vector2(cos(m_AngleRadians), sin(m_AngleRadians)) * m_Speed;
}

void GameDev2D::Ball::BounceBackLeft()
{
	float m_RandomAngle = Math::RandomFloat(120.0f, 240.0f);
	float m_AngleRadians = Math::DegreesToRadians(m_RandomAngle);
	m_Velocity = Vector2(cos(m_AngleRadians), sin(m_AngleRadians)) * m_Speed;
}

void GameDev2D::Ball::Reset()
{
	float m_RandomAngle = Math::RandomFloat(45.0f, 135.0f);
	float m_AngleRadians = Math::DegreesToRadians(m_RandomAngle);
	m_Velocity = Vector2(cos(m_AngleRadians), sin(m_AngleRadians)) * m_Speed;

	m_IsPaused = true;
}

int GameDev2D::Ball::GetNumPasses()
{
	return NUM_PASSES;
}


//Switches

void GameDev2D::Ball::Deactivate()
{
	m_IsActive = false;
}

void GameDev2D::Ball::Activate()
{
	m_IsActive = true;
}

bool GameDev2D::Ball::IsActive()
{
	return m_IsActive;
}

bool GameDev2D::Ball::IsGamePaused()
{
	return m_IsPaused;
}


//Setters

void GameDev2D::Ball::SetBallPosition(Vector2 position)
{
	m_Position = position;
}

void GameDev2D::Ball::SetBallVelocity(Vector2 velocity)
{
	m_Velocity = velocity;
}

void GameDev2D::Ball::SetBallAngle(float angle)
{
	m_Angle = angle;
}

void GameDev2D::Ball::SetBallSpeed(float speed)
{
	m_Speed = speed;
}

void GameDev2D::Ball::SetBallPaused(bool paused)
{
	m_IsPaused = paused;
}

void GameDev2D::Ball::SetBallRadius(float radius)
{
	m_Radius = radius;
}

void GameDev2D::Ball::SetBallHealth(int lives)
{
	m_Lives = lives;
}


//Getters

float GameDev2D::Ball::GetBallRadius()
{
	return m_Radius;
}

Vector2 GameDev2D::Ball::GetBallPosition()
{
	return m_Position;
}

float GameDev2D::Ball::GetBallSpeed()
{
	return m_Speed;
}

float GameDev2D::Ball::GetBallAngle()
{
	return m_Angle;
}

Vector2 GameDev2D::Ball::GetBallVelocity()
{
	return m_Velocity;
}


//Powerups

void GameDev2D::Ball::ResetSize()
{
	m_Radius = 7;
}

void Ball::MinusLife()
{
	if (m_Lives > 0)
	{
		m_Lives -= 1;
	}
	else if (m_Lives == 0)
	{
		m_Lives = 0;
	}
}

void Ball::PlusLife()
{
	if (m_Lives < 5)
	{
		m_Lives += 1;
	}
	else if (m_Lives >= 5)
	{
		m_Lives = 5;
	}
}

int Ball::IsDead()
{
	return m_Lives;
}

void GameDev2D::Ball::GrowBallSize()
{
	m_Radius = m_Radius + 2;
}

void GameDev2D::Ball::ReducedBallSize()
{
	m_Radius = m_Radius - 1;
}