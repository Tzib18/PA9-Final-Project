
// SFML PA9 Doodle Jump
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Player.hpp"
#include "Platform.hpp"
#include "Menu.hpp" 

int main()
{
    /***** Window Setup *****/
    sf::RenderWindow window(sf::VideoMode(800, 600), "Doodle Jump - PA9");
    window.setFramerateLimit(60);

    /***** Run the Menu First *****/
    Menu menu(window);
    menu.run(); // Wait for spacebar to continue

    /***** View (Camera) *****/
    sf::View view = window.getDefaultView();

    /***** Background Setup *****/
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("paper_background.jpg"))
    {
        std::cerr << "Failed to load paper_background.jpg" << std::endl;
        return -1;
    }

    sf::Vector2u bgSize = bgTexture.getSize();
    float bgScaleX = static_cast<float>(window.getSize().x) / bgSize.x;
    float bgScaleY = static_cast<float>(window.getSize().y) / bgSize.y;

    std::vector<sf::Sprite> backgrounds;
    for (int y = 0; y <= 1800; y += window.getSize().y)
    {
        sf::Sprite bg(bgTexture);
        bg.setScale(bgScaleX, bgScaleY);
        bg.setPosition(0.f, -static_cast<float>(y));
        backgrounds.push_back(bg);
    }

    float highestBG = backgrounds.front().getPosition().y;

    /***** Platform Setup *****/
    sf::Texture platformTexture;
    if (!platformTexture.loadFromFile("platform.png"))
    {
        std::cerr << "Failed to load platform.png" << std::endl;
        return -1;
    }

    std::vector<Platform> platforms;
    platforms.emplace_back(100.f, 500.f, &platformTexture);
    platforms.emplace_back(300.f, 400.f, &platformTexture);
    platforms.emplace_back(150.f, 300.f, &platformTexture);
    platforms.emplace_back(350.f, 200.f, &platformTexture);
    platforms.emplace_back(200.f, 100.f, &platformTexture);

    float highestPlatformY = 100.f;

    /***** Player Setup *****/
    Player player;

    /***** Time and Random Seed *****/
    sf::Clock clock;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    /***** Game Loop *****/
    while (window.isOpen())
    {
        /***** Event Polling *****/
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        /***** Delta Time *****/
        float deltaTime = clock.restart().asSeconds();

        /***** Player Update *****/
        player.handleInput();
        player.update(deltaTime);

        /***** Horizontal Wraparound with Height Locking *****/
        sf::Vector2f playerPos = player.getPosition();
        float playerWidth = player.getGlobalBounds().width;
        float yLock = playerPos.y; // Save current height before teleport

        if (playerPos.x > view.getCenter().x + 400.f)
        {
            player.setPosition(view.getCenter().x - 400.f - playerWidth, yLock);
        }
        if (playerPos.x + playerWidth < view.getCenter().x - 400.f)
        {
            player.setPosition(view.getCenter().x + 400.f, yLock);
        }

        /***** Platform Collision *****/
        for (auto& platform : platforms)
        {
            if (player.getBounds().intersects(platform.getBounds()))
            {
                float playerBottom = player.getPosition().y + player.getGlobalBounds().height;
                float platformTop = platform.getPosition().y;
                if (player.getVelocityY() > 0 && playerBottom < platformTop + 10.f)
                {
                    player.jump();
                }
            }
        }

        /***** Camera Follows Player Upward *****/
        if (player.getPosition().y < view.getCenter().y - 100.f)
        {
            view.setCenter(view.getCenter().x, player.getPosition().y + 100.f);
            window.setView(view);
        }

        /***** Game Over: Fell Below Screen *****/
        if (player.getPosition().y > view.getCenter().y + 300.f)
        {
            std::cout << "Game Over!" << std::endl;
            window.close();
        }

        /***** Add Background Tiles Before Needed *****/
        float topOfView = view.getCenter().y - window.getSize().y / 2.f;

        while (topOfView - window.getSize().y < highestBG)
        {
            sf::Sprite newBG(bgTexture);
            newBG.setScale(bgScaleX, bgScaleY);
            highestBG -= window.getSize().y;
            newBG.setPosition(0.f, highestBG);
            backgrounds.push_back(newBG);
        }

        /***** Remove Backgrounds Far Below Screen *****/
        backgrounds.erase(std::remove_if(backgrounds.begin(), backgrounds.end(), [&view, &window](const sf::Sprite& bg)
            {
                float bgTop = bg.getPosition().y;
                float viewBottom = view.getCenter().y + window.getSize().y / 2.f;
                return bgTop > viewBottom + window.getSize().y;
            }),
            backgrounds.end()
        );

        /***** Generate New Platforms Above View (Safer Spacing) *****/
        while (highestPlatformY > view.getCenter().y - 300.f)
        {
            highestPlatformY -= static_cast<float>(rand() % 50 + 80);
            float centerX = view.getCenter().x;
            float x = static_cast<float>((rand() % 400) + (centerX - 200));
            platforms.emplace_back(x, highestPlatformY, &platformTexture);
        }

        /***** Remove Platforms Far Below Screen *****/
        float screenBottom = view.getCenter().y + static_cast<float>(window.getSize().y) / 2.f;

        platforms.erase(std::remove_if(platforms.begin(), platforms.end(), [&](const Platform& p)
            {
                return p.getPosition().y > screenBottom + 100.f; // give extra room
            }), platforms.end());


        /***** Drawing Everything *****/
        window.clear();

        for (auto& bg : backgrounds)
        {
            window.draw(bg);
        }

        for (auto& platform : platforms)
        {
            window.draw(platform);
        }

        window.draw(player);
        window.display();
    }

    return 0;
}

