
// SFML PA9 Doodle Jump
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include "Player.hpp"
#include "Platform.hpp"
#include "MovingPlatform.hpp"
#include "CrackedPlatform.hpp"
#include "Menu.hpp"
#include "BackgroundAudioTrack.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"

int main()
{
    // ***** WINDOW AND MUSIC SETUP *****
    sf::RenderWindow window(sf::VideoMode(800, 600), "Malloc Jump (Inspired by Doodle Jump) - PA9");
    window.setFramerateLimit(60);

    SoundTrack musicPlayer;
    musicPlayer.playMusic();

    Menu menu(window);
    menu.run();

    sf::View view = window.getDefaultView();

    // ***** BACKGROUND SETUP *****
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("paper_background.jpg")) return -1;
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

    // ***** PLATFORM TEXTURES AND INITIAL PLATFORMS *****
    sf::Texture platformTexture, crackedPlatformTexture, movingPlatformTexture, brokenPlatformTexture;
    if (!platformTexture.loadFromFile("platform.png") ||
        !crackedPlatformTexture.loadFromFile("cracked_block.png") ||
        !movingPlatformTexture.loadFromFile("moving_block.png") ||
        !brokenPlatformTexture.loadFromFile("broken_block.png")) return -1;

    std::vector<Platform> platforms = {  // Normal platforms
        Platform(100.f, 500.f, &platformTexture),
        Platform(300.f, 400.f, &platformTexture),
        Platform(150.f, 300.f, &platformTexture),
        Platform(350.f, 200.f, &platformTexture),
        Platform(200.f, 100.f, &platformTexture)
    };

    std::vector<MovingPlatform> movingPlatforms = 
    {  // Moving platforms
        MovingPlatform(250.f, 350.f, &movingPlatformTexture)
    };

    std::vector<CrackedPlatform> crackedPlatforms =
    {  // Cracked platforms
        CrackedPlatform(400.f, 250.f, &crackedPlatformTexture, &brokenPlatformTexture)
    };

    // ***** PLAYER SETUP *****
    Player player;

    // ***** ENEMY TEXTURES AND VECTORS *****
    sf::Texture purpleGlobTexture, EnemiesTexture;
    if (!purpleGlobTexture.loadFromFile("purple_glob.png")) return -1;
    if (!EnemiesTexture.loadFromFile("enemies.png")) return -1;
    std::vector<Enemy> Enemies1, Enemies2;

    // ***** BULLET TEXTURES AND VECTOR *****
    sf::Texture bulletTextureUp, bulletTextureLeft, bulletTextureRight;
    if (!bulletTextureUp.loadFromFile("Bullet.png") ||
        !bulletTextureLeft.loadFromFile("Left_Bullet.png") ||
        !bulletTextureRight.loadFromFile("Right_Bullet.png"))
        return -1;

    std::vector<Bullet> bullets;

    // ***** SOUND SETUP *****
    bool isMuted = false;
    sf::SoundBuffer deathBuffer, bulletBuffer, globDeathBuffer, enemy2DeathBuffer;
    sf::Sound deathSound, bulletSound, globDeathSound, enemy2DeathSound;
    if (!deathBuffer.loadFromFile("player_death_sound.wav") ||
        !bulletBuffer.loadFromFile("bullet_sound.wav") ||
        !globDeathBuffer.loadFromFile("purple_glob_death_sound_effect.wav") ||
        !enemy2DeathBuffer.loadFromFile("enemie2_death_sound.wav")) return -1;
    deathSound.setBuffer(deathBuffer);
    bulletSound.setBuffer(bulletBuffer);
    globDeathSound.setBuffer(globDeathBuffer);
    enemy2DeathSound.setBuffer(enemy2DeathBuffer);

    sf::Texture muteIconTexture;
    if (!muteIconTexture.loadFromFile("mute.png")) return -1;
    sf::Sprite muteIcon(muteIconTexture);
    muteIcon.setScale(0.05f, 0.05f);

    sf::Clock clock;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    float highestPlatformY = 100.f;

    // ***** TIMER AND HIGHSCORE SETUP *****
    sf::Clock gameClock;  // Tracks elapsed game time
    sf::Time elapsedTime;
    sf::Font font;
    if (!font.loadFromFile("OpenSans.ttf")) return -1;
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10.f, 10.f);

    sf::Time highscoreTime;  // Holds the highscore time read from file
    std::ifstream highscoreFile("highscore.txt");
    if (highscoreFile.is_open()) {
        int min, sec;
        char colon;
        highscoreFile >> min >> colon >> sec;
        highscoreTime = sf::seconds(min * 60 + sec);
        highscoreFile.close();
    }

    // ***** MAIN GAME LOOP *****
    while (window.isOpen())
    {
        // ***** HANDLE EVENTS (INPUT) *****
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                sf::Vector2f pos = player.getPosition();
                float playerCenterX = pos.x + player.getGlobalBounds().width / 2.f - 5.f;
                float playerTopY = pos.y;

                // Shoot UP
                if (event.key.code == sf::Keyboard::Up)
                {
                    bullets.emplace_back(&bulletTextureUp, playerCenterX, playerTopY, sf::Vector2f(0.f, -600.f));  // Upward velocity
                    bulletSound.play();
                    player.startShooting();
                }
                // Shoot LEFT
                else if (event.key.code == sf::Keyboard::Left)
                {
                    bullets.emplace_back(&bulletTextureLeft, playerCenterX, playerTopY, sf::Vector2f(-600.f, 0.f));  // Leftward velocity
                    bulletSound.play();
                    player.startShooting();
                }
                // Shoot RIGHT
                else if (event.key.code == sf::Keyboard::Right)
                {
                    bullets.emplace_back(&bulletTextureRight, playerCenterX, playerTopY, sf::Vector2f(600.f, 0.f));   // Rightward velocity
                    bulletSound.play();
                    player.startShooting();
                }
                // Toggle mute on M key press
                else if (event.key.code == sf::Keyboard::M)
                {
                    isMuted = !isMuted;
                    float vol = isMuted ? 0.f : 100.f;
                    deathSound.setVolume(vol);
                    bulletSound.setVolume(vol);
                    globDeathSound.setVolume(vol);
                    enemy2DeathSound.setVolume(vol);

                    if (isMuted)
                        musicPlayer.stopMusic();
                    else
                        musicPlayer.playMusic();

                    std::cout << (isMuted ? "Muted." : "Unmuted.") << std::endl;
                }
            }
        }

        // ***** UPDATE GAME STATE *****
        float deltaTime = clock.restart().asSeconds();
        elapsedTime = gameClock.getElapsedTime();

        player.handleInput();
        player.update(deltaTime);

        // ***** CHECK GAME OVER (PLAYER FALLS) *****
        if (player.getPosition().y > view.getCenter().y + 300.f)
        {
            // Save new highscore if beaten
            if (elapsedTime > highscoreTime) 
            {
                std::ofstream outFile("highscore.txt");
                if (outFile.is_open())
                {
                    int hsMin = static_cast<int>(elapsedTime.asSeconds()) / 60;
                    int hsSec = static_cast<int>(elapsedTime.asSeconds()) % 60;
                    outFile << hsMin << ":" << (hsSec < 10 ? "0" : "") << hsSec;
                    outFile.close();

                    std::cout << "New Highscore! Time: " << hsMin << ":"
                        << (hsSec < 10 ? "0" : "") << hsSec << std::endl;
                }
            }
            else
            {
                int hsMin = static_cast<int>(elapsedTime.asSeconds()) / 60;
                int hsSec = static_cast<int>(elapsedTime.asSeconds()) % 60;

                std::cout << "Score Time: " << hsMin << ":"
                    << (hsSec < 10 ? "0" : "") << hsSec << std::endl;
            }

            deathSound.play();
            std::cout << "Game Over! (fell off screen)" << std::endl;
            sf::sleep(sf::seconds(1.0f));
            window.close();
        }

        // ***** UPDATE CAMERA *****
        if (player.getPosition().y < view.getCenter().y - 100.f)
        {
            view.setCenter(view.getCenter().x, player.getPosition().y + 100.f);
            window.setView(view);
        }

        // ***** SPAWN NEW BACKGROUNDS *****
        float topOfView = view.getCenter().y - window.getSize().y / 2.f;
        while (topOfView - window.getSize().y < highestBG)
        {
            sf::Sprite newBG(bgTexture);
            newBG.setScale(bgScaleX, bgScaleY);
            highestBG -= window.getSize().y;
            newBG.setPosition(0.f, highestBG);
            backgrounds.push_back(newBG);
        }

        // ***** SPAWN NEW PLATFORMS AND ENEMIES *****
        while (highestPlatformY > view.getCenter().y - 300.f)
        {
            highestPlatformY -= static_cast<float>(rand() % 50 + 80);
            float centerX = view.getCenter().x;
            float x = static_cast<float>((rand() % 350) + (centerX - 180));

            // Randomly choose platform type
            int platformType = rand() % 10;
            if (platformType < 7)
            {
                platforms.emplace_back(x, highestPlatformY, &platformTexture);
            }
            else if (platformType < 9)
            {
                movingPlatforms.emplace_back(x, highestPlatformY, &movingPlatformTexture);
            }
            else
            {
                crackedPlatforms.emplace_back(x, highestPlatformY, &crackedPlatformTexture, &brokenPlatformTexture);
            }

            // Random enemy spawn
            if ((rand() % 5) == 0)
            {
                if ((rand() % 2) == 0)
                {
                    Enemies1.emplace_back(&purpleGlobTexture, x + 10.f, highestPlatformY - 40.f);
                    Enemies1.back().setScale(0.15f, 0.15f);
                }
                else {
                    Enemies2.emplace_back(&EnemiesTexture, x + 10.f, highestPlatformY - 40.f);
                    Enemies2.back().setScale(0.5f, 0.5f);
                }
            }
        }

        // ***** HANDLE PLATFORM COLLISIONS *****
        for (auto& mp : movingPlatforms)
        {
            mp.update(deltaTime);
            if (player.getGlobalBounds().intersects(mp.getBounds()))
            {
                float playerBottom = player.getPosition().y + player.getGlobalBounds().height;
                float platformTop = mp.getPosition().y;
                if (player.getVelocityY() > 0 && playerBottom < platformTop + 10.f)
                    player.jump();
            }
        }

        for (auto& platform : platforms)
        {
            if (player.getGlobalBounds().intersects(platform.getBounds()))
            {
                float playerBottom = player.getPosition().y + player.getGlobalBounds().height;
                float platformTop = platform.getPosition().y;
                if (player.getVelocityY() > 0 && playerBottom < platformTop + 20.f)
                    player.jump();
            }
        }

        for (auto& cp : crackedPlatforms)
        {
            if (!cp.isBroken() && player.getGlobalBounds().intersects(cp.getBounds()))
            {
                float playerBottom = player.getPosition().y + player.getGlobalBounds().height;
                float platformTop = cp.getPosition().y;
                if (player.getVelocityY() > 0 && playerBottom < platformTop + 10.f)
                {
                    player.jump();
                    cp.breakPlatform();
                }
            }
        }

        // ***** UPDATE BULLETS *****
        for (auto& bullet : bullets) bullet.update(deltaTime);

        float screenTop = view.getCenter().y - window.getSize().y / 2.f;
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [screenTop](const Bullet& b) { return b.isOffScreen(screenTop); }), bullets.end());

        // ***** PLAYER COLLIDES WITH ENEMIES *****
        for (auto& enemy : Enemies1)
        {
            if (player.getGlobalBounds().intersects(enemy.getGlobalBounds()))
            {
                // Save highscore if beaten
                if (elapsedTime > highscoreTime) {
                    std::ofstream outFile("highscore.txt");
                    if (outFile.is_open()) {
                        int hsMin = static_cast<int>(elapsedTime.asSeconds()) / 60;
                        int hsSec = static_cast<int>(elapsedTime.asSeconds()) % 60;
                        outFile << hsMin << ":" << (hsSec < 10 ? "0" : "") << hsSec;
                        outFile.close();
                    }
                }
                deathSound.play();
                std::cout << "Game Over! (hit enemy)" << std::endl;
                sf::sleep(sf::seconds(1.0f));
                window.close();
            }
        }
        for (auto& enemy : Enemies2)
        {
            if (player.getGlobalBounds().intersects(enemy.getGlobalBounds()))
            {
                if (elapsedTime > highscoreTime) {
                    std::ofstream outFile("highscore.txt");
                    if (outFile.is_open()) {
                        int hsMin = static_cast<int>(elapsedTime.asSeconds()) / 60;
                        int hsSec = static_cast<int>(elapsedTime.asSeconds()) % 60;
                        outFile << hsMin << ":" << (hsSec < 10 ? "0" : "") << hsSec;
                        outFile.close();
                    }
                }
                deathSound.play();
                std::cout << "Game Over! (hit enemy)" << std::endl;
                sf::sleep(sf::seconds(1.0f));
                window.close();
            }
        }

        // ***** BULLETS COLLIDE WITH ENEMIES *****
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); )
        {
            bool hit = false;

            for (auto enemyIt = Enemies1.begin(); enemyIt != Enemies1.end(); )
            {
                if (bulletIt->getGlobalBounds().intersects(enemyIt->getGlobalBounds()))
                {
                    globDeathSound.play();
                    enemyIt = Enemies1.erase(enemyIt);
                    hit = true;
                    break;
                }
                else ++enemyIt;
            }

            if (!hit)
            {
                for (auto enemyIt = Enemies2.begin(); enemyIt != Enemies2.end(); )
                {
                    if (bulletIt->getGlobalBounds().intersects(enemyIt->getGlobalBounds()))
                    {
                        enemy2DeathSound.play();
                        enemyIt = Enemies2.erase(enemyIt);
                        hit = true;
                        break;
                    }
                    else ++enemyIt;
                }
            }

            if (hit)
                bulletIt = bullets.erase(bulletIt);
            else
                ++bulletIt;
        }

        // ***** UPDATE SCORE DISPLAY *****
        int minutes = static_cast<int>(elapsedTime.asSeconds()) / 60;
        int seconds = static_cast<int>(elapsedTime.asSeconds()) % 60;
        scoreText.setString("Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));

        // *****RENDER EVERYTHING *****
            window.clear();
        for (auto& bg : backgrounds) window.draw(bg);
        for (auto& platform : platforms) window.draw(platform);
        for (auto& mp : movingPlatforms) window.draw(mp);
        for (auto& cp : crackedPlatforms) window.draw(cp);
        for (auto& bullet : bullets) window.draw(bullet);
        for (auto& enemy : Enemies1) window.draw(enemy);
        for (auto& enemy : Enemies2) window.draw(enemy);
        window.draw(player);

        // ***** DRAW HUD (STATIC ITEMS) *****
        window.setView(window.getDefaultView());  // Reset to default view (locks HUD)

        window.draw(scoreText);  // Draw the time (score)
        if (isMuted) window.draw(muteIcon);  // Draw mute icon

        window.setView(view);  // camera follows player

        window.display();
    }

    return 0;
}
