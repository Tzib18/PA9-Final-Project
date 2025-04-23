
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
    mTitleText.setString("Malloc Jump!");
    mTitleText.setCharacterSize(48);
    mTitleText.setFillColor(sf::Color::Black); // changed color to black
    mTitleText.setPosition(50.f, 100.f);

    /***** Play Text *****/
    mPlayText.setFont(mFont);
    mPlayText.setString("Press SPACE to Play");
    mPlayText.setCharacterSize(24);
    mPlayText.setFillColor(sf::Color::White);
    mPlayText.setPosition(50.f, 260.f);

    /***** How To Play Text *****/
    mHowToPlayText.setFont(mFont);
    mHowToPlayText.setString("Press H for How to Play");
    mHowToPlayText.setCharacterSize(24);
    mHowToPlayText.setFillColor(sf::Color::White);
    mHowToPlayText.setPosition(50.f, 300.f);
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
                    return;  // Start game
                if (event.key.code == sf::Keyboard::M)
                    muted = !muted;  // Toggle mute
                if (event.key.code == sf::Keyboard::H)
                    drawHowToPlay();  // Show instructions
            }
        }

        // Draw the menu
        mWindow.clear();
        mWindow.draw(mBackground);
        mWindow.draw(mTitleText);
        mWindow.draw(mPlayText);
        mWindow.draw(mMuteText);
        mWindow.draw(mHowToPlayText);
        mWindow.display();
    }
}

void Menu::drawHowToPlay()
{
    sf::Text instructions;
    instructions.setFont(mFont);
    instructions.setCharacterSize(24);
    instructions.setFillColor(sf::Color::White);
    instructions.setPosition(50.f, 50.f);
    instructions.setString(
        "How to Play:\n\n" 
        "CONTROLS:\n"
        "Move Left: A\n"
        "Move Right: D\n"
        "Shoot Up: Up Arrow\n"
        "Shoot Left: Left Arrow\n"
        "Shoot Right: Right Arrow\n"
        "Mute: M\n"
        "Pause: esc\n\n"
        "GOAL:\n"
        "Try to avoid enemies and falling.\n"
        "Shoot enemies to survive longer!\n\n"
        "Press Enter to return to the Menu."
    );

    while (mWindow.isOpen())
    {
        sf::Event event;
        while (mWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                mWindow.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                return;  // Exit How to Play screen
        }

        mWindow.clear();
        mWindow.draw(mBackground);  // Keep the background
        mWindow.draw(instructions);
        mWindow.display();
    }
}

bool Menu::isMuted() const
{
    return muted;
}

