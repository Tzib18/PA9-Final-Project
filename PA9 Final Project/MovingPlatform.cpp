
#include "MovingPlatform.hpp"

MovingPlatform::MovingPlatform(float x, float y, sf::Texture* texture)
    : Platform(x, y, texture)
{
}

void MovingPlatform::update(float deltaTime)
{
    float moveDistance = mSpeed * deltaTime;
    if (mDirectionRight)
        move(moveDistance, 0.f);
    else
        move(-moveDistance, 0.f);

    // Reverse direction if hitting edges
    if (getPosition().x < 0.f || getPosition().x + getGlobalBounds().width > 800.f)
    {
        mDirectionRight = !mDirectionRight;
    }
}
