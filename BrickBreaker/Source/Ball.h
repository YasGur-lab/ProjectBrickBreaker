#pragma once
#include <GameDev2D.h>
#include <vector>

namespace GameDev2D
{
	class Ball
	{
	private:
		static const int NUM_EDGES;
		static const int NUM_PASSES;
		static const float SPEED;
	public:
		Ball ();
		~Ball();

		// Write game logic in the Update method.
		void OnUpdate(float delta);

		// Render game objects in the Draw method.
		void OnRender(BatchRenderer& batchRenderer);

		void OnKeyEvent(KeyCode keyCode, KeyState keyState);
		void OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY);

		void Reset();
		void BounceBackUpper();
		void BounceBackLower();
		void BounceBackRight();
		void BounceBackLeft();		
		bool IsBallOfEdge();
		int GetNumPasses();
		bool IsGamePaused();
		void Deactivate();
		void Activate();
		bool IsActive();
		float GetBallRadius();
		float GetBallSpeed();
		float GetBallAngle();
		Vector2 GetBallVelocity();
		Vector2 GetBallPosition();
		void SetBallPosition(Vector2 position);
		void SetBallVelocity(Vector2 velocity);
		void SetBallAngle(float angle);
		void SetBallSpeed(float speed);
		void SetBallPaused(bool paused);
		void SetBallRadius(float radius);
		void SetBallHealth(int lives);

		void ResetSize();

		// life methods
		void MinusLife();
		void PlusLife();
		int IsDead();

		//PowerUps
		void GrowBallSize();
		void ReducedBallSize();

	private:
		float m_Radius;
		float m_Outline;
		Color m_Color;
		Vector2 m_Position;
		bool m_IsActive;
		bool m_IsPaused;
		float m_Angle;
		Vector2 m_Velocity;
		float m_Speed;
		LineSegment m_ScreenEdges [3];

		//life system
		int m_Lives;
		SpriteFont m_TextLives;
	};
}