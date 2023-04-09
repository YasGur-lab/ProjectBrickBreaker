#include "Tweening.h"

GameDev2D::Tweening::Tweening(Vector2& reference)
	: m_Reference(reference)
	, m_Elapsed(0.0f)
	, m_Duration(0.0f)
	, m_Start(0,0)
	, m_Target(0,0)
	, m_IsRunning(false)
{
}

void GameDev2D::Tweening::OnUpdate(float delta)
{
	if (m_IsRunning)
	{
		if (m_Elapsed < m_Duration)
		{
			m_Elapsed += delta;
			if (m_Elapsed > m_Duration)
			{
				m_Elapsed = m_Duration;
				m_IsRunning = false;
			}
			float pct = m_Elapsed / m_Duration;
			m_Reference = m_Start + (m_Target - m_Start) * pct;
		}
	}
}

bool GameDev2D::Tweening::IsRunning()
{
	return m_IsRunning;
}

void GameDev2D::Tweening::Start(Vector2 target, float duration, Vector2 initialsize)
{
	Cancel();
	m_Reference = initialsize;
	m_Duration = duration;
	m_Start = m_Reference;
	m_Target = target;
	m_IsRunning = true;
}

void GameDev2D::Tweening::Cancel()
{
	m_Elapsed = 0.0f;
	m_Duration = 0.0f;
	m_Start = Vector2(0,0);
	m_Target = Vector2(0, 0);
	m_IsRunning = false;
}
