#include "Game.h"
#include "Paddle.h"
#include <iostream>
#include "Brick.h"
#include <vector>
#include <algorithm>
#include "Ball.h"
#include "Bullet.h"
#include "RightBullet.h"
#include "EpicBall.h"
#include <chrono>
#include <thread>

using namespace GameDev2D;
using namespace std::this_thread;
using namespace std::chrono;

Game::Game()
//game basics
	: m_Points(0)
	, m_TextObjective("OpenSans-CondBold_72")
	, m_TextGameOver("OpenSans-CondBold_72")
	, m_TextGameWon("OpenSans-CondBold_72")
	, m_BlackScreenSize(GetScreenWidth(), GetScreenHeight())
	, m_BlackScreenPosition(0, 0)
	, m_TextEscape("OpenSans-CondBold_72")
	, m_TimeAlive(0)
	, m_TimeAliveEpicBall(0)
	, m_TimeAliveAutomaticWeapon(0)
	, m_TimeBrickSpawning(0)
	//paddle grow power
	, m_TextPaddleGrow("OpenSans-CondBold_72")
	, m_DropPaddleGrow{}
	, isPaddleGrowReadyToDrop(false)
	, m_PaddleGrowDropRadius(10.0f)
	, m_PaddleGrowDropColor(ColorList::Green)
	//paddle reduced power 
	, m_TextPaddleReduced("OpenSans-CondBold_72")
	, m_DropPaddleReduced{}
	, m_PaddleReducedDropColor(ColorList::Coral)
	, isPaddleReducedReadyToDrop(false)
	//ball grow power
	, m_TextBallBigger("OpenSans-CondBold_72")
	, m_DropBallGrow{}
	, isBallGrowReadyToDrop(false)
	, m_BallGrowDropRadius(10.0f)
	, m_BallGrowDropColor(ColorList::Blue)
	//ball Health power
	, m_TextBallHealth("OpenSans-CondBold_72")
	, m_DropBallHealth{}
	, m_BallHealthDropColor(ColorList::FireBrick)
	, isBallHealthReadyToDrop(false)
	//Number of balls power
	, m_NumOfBall(1)
	, m_BallAdditionDropColor(ColorList::HotPink)
	, isBallAdditionReadyToDrop(false)
	, m_Nextball(0)
	//WeaponActivation
	, m_TextWeaponActivated("OpenSans-CondBold_72")
	, m_TextWeaponActivatedScreen("OpenSans-CondBold_72")
	, m_Bullets{}
	, isWeaponActiveReadyToDrop(false)
	, m_WeaponActiveDropColor(ColorList::White)
	, m_DropWeaponActivation{}
	, m_ActivateWeapons(false)
	//SuperWeaponActivation
	, m_TextSuperWeaponActivated("OpenSans-CondBold_72")
	, m_TextSuperWeaponActivatedScreen("OpenSans-CondBold_72")
	, m_EpicBall{}
	, isSuperWeaponActiveReadyToDrop(false)
	, m_SuperWeaponActiveDropColor(ColorList::PowderBlue)
	, m_ActivateSuperWeapons(false)
	//RightWeapon
	, m_TextRightWeaponActivated("OpenSans-CondBold_72")
	, m_TextRightWeaponActivatedScreen("OpenSans-CondBold_72")
	, m_RightBullets{}
	, isRightWeaponActiveReadyToDrop(false)
	, m_RightWeaponActiveDropColor(ColorList::PowderBlue)
	, m_ActivateRightWeapons(false)
{
	m_Paddle = new Paddle();

	for (int i = 0; i < 70; i++)
	{
		Brick* brick = new Brick();
		m_Brick.push_back(brick);
	}

	for (int i = 0; i < m_NumOfBall; i++)
	{
		Ball* ball = new Ball();
		m_Ball.push_back(ball);
	}

	for (int v = 0; v < m_Ball.size(); v++)
	{
		if (v >= 1)
		{
			m_Ball[v]->Deactivate();
		}
	}

	for (int i = 0; i < 200; i++)
	{
		Bullet* pBullet = new Bullet(m_Paddle);
		m_Bullets.push_back(pBullet);
	}

	for (int i = 0; i < 200; i++)
	{
		RightBullet* rBullet = new RightBullet(m_Paddle);
		m_RightBullets.push_back(rBullet);
	}

	for (int i = 0; i < 1; i++)
	{
		EpicBall* epicball = new EpicBall(m_Paddle);
		m_EpicBall.push_back(epicball);
	}

	BrickSpawn();
	Text();
}

Game::~Game()
{
	delete m_Paddle;

	for (unsigned int i = 0; i < m_Brick.size(); i++)
	{
		delete m_Brick[i];
	}

	for (unsigned int b = 0; b < m_Ball.size(); b++)
	{
		delete m_Ball[b];
	}

	for (unsigned int i = 0; i < m_Bullets.size(); i++)
	{
		delete m_Bullets[i];
	}

	for (unsigned int i = 0; i < m_RightBullets.size(); i++)
	{
		delete m_RightBullets[i];
	}

	for (unsigned int i = 0; i < m_EpicBall.size(); i++)
	{
		delete m_EpicBall[i];
	}
}

void Game::OnUpdate(float delta)
{
	m_Paddle->OnUpdate(delta);

	CheckForCollision(delta);

	OnUpdateCode(delta);

	//When a ball goes off the bottom of the screen
	if (m_Ball[0]->IsBallOfEdge() == true)
	{
		m_ActivateWeapons = false;
		m_Ball[0]->ResetSize();
		m_Paddle->ResetSize();
	}

	std::string PointsinText = "Destroy 70 bricks (" + std::to_string(m_Points) + "/70)";
	m_TextObjective.SetText(PointsinText);
}

void Game::OnRender(BatchRenderer& batchRenderer)
{
	batchRenderer.BeginScene();

	//Game basics
	m_Paddle->OnRender(batchRenderer);

		for (unsigned int i = 0; i < m_Brick.size(); i++)
		{
			m_Brick[i]->OnRender(batchRenderer);
		}


	for (unsigned int e = 0; e < m_Ball.size(); e++)
	{
		m_Ball[e]->OnRender(batchRenderer);
	}

	for (size_t i = 0; i < m_Bullets.size(); i++)
	{
		if (m_Bullets[i]->IsActive())
		{
			m_Bullets[i]->OnRender(batchRenderer);
		}
	}

	for (size_t i = 0; i < m_RightBullets.size(); i++)
	{
		if (m_RightBullets[i]->IsActive())
		{
			m_RightBullets[i]->OnRender(batchRenderer);
		}
	}


	for (size_t i = 0; i < m_EpicBall.size(); i++)
	{
		if (m_EpicBall[i]->IsActive())
		{
			m_EpicBall[i]->OnRender(batchRenderer);
		}

	}


	//Objective
	for (unsigned int e = 0; e < m_Ball.size(); e++)
	{
		if (m_Ball[0]->IsDead() != 0)
		{
			batchRenderer.RenderSpriteFont(m_TextObjective);
		}
	}

	//Render PowerUps
	PowerUpRender(batchRenderer);

	//Display text on screen indicating to the user it is game over\
	//Do not spawn the ball again
	//Game Lost
	for (unsigned int e = 0; e < m_Ball.size(); e++)
	{
		if (m_Ball[0]->IsDead() == 0)
		{
			m_Ball[e]->Deactivate();
			batchRenderer.RenderQuad(m_BlackScreenPosition, m_BlackScreenSize, ColorList::Black);
			batchRenderer.RenderSpriteFont(m_TextGameOver);
			batchRenderer.RenderSpriteFont(m_TextEscape);
		}
	}

	//When the user has destroyed all the bricks without losing all their lives, display a congratulatory message indicating they have won
	//Game Won
	for (unsigned int e = 0; e < m_Ball.size(); e++)
	{
		if (m_Points == 70)
		{
			m_Ball[e]->Deactivate();
			batchRenderer.RenderQuad(m_BlackScreenPosition, m_BlackScreenSize, ColorList::Black);
			batchRenderer.RenderSpriteFont(m_TextGameWon);
			batchRenderer.RenderSpriteFont(m_TextEscape);
		}
	}

	batchRenderer.EndScene();
}


//On Key and Mouse Event

void Game::OnKeyEvent(KeyCode keyCode, KeyState keyState)
{
	if( keyCode == KeyCode::Escape && keyState == KeyState::Down )
	{
		Application::Get().Quit();
	}
	m_Paddle->OnKeyEvent(keyCode, keyState);

	m_Ball[0]->OnKeyEvent(keyCode, keyState);

	Vector2 BrickCenter(m_Paddle->GetPaddlePosition().x + m_Paddle->GetPaddleSize().x/2, m_Paddle->GetPaddlePosition().y + 55);

	//implement debug keys, using the number keys to spawn your 5 powerups
	//On command power ups

	if (keyCode == KeyCode::One && keyState == KeyState::Down)
	{
		DropWeaponActivation(BrickCenter);
		m_DropWeaponActivation.push_back(BrickCenter);
	}
	if (keyCode == KeyCode::Two && keyState == KeyState::Down)
	{
		DropPaddleGrow(BrickCenter);
		m_DropPaddleGrow.push_back(BrickCenter);
	}
	if (keyCode == KeyCode::Three && keyState == KeyState::Down)
	{
		DropBallGrow(BrickCenter);
		m_DropBallGrow.push_back(BrickCenter);
	}
	if (keyCode == KeyCode::Four && keyState == KeyState::Down)
	{
		DropEpicBall(BrickCenter);
		m_DropEpicBall.push_back(BrickCenter);
	}
	if (keyCode == KeyCode::Five && keyState == KeyState::Down)
	{
		DropBallHealth(BrickCenter);
		m_DropBallHealth.push_back(BrickCenter);
	}
	if (keyCode == KeyCode::R && keyState == KeyState::Down)
	{
		BrickSpawn();
		m_Points = 0;
		m_Ball[0]->Reset();
		m_ActivateWeapons = false;
		m_Ball[0]->ResetSize();
		m_Paddle->ResetSize();
		m_Ball[0]->SetBallHealth(3);
	}
}

void Game::OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY)
{		
	for (unsigned int e = 0; e < m_Ball.size(); e++)
	{
		m_Ball[e]->OnMouseButtonEvent(button, state, mouseX, mouseY);
	}
}

void Game::OnMouseMovedEvent(float mouseX, float mouseY)
{
}


//Update code

void Game::OnUpdateCode(float delta)
{
	//Left bullets
	for (unsigned int i = 0; i < m_Bullets.size(); i++)
	{
		if (m_Bullets[i]->IsActive())
		{
			m_Bullets[i]->OnUpdate(delta);
		}
	}

	//Right bullets
	for (unsigned int i = 0; i < m_RightBullets.size(); i++)
	{
		if (m_RightBullets[i]->IsActive())
		{
			m_RightBullets[i]->OnUpdate(delta);
		}
	}

	//Epic ball
	for (unsigned int i = 0; i < m_EpicBall.size(); i++)
	{
		if (m_EpicBall[i]->IsActive())
		{
			m_EpicBall[i]->OnUpdate(delta);
		}
	}

	//EpicBall activation
	if (m_ActivateSuperWeapons == true)
	{
		float speed = 120.0f;
		float m_AngleRadians = 90 / 180.0f * (float)M_PI; ;
		Vector2 bulletDirection = Vector2(cos(m_AngleRadians), sin(m_AngleRadians));
		Vector2 offset = bulletDirection * m_Paddle->GetPaddleSize();
		Vector2 bulletVelocity = bulletDirection * speed;

		EpicBall* epicball = FindInactiveEpicBall();
		Vector2 BulletPosRight = m_Paddle->GetPaddlePosition() + m_Paddle->GetPaddleSize()/2 + offset;

		if (epicball != nullptr)
		{
			epicball->Activate(Vector2(BulletPosRight));
			epicball->SetVelocity(bulletVelocity);
		}
	}

	if (m_ActivateWeapons == true)
	{
		m_TimeAliveAutomaticWeapon += delta;

		if (m_TimeAliveAutomaticWeapon >= 0.15)
		{
			m_TimeAliveAutomaticWeapon = 0;
			float speed = 120.0f;
			float m_AngleRadians = 90 / 180.0f * (float)M_PI; ;
			Vector2 bulletDirection = Vector2(cos(m_AngleRadians), sin(m_AngleRadians));
			Vector2 offset = bulletDirection * m_Paddle->GetPaddleSize();
			Vector2 bulletVelocity = bulletDirection * speed;

			Bullet* bullet = FindInactiveBullet();
			Vector2 BulletPosRight = m_Paddle->GetPaddlePosition() + offset;

			RightBullet* rightbullet = FindInactiveRightBullet();
			Vector2 RightBulletPosRight = m_Paddle->GetPaddlePosition() + m_Paddle->GetPaddleSize() + offset;

			if (bullet != nullptr || rightbullet != nullptr)
			{
				bullet->Activate(Vector2(BulletPosRight));
				bullet->SetVelocity(bulletVelocity);
			}

			if (rightbullet != nullptr)
			{
				rightbullet->Activate(Vector2(RightBulletPosRight));
				rightbullet->SetVelocity(bulletVelocity);
			}
		}
	}

	//weapon timer
	if (m_ActivateWeapons == true)
	{
		m_TimeAlive += delta;

		if (m_TimeAlive >= 2)
		{
			m_TimeAlive = 0;
			m_ActivateWeapons = false;
			m_ActivateSuperWeapons = false;
		}
	}

	//EpicBall timer
	if (m_ActivateSuperWeapons == true)
	{
		m_TimeAliveEpicBall += delta;

		if (m_TimeAliveEpicBall >= 1)
		{
			m_TimeAliveEpicBall = 0;
			m_ActivateSuperWeapons = false;
		}
	}

	//BrickSpawning
	m_TimeBrickSpawning += delta;
	if (m_TimeBrickSpawning >= 0.5)
	{
		m_TimeBrickSpawning = 0.0f;
	}

	//bricks
	for (unsigned int i = 0; i < m_Brick.size(); i++)
	{
		m_Brick[i]->OnUpdate(delta);
	}

	//ball starting position
	for (unsigned int b = 0; b < m_Ball.size(); b++)
	{
		if (m_Ball[b]->IsGamePaused() == true)
		{
			m_Ball[b]->SetBallPosition(Vector2(m_Paddle->GetPaddlePosition().x + m_Paddle->GetPaddleSize().x / 2, m_Paddle->GetPaddlePosition().y + 20 + m_Ball[b]->GetBallRadius()));
		}
	}

	//ball
	for (unsigned int b = 0; b < m_Ball.size(); b++)
	{
		m_Ball[b]->OnUpdate(delta);
	}

	//Power ups
	for (unsigned int d = 0; d < m_DropPaddleGrow.size(); d++)
	{
		m_DropPaddleGrow[d].y -= 2;

		if (m_DropPaddleGrow[d].y <= 0)
		{
			m_DropPaddleGrow.erase(std::remove(m_DropPaddleGrow.begin(), m_DropPaddleGrow.end(), m_DropPaddleGrow[d]), m_DropPaddleGrow.end());
		}
	}

	for (unsigned int d = 0; d < m_DropPaddleReduced.size(); d++)
	{
		m_DropPaddleReduced[d].y -= 2;

		if (m_DropPaddleReduced[d].y <= 0)
		{
			m_DropPaddleReduced.erase(std::remove(m_DropPaddleReduced.begin(), m_DropPaddleReduced.end(), m_DropPaddleReduced[d]), m_DropPaddleReduced.end());
		}
	}

	for (unsigned int b = 0; b < m_DropBallGrow.size(); b++)
	{
		m_DropBallGrow[b].y -= 2;

		if (m_DropBallGrow[b].y <= 0)
		{
			m_DropBallGrow.erase(std::remove(m_DropBallGrow.begin(), m_DropBallGrow.end(), m_DropBallGrow[b]), m_DropBallGrow.end());
		}
	}

	for (unsigned int b = 0; b < m_DropBallHealth.size(); b++)
	{
		m_DropBallHealth[b].y -= 2;

		if (m_DropBallHealth[b].y <= 0)
		{
			m_DropBallHealth.erase(std::remove(m_DropBallHealth.begin(), m_DropBallHealth.end(), m_DropBallHealth[b]), m_DropBallHealth.end());
		}
	}

	for (unsigned int w = 0; w < m_DropWeaponActivation.size(); w++)
	{
		m_DropWeaponActivation[w].y -= 2;

		if (m_DropWeaponActivation[w].y <= 0)
		{
			m_DropWeaponActivation.erase(std::remove(m_DropWeaponActivation.begin(), m_DropWeaponActivation.end(), m_DropWeaponActivation[w]), m_DropWeaponActivation.end());
		}
	}

	for (unsigned int w = 0; w < m_DropEpicBall.size(); w++)
	{
		m_DropEpicBall[w].y -= 2;

		if (m_DropEpicBall[w].y <= 0)
		{
			m_DropEpicBall.erase(std::remove(m_DropEpicBall.begin(), m_DropEpicBall.end(), m_DropEpicBall[w]), m_DropEpicBall.end());
		}
	}
}


//collision

void Game::CheckForCollision(float delta)
{
	CollisionBallPaddle(delta);
	CollisionBallBrick(delta);
	CollisionPowerUpPaddle(delta);
	CollisionBulletBrick(delta);
	CollisionRightBulletBrick(delta);
	CollisionEpicBallBrick(delta);
}

void Game::CollisionPowerUpPaddle(float delta)
{
	PaddleGrow(delta);
	PaddleReduced(delta);
	BallGrow(delta);
	BallPlusLife(delta);
	WeaponsActivate(delta);
	EpicBallActivate(delta);
}

void Game::CollisionBallBrick(float delta)
{
	for (unsigned int b = 0; b < m_Brick.size(); b++)
	{
		//for (unsigned int e = 0; e < m_Ball.size(); e++)
		//{
			if (m_Ball[0]->IsActive() && m_Brick[b]->IsActive() == true)
			{
				AABB aabb(Vector2(m_Brick[b]->GetBrickPosition().x, m_Brick[b]->GetBrickPosition().y),
					Vector2(m_Brick[b]->GetBrickPosition().x + 50, m_Brick[b]->GetBrickPosition().y + 30));

				if (m_Ball[0]->GetBallPosition().x + m_Ball[0]->GetBallRadius() >= aabb.lower.x && m_Ball[0]->GetBallPosition().x - m_Ball[0]->GetBallRadius() <= aabb.upper.x
					&& m_Ball[0]->GetBallPosition().y + m_Ball[0]->GetBallRadius() >= aabb.lower.y && m_Ball[0]->GetBallPosition().y - m_Ball[0]->GetBallRadius() <= aabb.upper.y)
				{
					m_Brick[b]->MinusLife();

					if (m_Brick[b]->Lives() == 0)
					{
						//The powerup will spawn from the center of the brick 
						Vector2 BrickCenter(m_Brick[b]->GetBrickPosition().x + 25, m_Brick[b]->GetBrickPosition().y + 15);
						m_Brick[b]->Deactivate();
						m_Points = m_Points + 1;

						//When a Brick is destroyed there is a 1 in 3 chance of spawning a powerup 
						int PowerSpawn = Math::RandomInt(1, 3);
						//5 implemented powers
						if (PowerSpawn == 1)
						{
							int randomNumber = Math::RandomInt(1, 5);

							if (randomNumber == 1)
							{
								DropPaddleGrow(BrickCenter);
								m_DropPaddleGrow.push_back(BrickCenter);
							}
							if (randomNumber == 2)
							{
								DropBallGrow(BrickCenter);
								m_DropBallGrow.push_back(BrickCenter);
							}
							if (randomNumber == 3)
							{
								DropEpicBall(BrickCenter);
								m_DropEpicBall.push_back(BrickCenter);
							}
							if (randomNumber == 4)
							{
								DropBallHealth(BrickCenter);
								m_DropBallHealth.push_back(BrickCenter);
							}
							if (randomNumber == 5)
							{
								DropWeaponActivation(BrickCenter);
								m_DropWeaponActivation.push_back(BrickCenter);
							}
						}
					}
					if (m_Ball[0]->GetBallPosition().y <= aabb.lower.y)
					{
						m_Ball[0]->BounceBackLower();
					}

					if (m_Ball[0]->GetBallPosition().y + 12 >= aabb.upper.y)
					{
						m_Ball[0]->BounceBackUpper();
					}

					if (m_Ball[0]->GetBallPosition().x <= aabb.lower.x)
					{
						m_Ball[0]->BounceBackLeft();
					}

					if (m_Ball[0]->GetBallPosition().x >= aabb.upper.x)
					{
						m_Ball[0]->BounceBackRight();
					}
				}
			}
	}
}

void Game::CollisionEpicBallBrick(float delta)
{
	for (unsigned int b = 0; b < m_Brick.size(); b++)
	{
		if (m_EpicBall[0]->IsActive() && m_Brick[b]->IsActive() == true)
		{
			AABB aabb(Vector2(m_Brick[b]->GetBrickPosition().x, m_Brick[b]->GetBrickPosition().y),
				Vector2(m_Brick[b]->GetBrickPosition().x + 50, m_Brick[b]->GetBrickPosition().y + 30));

			if (m_EpicBall[0]->GetEpicBallPosition().x + m_EpicBall[0]->GetEpicBallRadius() >= aabb.lower.x && m_EpicBall[0]->GetEpicBallPosition().x - m_EpicBall[0]->GetEpicBallRadius() <= aabb.upper.x
				&& m_EpicBall[0]->GetEpicBallPosition().y + m_EpicBall[0]->GetEpicBallRadius() >= aabb.lower.y && m_EpicBall[0]->GetEpicBallPosition().y + m_EpicBall[0]->GetEpicBallRadius() <= aabb.upper.y)
			{
				m_Brick[b]->MinusLife();

				if (m_Brick[b]->Lives() == 0)
				{
					//The powerup will spawn from the center of the brick 
					Vector2 BrickCenter(m_Brick[b]->GetBrickPosition().x + 25, m_Brick[b]->GetBrickPosition().y + 15);
					m_Brick[b]->Deactivate();
					m_Points = m_Points + 1;

					//When a Brick is destroyed there is a 1 in 3 chance of spawning a powerup 
					int PowerSpawn = Math::RandomInt(1, 3);
					//5 implemented powers
					if (PowerSpawn == 1)
					{
						int randomNumber = Math::RandomInt(1, 5);

						if (randomNumber == 1)
						{
							DropPaddleGrow(BrickCenter);
							m_DropPaddleGrow.push_back(BrickCenter);
						}
						if (randomNumber == 2)
						{
							DropBallGrow(BrickCenter);
							m_DropBallGrow.push_back(BrickCenter);
						}
						if (randomNumber == 3)
						{
							DropEpicBall(BrickCenter);
							m_DropEpicBall.push_back(BrickCenter);
						}
						if (randomNumber == 4)
						{
							DropBallHealth(BrickCenter);
							m_DropBallHealth.push_back(BrickCenter);
						}
						if (randomNumber == 5)
						{
							DropWeaponActivation(BrickCenter);
							m_DropWeaponActivation.push_back(BrickCenter);
						}
					}
				}
			}
		}
	}
}

void Game::CollisionBulletBrick(float delta)
{
	for (unsigned int b = 0; b < m_Bullets.size(); b++)
	{
		for (unsigned int a = 0; a < m_Brick.size(); a++)
		{
			if (m_Bullets[b]->IsActive() && m_Brick[a]->IsActive() == true)
			{
				AABB aabb(Vector2(m_Brick[a]->GetBrickPosition().x, m_Brick[a]->GetBrickPosition().y),
					Vector2(m_Brick[a]->GetBrickPosition().x + 50, m_Brick[a]->GetBrickPosition().y + 30));

				if (m_Bullets[b]->GetBulletPosition().x + m_Bullets[b]->GetBulletRadius() >= aabb.lower.x && m_Bullets[b]->GetBulletPosition().x - m_Bullets[b]->GetBulletRadius() <= aabb.upper.x
					&& m_Bullets[b]->GetBulletPosition().y + m_Bullets[b]->GetBulletRadius() >= aabb.lower.y && m_Bullets[b]->GetBulletPosition().y - m_Bullets[b]->GetBulletRadius() <= aabb.upper.y)
				{
					m_Brick[a]->MinusLife();
					m_Bullets[b]->Deactivate();

					if (m_Brick[a]->Lives() == 0)
					{
						//The powerup will spawn from the center of the brick 
						Vector2 BrickCenter(m_Brick[a]->GetBrickPosition().x + 25, m_Brick[a]->GetBrickPosition().y + 15);
						m_Brick[a]->Deactivate();
						m_Points = m_Points + 1;

						//When a Brick is destroyed there is a 1 in 3 chance of spawning a powerup 
						int PowerSpawn = Math::RandomInt(1, 3);
						//5 implemented powers
						if (PowerSpawn == 1)
						{
							int randomNumber = Math::RandomInt(1, 5);

							if (randomNumber == 1)
							{
								DropPaddleGrow(BrickCenter);
								m_DropPaddleGrow.push_back(BrickCenter);
							}
							if (randomNumber == 2)
							{
								DropBallGrow(BrickCenter);
								m_DropBallGrow.push_back(BrickCenter);
							}
							if (randomNumber == 3)
							{
								DropEpicBall(BrickCenter);
								m_DropEpicBall.push_back(BrickCenter);
							}
							if (randomNumber == 4)
							{
								DropBallHealth(BrickCenter);
								m_DropBallHealth.push_back(BrickCenter);
							}
							if (randomNumber == 5)
							{
								DropWeaponActivation(BrickCenter);
								m_DropWeaponActivation.push_back(BrickCenter);
							}
						}
					}
				}
			}
		}
	}
}

void Game::CollisionRightBulletBrick(float delta)
{
	for (unsigned int b = 0; b < m_RightBullets.size(); b++)
	{
		for (unsigned int a = 0; a < m_Brick.size(); a++)
		{
			if (m_RightBullets[b]->IsActive() && m_Brick[a]->IsActive() == true)
			{
				AABB aabb(Vector2(m_Brick[a]->GetBrickPosition().x, m_Brick[a]->GetBrickPosition().y),
					Vector2(m_Brick[a]->GetBrickPosition().x + 50, m_Brick[a]->GetBrickPosition().y + 30));

				if (m_RightBullets[b]->GetBulletPosition().x + m_RightBullets[b]->GetBulletRadius() >= aabb.lower.x && m_RightBullets[b]->GetBulletPosition().x - m_RightBullets[b]->GetBulletRadius() <= aabb.upper.x
					&& m_RightBullets[b]->GetBulletPosition().y + m_RightBullets[b]->GetBulletRadius() >= aabb.lower.y && m_RightBullets[b]->GetBulletPosition().y - m_RightBullets[b]->GetBulletRadius() <= aabb.upper.y)
				{
					m_Brick[a]->MinusLife();
					m_RightBullets[b]->Deactivate();

					if (m_Brick[a]->Lives() == 0)
					{
						//The powerup will spawn from the center of the brick 
						Vector2 BrickCenter(m_Brick[a]->GetBrickPosition().x + 25, m_Brick[a]->GetBrickPosition().y + 15);
						m_Brick[a]->Deactivate();
						m_Points = m_Points + 1;

						//When a Brick is destroyed there is a 1 in 3 chance of spawning a powerup 
						int PowerSpawn = Math::RandomInt(1, 3);
						//5 implemented powers
						if (PowerSpawn == 1)
						{
							int randomNumber = Math::RandomInt(1, 5);

							if (randomNumber == 1)
							{
								DropPaddleGrow(BrickCenter);
								m_DropPaddleGrow.push_back(BrickCenter);
							}
							if (randomNumber == 2)
							{
								DropBallGrow(BrickCenter);
								m_DropBallGrow.push_back(BrickCenter);
							}
							if (randomNumber == 3)
							{
								DropEpicBall(BrickCenter);
								m_DropEpicBall.push_back(BrickCenter);
							}
							if (randomNumber == 4)
							{
								DropBallHealth(BrickCenter);
								m_DropBallHealth.push_back(BrickCenter);
							}
							if (randomNumber == 5)
							{
								DropWeaponActivation(BrickCenter);
								m_DropWeaponActivation.push_back(BrickCenter);
							}
						}
					}
				}
			}
		}
	}
}

void Game::CollisionBallPaddle(float delta)
{
	for (unsigned int p = 0; p < m_Brick.size(); p++)
	{
		for (unsigned int e = 0; e < m_Ball.size(); e++)
		{
			if (m_Ball[0]->IsActive() && m_Paddle->IsActive() == true)
			{
				AABB aabb(Vector2(m_Paddle->GetPaddlePosition().x, m_Paddle->GetPaddlePosition().y),
					Vector2(m_Paddle->GetPaddlePosition().x + m_Paddle->GetPaddleSize().x, m_Paddle->GetPaddlePosition().y + 20));

				if (m_Ball[0]->GetBallPosition().x + m_Ball[0]->GetBallRadius() >= aabb.lower.x && m_Ball[0]->GetBallPosition().x - m_Ball[0]->GetBallRadius() <= aabb.upper.x
					&& m_Ball[0]->GetBallPosition().y + m_Ball[0]->GetBallRadius() >= aabb.lower.y && m_Ball[0]->GetBallPosition().y - m_Ball[0]->GetBallRadius() <= aabb.upper.y)
				{
					float radius = m_Ball[0]->GetBallRadius() * 2;
					if ((GetScreenHeight() * 0.1) <= m_Ball[0]->GetBallPosition().y + radius)
					{
						m_Ball[0]->SetBallPosition(Vector2(m_Ball[0]->GetBallPosition().x, m_Paddle->GetPaddlePosition().y + 20 + m_Ball[0]->GetBallRadius()));
						m_Ball[0]->BounceBackUpper();
					}
				}
			}
		}
	}
}


//Power ups

void Game::PaddleGrow(float delta)
{
	for (unsigned int p = 0; p < m_DropPaddleGrow.size(); p++)
	{
		AABB aabb(Vector2(m_Paddle->GetPaddlePosition().x, m_Paddle->GetPaddlePosition().y),
			Vector2(m_Paddle->GetPaddlePosition().x + m_Paddle->GetPaddleSize().x, m_Paddle->GetPaddlePosition().y + 20));

		if (m_DropPaddleGrow[p].x >= aabb.lower.x && m_DropPaddleGrow[p].x <= aabb.upper.x
			&& m_DropPaddleGrow[p].y >= aabb.lower.y && m_DropPaddleGrow[p].y <= aabb.upper.y)
		{
			m_DropPaddleGrow.erase(std::remove(m_DropPaddleGrow.begin(), m_DropPaddleGrow.end(), m_DropPaddleGrow[p]), m_DropPaddleGrow.end());
			m_Paddle->GrowPaddleSize();
		}
	}
}

void Game::PaddleReduced(float delta)
{
	for (unsigned int r = 0; r < m_DropPaddleReduced.size(); r++)
	{
		AABB aabb(Vector2(m_Paddle->GetPaddlePosition().x, m_Paddle->GetPaddlePosition().y),
			Vector2(m_Paddle->GetPaddlePosition().x + m_Paddle->GetPaddleSize().x, m_Paddle->GetPaddlePosition().y + 20));

		if (m_DropPaddleReduced[r].x >= aabb.lower.x && m_DropPaddleReduced[r].x <= aabb.upper.x
			&& m_DropPaddleReduced[r].y >= aabb.lower.y && m_DropPaddleReduced[r].y <= aabb.upper.y)
		{
			m_DropPaddleReduced.erase(std::remove(m_DropPaddleReduced.begin(), m_DropPaddleReduced.end(), m_DropPaddleReduced[r]), m_DropPaddleReduced.end());
			m_Paddle->ReducedPaddleSize();
		}
	}
}

void Game::BallGrow(float delta)
{
	for (unsigned int b = 0; b < m_DropBallGrow.size(); b++)
	{
		AABB aabb(Vector2(m_Paddle->GetPaddlePosition().x, m_Paddle->GetPaddlePosition().y),
			Vector2(m_Paddle->GetPaddlePosition().x + m_Paddle->GetPaddleSize().x, m_Paddle->GetPaddlePosition().y + 20));

		if (m_DropBallGrow[b].x >= aabb.lower.x && m_DropBallGrow[b].x <= aabb.upper.x
			&& m_DropBallGrow[b].y >= aabb.lower.y && m_DropBallGrow[b].y <= aabb.upper.y)
		{
			m_DropBallGrow.erase(std::remove(m_DropBallGrow.begin(), m_DropBallGrow.end(), m_DropBallGrow[b]), m_DropBallGrow.end());
			for (unsigned int e = 0; e < m_Ball.size(); e++)
			{
				m_Ball[0]->GrowBallSize();
			}
		}
	}
}

void Game::BallPlusLife(float delta)
{
	for (unsigned int a = 0; a < m_DropBallHealth.size(); a++)
	{
		AABB aabb(Vector2(m_Paddle->GetPaddlePosition().x, m_Paddle->GetPaddlePosition().y),
			Vector2(m_Paddle->GetPaddlePosition().x + m_Paddle->GetPaddleSize().x, m_Paddle->GetPaddlePosition().y + 20));

		if (m_DropBallHealth[a].x >= aabb.lower.x && m_DropBallHealth[a].x <= aabb.upper.x
			&& m_DropBallHealth[a].y >= aabb.lower.y && m_DropBallHealth[a].y <= aabb.upper.y)
		{
			m_DropBallHealth.erase(std::remove(m_DropBallHealth.begin(), m_DropBallHealth.end(), m_DropBallHealth[a]), m_DropBallHealth.end());
			for (unsigned int e = 0; e < m_Ball.size(); e++)
			{
				m_Ball[e]->PlusLife();
			}
		}
	}
}

void Game::WeaponsActivate(float delta)
{
	for (unsigned int w = 0; w < m_DropWeaponActivation.size(); w++)
	{
		AABB aabb(Vector2(m_Paddle->GetPaddlePosition().x, m_Paddle->GetPaddlePosition().y),
			Vector2(m_Paddle->GetPaddlePosition().x + m_Paddle->GetPaddleSize().x, m_Paddle->GetPaddlePosition().y + 20));

		if (m_DropWeaponActivation[w].x >= aabb.lower.x && m_DropWeaponActivation[w].x <= aabb.upper.x
			&& m_DropWeaponActivation[w].y >= aabb.lower.y && m_DropWeaponActivation[w].y <= aabb.upper.y)
		{
			m_DropWeaponActivation.erase(std::remove(m_DropWeaponActivation.begin(), m_DropWeaponActivation.end(), m_DropWeaponActivation[w]), m_DropWeaponActivation.end());
			m_ActivateWeapons = true;
		}
	}
}

void Game::EpicBallActivate(float delta)
{
	for (unsigned int w = 0; w < m_DropEpicBall.size(); w++)
	{
		AABB aabb(Vector2(m_Paddle->GetPaddlePosition().x, m_Paddle->GetPaddlePosition().y),
			Vector2(m_Paddle->GetPaddlePosition().x + m_Paddle->GetPaddleSize().x, m_Paddle->GetPaddlePosition().y + 20));

		if (m_DropEpicBall[w].x >= aabb.lower.x && m_DropEpicBall[w].x <= aabb.upper.x
			&& m_DropEpicBall[w].y >= aabb.lower.y && m_DropEpicBall[w].y <= aabb.upper.y)
		{
			m_DropEpicBall.erase(std::remove(m_DropEpicBall.begin(), m_DropEpicBall.end(), m_DropEpicBall[w]), m_DropEpicBall.end());
			m_ActivateSuperWeapons = true;
		}
	}
}

void Game::PowerUpRender(BatchRenderer& batchRenderer)
{
	if (isPaddleGrowReadyToDrop == true)
	{
		for (unsigned int d = 0; d < m_DropPaddleGrow.size(); d++)
		{
			m_TextPaddleGrow.SetPosition(Vector2(m_DropPaddleGrow[d].x, m_DropPaddleGrow[d].y + 10));
			batchRenderer.RenderSpriteFont(m_TextPaddleGrow);

			batchRenderer.RenderCircle(m_DropPaddleGrow[d], m_PaddleGrowDropRadius, m_PaddleGrowDropColor);
		}
	}

	if (isPaddleReducedReadyToDrop == true)
	{
		for (unsigned int a = 0; a < m_DropPaddleReduced.size(); a++)
		{
			m_TextPaddleReduced.SetPosition(Vector2(m_DropPaddleReduced[a].x, m_DropPaddleReduced[a].y + 10));
			batchRenderer.RenderSpriteFont(m_TextPaddleReduced);

			batchRenderer.RenderCircle(m_DropPaddleReduced[a], m_PaddleGrowDropRadius, m_PaddleReducedDropColor);
		}
	}

	if (isBallGrowReadyToDrop == true)
	{
		for (unsigned int c = 0; c < m_DropBallGrow.size(); c++)
		{
			m_TextBallBigger.SetPosition(Vector2(m_DropBallGrow[c].x, m_DropBallGrow[c].y + 10));
			batchRenderer.RenderSpriteFont(m_TextBallBigger);


			batchRenderer.RenderCircle(m_DropBallGrow[c], m_BallGrowDropRadius, m_BallGrowDropColor);
		}
	}

	if (isBallHealthReadyToDrop == true)
	{
		for (unsigned int b = 0; b < m_DropBallHealth.size(); b++)
		{
			m_TextBallHealth.SetPosition(Vector2(m_DropBallHealth[b].x, m_DropBallHealth[b].y + 10));
			batchRenderer.RenderSpriteFont(m_TextBallHealth);

			batchRenderer.RenderCircle(m_DropBallHealth[b], m_BallGrowDropRadius, m_BallHealthDropColor);
		}
	}

	if (isWeaponActiveReadyToDrop == true)
	{
		for (unsigned int n = 0; n < m_DropWeaponActivation.size(); n++)
		{
			m_TextWeaponActivated.SetPosition(Vector2(m_DropWeaponActivation[n].x, m_DropWeaponActivation[n].y + 10));
			batchRenderer.RenderSpriteFont(m_TextWeaponActivated);

			batchRenderer.RenderCircle(m_DropWeaponActivation[n], m_BallGrowDropRadius, m_WeaponActiveDropColor);
		}
	}

	if (isSuperWeaponActiveReadyToDrop == true)
	{
		for (unsigned int z = 0; z < m_DropEpicBall.size(); z++)
		{
			m_TextSuperWeaponActivated.SetPosition(Vector2(m_DropEpicBall[z].x, m_DropEpicBall[z].y + 10));
			batchRenderer.RenderSpriteFont(m_TextSuperWeaponActivated);

			batchRenderer.RenderCircle(m_DropEpicBall[z], m_BallGrowDropRadius, m_SuperWeaponActiveDropColor);
		}
	}

	if (m_ActivateWeapons == true)
	{
		batchRenderer.RenderSpriteFont(m_TextWeaponActivatedScreen);
	}
}


//Switches

void Game::DeletePaddleGrow(Vector2 position)
{
	isPaddleGrowReadyToDrop = false;
}

void Game::DropPaddleGrow(Vector2 position)
{
	isPaddleGrowReadyToDrop = true;
}

void Game::DeleteBallGrow(Vector2 position)
{
	isBallGrowReadyToDrop = false;
}

void Game::DropBallGrow(Vector2 position)
{
	isBallGrowReadyToDrop = true;
}

void Game::DeletePaddleReduced(Vector2 position)
{
	isPaddleReducedReadyToDrop = false;
}

void Game::DropPaddleReduced(Vector2 position)
{
	isPaddleReducedReadyToDrop = true;
}

void Game::DeleteBallHealth(Vector2 position)
{
	isBallHealthReadyToDrop = false;
}

void Game::DropBallHealth(Vector2 position)
{
	isBallHealthReadyToDrop = true;
}

void Game::DeleteEpicBall(Vector2 position)
{
	isSuperWeaponActiveReadyToDrop = false;
}

void Game::DropEpicBall(Vector2 position)
{
	isSuperWeaponActiveReadyToDrop = true;
}

void Game::DeleteWeaponActivation(Vector2 position)
{
	isWeaponActiveReadyToDrop = false;
}

void Game::DropWeaponActivation(Vector2 position)
{
	isWeaponActiveReadyToDrop = true;
}

void Game::DeleteRightWeaponActivation(Vector2 position)
{
	isRightWeaponActiveReadyToDrop = false;
}

void Game::DropRightWeaponActivation(Vector2 position)
{
	isRightWeaponActiveReadyToDrop = true;
}


//text

void Game::Text()
{
	m_TextObjective.SetPosition(10, GetScreenHeight() - GetScreenHeight() + 10);
	m_TextObjective.SetScale(0.5, 0.5);

	m_TextGameOver.SetText("GAME OVER!");
	m_TextGameOver.SetAnchor(0.5, 0);
	m_TextGameOver.SetPosition(GetHalfScreenWidth(), GetHalfScreenHeight());

	m_TextGameWon.SetText("GAME WON!");
	m_TextGameWon.SetAnchor(0.5, 0);
	m_TextGameWon.SetPosition(GetHalfScreenWidth(), GetHalfScreenHeight());

	m_TextEscape.SetText("Press the Escape Key to close the game.");
	m_TextEscape.SetAnchor(0.5, 0);
	m_TextEscape.SetScale(0.5, 0.5);
	m_TextEscape.SetPosition(GetHalfScreenWidth(), GetHalfScreenHeight() - 50);

	m_TextPaddleGrow.SetText("Increase Paddle Size");
	m_TextPaddleGrow.SetAnchor(0.5, 0);
	m_TextPaddleGrow.SetScale(0.25, 0.25);

	m_TextPaddleReduced.SetText("Reduce Paddle Size");
	m_TextPaddleReduced.SetAnchor(0.5, 0);
	m_TextPaddleReduced.SetScale(0.25, 0.25);

	m_TextBallBigger.SetText("Increase Ball Size");
	m_TextBallBigger.SetAnchor(0.5, 0);
	m_TextBallBigger.SetScale(0.25, 0.25);

	m_TextBallHealth.SetText("Increase Health");
	m_TextBallHealth.SetAnchor(0.5, 0);
	m_TextBallHealth.SetScale(0.25, 0.25);

	m_TextWeaponActivated.SetText("Activate Weapons");
	m_TextWeaponActivated.SetAnchor(0.5, 0);
	m_TextWeaponActivated.SetScale(0.25, 0.25);

	m_TextWeaponActivatedScreen.SetText("Weapons Activated!");
	m_TextWeaponActivatedScreen.SetAnchor(0.5, 0);
	m_TextWeaponActivatedScreen.SetScale(0.5, 0.5);
	m_TextWeaponActivatedScreen.SetPosition(GetHalfScreenWidth(), GetHalfScreenHeight() - GetHalfScreenHeight() + 20);

	m_TextSuperWeaponActivated.SetText("Activate Epic Bullet");
	m_TextSuperWeaponActivated.SetAnchor(0.5, 0);
	m_TextSuperWeaponActivated.SetScale(0.25, 0.25);
}


//Bricks

void Game::BrickSpawn()
{
	//Each brick should be randomly assigned a health value between 1 and 4 

	for (unsigned int d = 0; d < m_Brick.size(); d++)
	{
		m_Brick[d]->Deactivate();
	}

	for (unsigned int i = 0; i < m_Brick.size(); i++)
	{
		if (i >= 1 && i <= 13)
		{
			m_Brick[i]->SetBrickPosition(Vector2(m_Brick[i - 1]->GetBrickPosition().x + 52, (m_Brick[i - 1]->GetBrickPosition().y)));
			m_Brick[i]->SetBrickLives(Math::RandomInt(1, 4));
			m_Brick[i]->Activate();
		}

		if (i == 14)
		{
			m_Brick[i]->SetBrickPosition(Vector2(m_Brick[0]->GetBrickPosition().x, (m_Brick[i - 1]->GetBrickPosition().y - 32)));
			m_Brick[i]->SetBrickLives(Math::RandomInt(1, 4));
			m_Brick[i]->Activate();
		}

		if (i >= 15 && i <= 27)
		{

			m_Brick[i]->SetBrickPosition(Vector2(m_Brick[i - 1]->GetBrickPosition().x + 52, (m_Brick[14]->GetBrickPosition().y)));
			m_Brick[i]->SetBrickLives(Math::RandomInt(1, 4));
			m_Brick[i]->Activate();
		}

		if (i == 28)
		{
			m_Brick[i]->SetBrickPosition(Vector2(m_Brick[14]->GetBrickPosition().x, (m_Brick[14]->GetBrickPosition().y - 32)));
			m_Brick[i]->SetBrickLives(Math::RandomInt(1, 4));
			m_Brick[i]->Activate();
		}

		if (i >= 29 && i <= 41)
		{

			m_Brick[i]->SetBrickPosition(Vector2(m_Brick[i - 1]->GetBrickPosition().x + 52, (m_Brick[28]->GetBrickPosition().y)));
			m_Brick[i]->SetBrickLives(Math::RandomInt(1, 4));
			m_Brick[i]->Activate();
		}

		if (i == 42)
		{
			m_Brick[i]->SetBrickPosition(Vector2(m_Brick[28]->GetBrickPosition().x, (m_Brick[28]->GetBrickPosition().y - 32)));
			m_Brick[i]->SetBrickLives(Math::RandomInt(1, 4));
			m_Brick[i]->Activate();
		}

		if (i >= 43 && i <= 55)
		{
			m_Brick[i]->SetBrickPosition(Vector2(m_Brick[i - 1]->GetBrickPosition().x + 52, (m_Brick[42]->GetBrickPosition().y)));
			m_Brick[i]->SetBrickLives(Math::RandomInt(1, 4));
			m_Brick[i]->Activate();
		}

		if (i == 56)
		{
			m_Brick[i]->SetBrickPosition(Vector2(m_Brick[42]->GetBrickPosition().x, (m_Brick[42]->GetBrickPosition().y - 32)));
			m_Brick[i]->SetBrickLives(Math::RandomInt(1, 4));
			m_Brick[i]->Activate();
		}

		if (i >= 57 && i <= 70)
		{
			m_Brick[i]->SetBrickPosition(Vector2(m_Brick[i - 1]->GetBrickPosition().x + 52, (m_Brick[56]->GetBrickPosition().y)));
			m_Brick[i]->SetBrickLives(Math::RandomInt(1, 4));
			m_Brick[i]->Activate();
		}
	}
}


//Finding bullets

Bullet* Game::FindInactiveBullet()
{
	for (size_t i = 0; i < m_Bullets.size(); i++)
	{
		if (m_Bullets[i]->IsActive() == false)
		{
			return m_Bullets[i];
		}
	}
	return nullptr;
}

RightBullet* Game::FindInactiveRightBullet()
{
	for (size_t i = 0; i < m_RightBullets.size(); i++)
	{
		if (m_RightBullets[i]->IsActive() == false)
		{
			return m_RightBullets[i];
		}
	}
	return nullptr;
}

EpicBall *Game::FindInactiveEpicBall()
{
	for (size_t i = 0; i < m_EpicBall.size(); i++)
	{
		if (m_EpicBall[i]->IsActive() == false)
		{
			return m_EpicBall[i];
		}
	}
	return nullptr;
}


//Collision from A3 Solution testing

Brick* Game::FindNearestBrickOnCircleSweep(LineSegment path, float radius, Vector2& outIntersectionPoint, Vector2& outNormal)
{
	Brick* pClosestBrick = nullptr;
	float closestDistance = FLT_MAX;

	for (Brick* pBrick : m_Brick)
	{
		// Skip inactive Bricks.
		if (pBrick->IsActive() == false)
			continue;

		AABB box;
		box.lower = pBrick->GetBrickPosition() - pBrick->GetSize() / 2;
		box.upper = pBrick->GetBrickPosition() + pBrick->GetSize() / 2;

		Vector2 point;
		Vector2 normal;
		bool hit = box.DoesIntersectWithCircle(path, radius, point, normal);

		// If hit and it's not a brick marked to be ignored.
		if (hit)
		{
			float distance = point.DistanceSquared(path.start);
			if (distance < closestDistance)
			{
				pClosestBrick = pBrick;
				closestDistance = distance;

				outIntersectionPoint = point;
				outNormal = normal;
			}
		}
	}

	return pClosestBrick;
}

void Game::UpdateBalls(float delta)
{
	for (Ball* pBall : m_Ball)
	{
		// Skip inactive Balls.
		if (pBall->IsActive() == false)
			continue;

		Vector2 position = pBall->GetBallPosition();
		Vector2 velocity = pBall->GetBallVelocity();
		Vector2 displacementRemaining = pBall->GetBallVelocity() * delta;

		// Keep looping, until there are no more collisions,
		// reducing the displacementRemaining each time.
		bool collided = true;
		while (collided)
		{
			Vector2 intersectionPoint;
			Vector2 normal;
			LineSegment path(position, position + displacementRemaining);
			Brick* pBrickHit = FindNearestBrickOnCircleSweep(path, pBall->GetBallRadius(), intersectionPoint, normal);

			if (pBrickHit != nullptr)
			{
				// Reduce the displacementRemaining.
				displacementRemaining = path.end - intersectionPoint;

				// Reflect the velocity and displacementRemaining off the normal.
				velocity = velocity - 2 * normal * (velocity.DotProduct(normal));
				displacementRemaining = displacementRemaining - 2 * normal * (displacementRemaining.DotProduct(normal));

				// Move to the intersection point.
				position = intersectionPoint;

				// Destroy the brick.
				if (pBrickHit->IsActive())
				{
					pBrickHit->Deactivate();
				}
			}
			else
			{
				// No collision, so move the full amount and exit this loop.
				position = path.end;
				collided = false;
			}
		}

		// Update the ball with it's new properties.
		pBall->SetBallPosition(position);
		pBall->SetBallVelocity(velocity);
	}
}