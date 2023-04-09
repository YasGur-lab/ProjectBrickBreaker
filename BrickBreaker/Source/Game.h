#pragma once

#include <GameDev2D.h>
#include "Paddle.h"
#include "Brick.h"
#include "Ball.h"
#include <vector>
#include "Bullet.h"
#include "RightBullet.h"
#include "EpicBall.h"

using namespace GameDev2D;

class Game
{
public:
	Game();
	~Game();

	// Write game logic in the Update method.
	void OnUpdate(float delta);
	void OnUpdateCode(float delta);

	// Render game objects in the Draw method.
	void OnRender(BatchRenderer& batchRenderer);

	// Input event methods.
	void OnKeyEvent(KeyCode keyCode, KeyState keyState);
	void OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY);
	void OnMouseMovedEvent(float mouseX, float mouseY);

	//Collision
	void CheckForCollision(float delta);

	void CollisionBallBrick(float delta);
	void CollisionEpicBallBrick(float delta);
	void CollisionBallPaddle(float delta);
	void CollisionBulletBrick(float delta);
	void CollisionRightBulletBrick(float delta);
	void CollisionPowerUpPaddle(float delta);
	
	//PowerUps
	void DropPaddleGrow(Vector2 position);
	void DeletePaddleGrow(Vector2 position);	
	std::vector<Vector2> m_DropPaddleGrow;

	void DeletePaddleReduced(Vector2 position);
	void DropPaddleReduced(Vector2 position);
	std::vector<Vector2> m_DropPaddleReduced;

	void DropBallGrow(Vector2 position);
	void DeleteBallGrow(Vector2 position);
	std::vector<Vector2> m_DropBallGrow;

	void DeleteBallHealth(Vector2 position);
	void DropBallHealth(Vector2 position);
	std::vector<Vector2> m_DropBallHealth;

	void DeleteEpicBall(Vector2 position);
	void DropEpicBall(Vector2 position);
	std::vector<Vector2> m_DropEpicBall;

	void PaddleGrow(float delta);
	void PaddleReduced(float delta);
	void BallGrow(float delta);
	void BallPlusLife(float delta);
	void WeaponsActivate(float delta);
	void EpicBallActivate(float delta);
	void PowerUpRender(BatchRenderer& batchRenderer);

	//bullet
	Bullet* FindInactiveBullet();
	std::vector<Vector2> m_DropWeaponActivation;
	void DeleteWeaponActivation(Vector2 position);
	void DropWeaponActivation(Vector2 position);

	//Right bullet
	RightBullet* FindInactiveRightBullet();
	EpicBall* FindInactiveEpicBall();
	std::vector<Vector2> m_DropRightWeaponActivation;
	void DeleteRightWeaponActivation(Vector2 position);
	void DropRightWeaponActivation(Vector2 position);

	//Text
	void Text();

	//Bricks
	void BrickSpawn();


	//test collision
	Brick* FindNearestBrickOnCircleSweep(LineSegment path, float radius, Vector2& outIntersectionPoint, Vector2& outNormal);
	void UpdateBalls(float delta);


private:
	Paddle* m_Paddle;
	std::vector<Brick*> m_Brick;
	std::vector <Ball*> m_Ball;
	int m_Points;
	SpriteFont m_TextObjective;
	float m_TimeAlive;
	float m_TimeAliveEpicBall;
	float m_TimeAliveAutomaticWeapon;
	float m_TimeBrickSpawning;

	//GameStatus
	SpriteFont m_TextGameOver;
	SpriteFont m_TextGameWon;
	Vector2 m_BlackScreenSize;
	Vector2 m_BlackScreenPosition;
	SpriteFont m_TextEscape;

	//PowerUps
	//PaddleGrow
	SpriteFont m_TextPaddleGrow;
	bool isPaddleGrowReadyToDrop;
	float m_PaddleGrowDropRadius;
	Color m_PaddleGrowDropColor;
	//PaddleReduced
	SpriteFont m_TextPaddleReduced;
	bool isPaddleReducedReadyToDrop;
	Color m_PaddleReducedDropColor;
	//BallGrow
	SpriteFont m_TextBallBigger;
	bool isBallGrowReadyToDrop;
	float m_BallGrowDropRadius;
	Color m_BallGrowDropColor;
	//BallHealth
	SpriteFont m_TextBallHealth;
	bool isBallHealthReadyToDrop;
	Color m_BallHealthDropColor;
	//Add balls
	int m_NumOfBall;	
	bool isBallAdditionReadyToDrop;
	Color m_BallAdditionDropColor;
	int m_Nextball;
	//Bullets
	SpriteFont m_TextWeaponActivated;
	SpriteFont m_TextWeaponActivatedScreen;
	std::vector<Bullet*> m_Bullets;
	bool isWeaponActiveReadyToDrop;
	Color m_WeaponActiveDropColor;
	bool m_ActivateWeapons;
	//EpicBall
	SpriteFont m_TextSuperWeaponActivated;
	SpriteFont m_TextSuperWeaponActivatedScreen;
	std::vector<EpicBall*> m_EpicBall;
	bool isSuperWeaponActiveReadyToDrop;
	Color m_SuperWeaponActiveDropColor;
	bool m_ActivateSuperWeapons;
	//RightBullet
	SpriteFont m_TextRightWeaponActivated;
	SpriteFont m_TextRightWeaponActivatedScreen;
	std::vector<RightBullet*> m_RightBullets;
	bool isRightWeaponActiveReadyToDrop;
	Color m_RightWeaponActiveDropColor;
	bool m_ActivateRightWeapons;
};
