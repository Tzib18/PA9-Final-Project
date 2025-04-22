
#pragma once
#include <SFML/Graphics.hpp>

class Bullet : public sf::Sprite
{
public:
    Bullet(sf::Texture* texture, float x, float y, sf::Vector2f velocity);  // Velocity stays here
    void update(float deltaTime);
    bool isOffScreen(float screenTop) const;

private:
    sf::Vector2f mVelocity;  // Holds both speed and direction
};