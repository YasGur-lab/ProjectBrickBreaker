#pragma once
#include <GameDev2D.h>
#include <vector>
#include "Tweening.h"

namespace GameDev2D
{
	class Brick 
	{
	private:

	public:
		Brick();
		~Brick();

		// Write game logic in the Update method.
		void OnUpdate(float delta);

		// Render game objects in the Draw method.
		void OnRender(BatchRenderer& batchRenderer);

		void OnKeyEvent(KeyCode keyCode, KeyState keyState);
		
		void Deactivate();
		void Activate();
		bool IsActive();

		void SetBrickPosition(Vector2 position);
		Vector2 GetBrickPosition();
		Vector2 GetSize();

		// life methods		
		void MinusLife();
		float Lives();
		void SetBrickLives(float lives);

	private:
		Vector2 m_Position;
		Vector2 m_Size;
		Color m_Color1;
		Color m_Color2;
		Color m_Color3;
		Color m_Color4;
		bool m_IsActive;
		float m_Lives;
		Tweening m_SizeTween;
		float timer;
	};
}