

#include "Platform.hpp"
#include <iostream>

// Sets platform sprite using the shared texture and sets position
Platform::Platform(float x, float y, sf::Texture* texture)
{
    mTexture = texture;
    this->setTexture(*mTexture);
    this->setPosition(x, y);
}

// Used in player collision checks
sf::FloatRect Platform::getBounds() const
{
    return this->getGlobalBounds();
}
