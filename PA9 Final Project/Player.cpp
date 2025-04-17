
// Player.cpp
#include "Player.hpp"
#include <iostream>

// Constructor - loads the doodle sprite and sets initial position
Player::Player() 
{
    if (!mTexture.loadFromFile("doodle.png")) 
    {
        std::cerr << "Error: Failed to load doodle.png\n";
    }

    // Set texture to the sprite (sf::Sprite parent class)
    this->setTexture(mTexture);

    // Set starting position in the window
    this->setPosition(400.f, 300.f);

    // Scale down the player sprite to make it smaller
    this->setScale(0.5f, 0.5f); // Feel free to Adjust scale as needed

    // Initial vertical velocity (gravity effect)
    mVelocityY = 0.f;
}

// Applies gravity and updates vertical movement
void Player::update(float deltaTime)
{
    mVelocityY += mGravity;          // Apply gravity
    this->move(0.f, mVelocityY);     // Move vertically
}

// Handles left/right movement using A and D keys
void Player::handleInput() 
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
    {
        this->move(-mSpeed, 0.f);    // Move left
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
    {
        this->move(mSpeed, 0.f);     // Move right
    }
}

// Makes the player jump by setting upward velocity
void Player::jump() 
{
    mVelocityY = mJumpSpeed;
}

// Resets the player's position and velocity (used for restart or fall)
void Player::resetPosition()
{
    this->setPosition(400.f, 300.f); // Reset position
    mVelocityY = 0.f;               // Reset vertical velocity
}

// Sets vertical velocity manually (used when landing on platform)
void Player::setVelocityY(float v) 
{
    mVelocityY = v;
}

// Returns current vertical velocity
float Player::getVelocityY() const 
{
    return mVelocityY;
}

// Returns bounding box for collision detection
sf::FloatRect Player::getBounds() const
{
    return this->getGlobalBounds();
}
