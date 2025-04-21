

#include "Platform.hpp"
#include <iostream>

// Sets platform sprite using the shared texture and sets position
Platform::Platform(float x, float y, sf::Texture* texture)
{
    mTexture = texture;
    this->setTexture(*mTexture);
    // Scale it down — try these values first
    this->setScale(0.32f, 0.45f); // Shrinks width and height
    this->setPosition(x, y);
}

// Used in player collision checks
sf::FloatRect Platform::getBounds() const
{
    return this->getGlobalBounds();
}





