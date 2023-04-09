#pragma once

#include <GameDev2D.h>
#include "Paddle.h"

using namespace GameDev2D;

class Bullet
{
private:
    static const float MAX_VELOCITY;

public:
       Bullet(Paddle* paddle);
    ~Bullet();

    // Write game logic in the Update method.
    void OnUpdate(float deltaTime);

    // Render game objects in the Draw method.
    void OnRender(BatchRenderer& batchRenderer);

    void OnKeyEvent(KeyCode keyCode, KeyState keyState);

    bool IsActive();
    void Activate(Vector2 position);
    void Deactivate();
    void SetVelocity(Vector2 Velocity);
    Vector2 GetBulletPosition();
    float GetBulletRadius();
    void SetPosition(Vector2 position);

protected:
    Paddle* m_Paddle;
    bool m_Active;
    float m_Radius;
    Vector2 m_Position;

private:
    Vector2 m_Velocity;
    float accelerationRate;
    float m_Angle;
    Vector2 m_Speed;
    float m_TimeAlive;
};