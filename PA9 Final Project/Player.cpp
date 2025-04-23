
#include "Player.hpp"
#include <iostream>

// Constructor - loads the doodle sprite and sets initial position
Player::Player()
{
    // Load normal texture
    if (!mNormalTexture.loadFromFile("main_character.png"))
    {
        std::cerr << "Error: Failed to load main_character.png" << std::endl;
    }

    // Load shooting texture
    if (!mShootingTexture.loadFromFile("main_character_shooting.png"))
    {
        std::cerr << "Error: Failed to load main_character_shooting.png" << std::endl;
    }

    // Set normal texture initially
    this->setTexture(mNormalTexture);

    // Starting position
    this->setPosition(400.f, 300.f);

    // Scale the player sprite
    this->setScale(1.0f, 1.0f);

    // Initial vertical velocity
    mVelocityY = 0.f;
}

// Applies gravity, updates vertical movement and handles texture switch
void Player::update(float deltaTime)
{
    // Apply gravity
    mVelocityY += mGravity;
    this->move(0.f, mVelocityY);

    // ***** Handle texture switching back to normal *****
    if (mIsShooting && mShootTimer.getElapsedTime().asSeconds() > 0.2f)  // Show shooting texture for 0.2 seconds
    {
        this->setTexture(mNormalTexture);
        mIsShooting = false;
    }
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

void Player::startShooting()
{
    this->setTexture(mShootingTexture);  // Switch to shooting texture
    mIsShooting = true;
    mShootTimer.restart();  // Start timer to track how long shooting texture is shown
}
