#pragma once
#include <GameDev2D.h>

namespace GameDev2D
{
	class Tweening
	{
	public:
		Tweening(Vector2& reference);
		void OnUpdate(float delta);
		bool IsRunning();

		void Start(Vector2 target, float duration, Vector2 initialsize);
		void Cancel();

	private:
		Vector2& m_Reference;
		float m_Elapsed;
		float m_Duration;
		Vector2 m_Start;
		Vector2 m_Target;
		bool m_IsRunning;
	};
}


