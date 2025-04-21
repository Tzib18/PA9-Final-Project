
#include "Menu.hpp"
#include <iostream>

Menu::Menu(sf::RenderWindow& window) : mWindow(window)
{
    if (!mFont.loadFromFile("OpenSans.ttf")) 
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    if (!mBackgroundTexture.loadFromFile("menu_background.jpg")) 
    {
        std::cerr << "Failed to load menu background!" << std::endl;
    }

    mBackground.setTexture(mBackgroundTexture);
    mBackground.setScale(
        static_cast<float>(mWindow.getSize().x) / mBackgroundTexture.getSize().x,
        static_cast<float>(mWindow.getSize().y) / mBackgroundTexture.getSize().y
    );

    /***** Title Text *****/
    mTitleText.setFont(mFont);
    mTitleText.setString("Doodle Jump");
    mTitleText.setCharacterSize(48);
    mTitleText.setFillColor(sf::Color::Red);
    mTitleText.setPosition(50.f, 100.f); // Left side

    /***** Play Text *****/
    mPlayText.setFont(mFont);
    mPlayText.setString("Press SPACE to Play");
    mPlayText.setCharacterSize(24);
    mPlayText.setFillColor(sf::Color::White);
    mPlayText.setPosition(50.f, 200.f); // Left side

    /***** Mute Text *****/
    mMuteText.setFont(mFont);
    mMuteText.setString("Press M to Toggle Mute");
    mMuteText.setCharacterSize(24);
    mMuteText.setFillColor(sf::Color::White);
    mMuteText.setPosition(50.f, 260.f); // Left side
}

void Menu::run() 
{
    while (mWindow.isOpen())
    {
        sf::Event event;
        while (mWindow.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                mWindow.close();

            if (event.type == sf::Event::KeyPressed) 
            {
                if (event.key.code == sf::Keyboard::Space)
                    return;
                if (event.key.code == sf::Keyboard::M)
                    muted = !muted;
            }
        }

        mWindow.clear();
        mWindow.draw(mBackground);
        mWindow.draw(mTitleText);
        mWindow.draw(mPlayText);
        mWindow.draw(mMuteText);
        mWindow.display();
    }
}

bool Menu::isMuted() const 
{
    return muted;
}
