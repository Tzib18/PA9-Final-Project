
#pragma once
// #include "Player.hpp"

//class Enemy : public Player 
//{
//public:
//    Enemy(sf::Texture* texture, float x, float y);
//    virtual void update(float deltaTime) override;
//
//private:
//    sf::Texture* mTexture; 
//};

#pragma once
#include <SFML/Graphics.hpp>

class Enemy : public sf::Sprite {
public:
    Enemy(sf::Texture* texture, float x, float y);
    void update(float deltaTime);

private:
    sf::Texture* mTexture; // Store pointer to texture
};
