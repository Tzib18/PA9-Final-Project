
#pragma once
#include <SFML/Graphics.hpp>

class Enemy : public sf::Sprite 
{
public:
    Enemy(sf::Texture* texture, float x, float y);
    void update(float deltaTime);

private:
    sf::Texture* mTexture;
    float mSpeed;     // horizontal movement speed
    bool mMovingRight; // direction flag
};

