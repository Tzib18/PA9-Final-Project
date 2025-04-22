
#pragma once
#include "Platform.hpp"

class CrackedPlatform : public Platform
{
public:
    CrackedPlatform(float x, float y, sf::Texture* normalTexture, sf::Texture* brokenTexture);

    bool isBroken() const;
    void breakPlatform();

private:
    bool mBroken = false;
    sf::Texture* mBrokenTexture;
};
