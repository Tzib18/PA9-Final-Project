
#pragma once
// do we want this class to also show the gameplay instructions?
// or do the instructions just go in the readme?
// should we try to save highscores? (possibly write to file, read from file, compare?)

#include <SFML/Graphics.hpp>

class Menu
{
public:
    Menu(sf::RenderWindow& window);
    void run(); // Show the menu and wait for selection
    bool isMuted() const;

private:
    sf::RenderWindow& mWindow;
    sf::Font mFont;
    sf::Texture mBackgroundTexture;
    sf::Sprite mBackground;
    sf::Text mTitleText;
    sf::Text mPlayText;
    sf::Text mMuteText;

    bool muted = false;
};
