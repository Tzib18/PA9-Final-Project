
#include "Enemy.hpp"

//Enemy::Enemy(sf::Texture* texture, float x, float y)
//{
//    mTexture = texture; 
//
//    this->setTexture(*mTexture);
//    this->setScale(0.99f, 0.99f);
//    this->setPosition(x, y);
//}
//
//void Enemy::update(float deltaTime)
//{
//    // add movement later if needed
//}

#include "Enemy.hpp"

Enemy::Enemy(sf::Texture* texture, float x, float y) 
{
    mTexture = texture;  
    setTexture(*mTexture);
    setScale(0.15f, 0.15f);  // Resize for visibility
    setPosition(x, y);
}

void Enemy::update(float deltaTime) 
{
    // Placeholder for future movement
}
