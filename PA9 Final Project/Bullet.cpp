
#include "Bullet.hpp"

Bullet::Bullet(sf::Texture* texture, float x, float y)
{
    setTexture(*texture);
    setScale(0.75f, 0.75f); // adjusts bullet size
    setPosition(x, y);
}

void Bullet::update(float deltaTime) 
{
    move(0.f, -mSpeed * deltaTime); // move straight up
}

bool Bullet::isOffScreen(float screenTop) const 
{
    return getPosition().y + getGlobalBounds().height < screenTop;
}
