
#include "Enemy.hpp"

Enemy::Enemy(sf::Texture* texture, float x, float y)
{
    mTexture = texture;
    setTexture(*mTexture);
    setScale(0.15f, 0.15f);
    setPosition(x, y);

    mSpeed = 50.f;        // pixels per second
    mMovingRight = (rand() % 2 == 0);  // random start direction
}

void Enemy::update(float deltaTime) 
{
    float dx = mSpeed * deltaTime;

    if (mMovingRight)
        move(dx, 0.f);
    else
        move(-dx, 0.f);

    // Reverse direction at window bounds (wrap around view width)
    float x = getPosition().x;
    float width = getGlobalBounds().width;

    if (x < 0.f) 
    {
        setPosition(0.f, getPosition().y);
        mMovingRight = true;
    }
    else if (x + width > 800.f)
    {
        setPosition(800.f - width, getPosition().y);
        mMovingRight = false;
    }
}

