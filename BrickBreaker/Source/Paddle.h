#pragma once
#include <GameDev2D.h>

namespace GameDev2D
{
	class Paddle
	{
	private:
		static const float MAX_VELOCITY;

	public:
		Paddle();
		~Paddle();

		// Write game logic in the Update method.
		void OnUpdate(float delta);

		// Render game objects in the Draw method.
		void OnRender(BatchRenderer& batchRenderer);

		void OnKeyEvent(KeyCode keyCode, KeyState keyState);

		Vector2 GetPaddlePosition();
		Vector2 GetPaddleSize();
		Vector2 GetPaddleSpeed();
		bool IsActive();

		//PowerUps
		void GrowPaddleSize();
		void ReducedPaddleSize();

		//Reset if ball of edge
		void ResetSize();

	private:
		Vector2 m_Position;
		Vector2 m_Size;
		Vector2 m_Velocity;
		Color m_Color;
		bool m_IsActive;

		//movements
		bool m_MovingLeft;
		bool m_MovingRight;
	};
}