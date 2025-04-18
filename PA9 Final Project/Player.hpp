

#pragma once

#include <SFML/Graphics.hpp>

// Player class - represents the doodle character.
// Inherits from sf::Sprite for easy rendering and movement.
class Player : public sf::Sprite 
{
public:
    // Constructor - loads texture and sets initial values
    Player();

    // Applies gravity and updates vertical position
    void update(float deltaTime);

    // Handles A/D movement input (left/right)
    void handleInput();

    // Causes the player to jump (when hitting platform)
    void jump();

    // Reset player to start position (used on death/restart)
    void resetPosition();

    // Velocity controls
    void setVelocityY(float v);
    float getVelocityY() const;

    // Used for collision detection
    sf::FloatRect getBounds() const;

private:
    sf::Texture mTexture;      // Image texture of the doodle character
    float mVelocityY;          // Vertical speed (gravity or jump)


    const float mGravity = 0.5f;      // Constant downward force
    const float mJumpSpeed = -13.5f;   // Speed when jumping up
    const float mSpeed = 4.5f;         // Side movement speed
};
