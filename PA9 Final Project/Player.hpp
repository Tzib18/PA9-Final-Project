

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
   virtual void update(float deltaTime);

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

    // ***** Shooting animation control *****
    void startShooting();  // Call this when shooting to switch texture

private:
    // ***** Textures *****
    sf::Texture mNormalTexture;      // Normal player texture
    sf::Texture mShootingTexture;    // Shooting player texture 

    // Movement variables
    float mVelocityY;          // Vertical speed (gravity or jump)
    const float mGravity = 0.5f;      // Constant downward force
    const float mJumpSpeed = -13.5f;   // Speed when jumping up
    const float mSpeed = 5.6f;         // Side movement speed

    // ***** Shooting state *****
    sf::Clock mShootTimer;   // Timer for how long the shooting texture shows
    bool mIsShooting = false;
};
