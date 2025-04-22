
#pragma once
#include <SFML/Graphics.hpp>

class Bullet : public sf::Sprite
{
public:
    Bullet(sf::Texture* texture, float x, float y);
    void update(float deltaTime);
    bool isOffScreen(float screenTop) const;

private:
    float mSpeed = 500.f; // bullet movement speed
};

