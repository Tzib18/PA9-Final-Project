
#include "CrackedPlatform.hpp"

CrackedPlatform::CrackedPlatform(float x, float y, sf::Texture* normalTexture, sf::Texture* brokenTexture)
    : Platform(x, y, normalTexture), mBrokenTexture(brokenTexture)
{
}

bool CrackedPlatform::isBroken() const
{
    return mBroken;
}

void CrackedPlatform::breakPlatform()
{
    if (!mBroken)
    {
        setTexture(*mBrokenTexture);
        mBroken = true;
    }
}
