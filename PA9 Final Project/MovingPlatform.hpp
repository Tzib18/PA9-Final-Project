
#pragma once
#include "Platform.hpp"

class MovingPlatform : public Platform
{
public:
    MovingPlatform(float x, float y, sf::Texture* texture);

    void update(float deltaTime);

private:
    bool mDirectionRight = true;
    float mSpeed = 50.f;
};
