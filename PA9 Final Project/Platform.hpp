
#pragma once

#include <SFML/Graphics.hpp>

// Platform class - player bounces when falling onto it
class Platform : public sf::Sprite
{
public:
    // Constructor takes position and shared texture pointer
    Platform(float x, float y, sf::Texture* texture);

    // Returns bounding box for collision checks
    sf::FloatRect getBounds() const;

private:
    sf::Texture* mTexture; // Shared texture pointer (does not own memory)
};



