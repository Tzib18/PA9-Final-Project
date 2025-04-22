
#include "Bullet.hpp"

Bullet::Bullet(sf::Texture* texture, float x, float y, sf::Vector2f velocity)
{
    setTexture(*texture);
    setPosition(x, y);
    mVelocity = velocity;  // Just store the velocity vector
}

void Bullet::update(float deltaTime)
{
    move(mVelocity * deltaTime);  // Apply velocity * time
}

bool Bullet::isOffScreen(float screenTop) const
{
    return getPosition().y + getGlobalBounds().height < screenTop ||
        getPosition().x + getGlobalBounds().width < 0.f ||  // Off left
        getPosition().x > 800.f;                           // Off right
}

