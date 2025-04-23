
#pragma once
#include <SFML/Graphics.hpp>

class Menu
{
public:
    Menu(sf::RenderWindow& window);
    void run();  // Show the menu and wait for selection
    bool isMuted() const;

private:
    sf::RenderWindow& mWindow;
    sf::Font mFont;
    sf::Texture mBackgroundTexture;
    sf::Sprite mBackground;
    sf::Text mTitleText;
    sf::Text mPlayText;
    sf::Text mMuteText;
    sf::Text mHowToPlayText;  

    bool muted = false;

    void drawHowToPlay();  
};
