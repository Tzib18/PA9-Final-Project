
// SFML PA9 Malloc Jump (Inspired by Doodle Jump)
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
#include "Player score.hpp"

int main()
{
    // ***** WINDOW AND MUSIC SETUP *****
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Malloc Jump (Inspired by Doodle Jump) - PA9");
    window.setFramerateLimit(60);

    SoundTrack musicPlayer;
    musicPlayer.playMusic();

    Menu menu(window);
    menu.run(); // Show menu before game starts

    sf::View view = window.getDefaultView(); // Camera view

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
    float startingPlatformY = highestPlatformY;  // Save the starting platform height to use for heightScore calculations later


    // ***** TIMER, SCORE, AND HIGHSCORE SETUP *****
    int killScore = 0;  // Tracks score from enemies killed
    float maxHeight = player.getPosition().y;  // Track highest Y position (lower Y means higher in SFML)


    sf::Clock gameClock;  // Tracks elapsed game time
    sf::Time elapsedTime;

    sf::Font font;
    if (!font.loadFromFile("OpenSans.ttf")) return -1;

    // ***** Timer Text Setup *****
    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::Black);
    timerText.setPosition(10.f, 10.f);  // Top-left

    // ***** Score Text Setup *****
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10.f, 40.f);  // Just below the timer

    // ***** Highscore Text Setup *****
    sf::Text highscoreText;
    highscoreText.setFont(font);
    highscoreText.setCharacterSize(24);
    highscoreText.setFillColor(sf::Color::Black);
    highscoreText.setPosition(10.f, 70.f);  // Below the score

    // ***** Highscore Setup *****
    int highscore = 0;  // Default highscore
    std::ifstream highscoreFile("highscore.txt");
    if (highscoreFile.is_open()) 
    {
        highscoreFile >> highscore;
        highscoreFile.close();
    }

    /******Pause Menu Setup*****/
    bool pause = false; // initial pause position is false
    sf::Text pausedText("PAUSED", font, 50);
    pausedText.setFillColor(sf::Color::Blue);
    pausedText.setStyle(sf::Text::Bold);
    pausedText.setPosition(view.getCenter().x - 100.f, view.getCenter().y - 50.f);

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
                // Pause game when ESC is pressed
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    pause = !pause; // reverse pause position
                    pause ? std::cout << "Paused" << std::endl : std::cout << "Resumed" << std::endl;
                    pausedText.setPosition(view.getCenter().x - 100.f, view.getCenter().y - 50.f);
                }
            }
        }

        /******* player score *******/

        int heightScore = static_cast<int>(startingPlatformY - maxHeight);  // How much higher player climbed from the first platform
        int totalScore = killScore + heightScore;  // Total score = kills + height

        if (!pause) {
            float deltaTime = clock.restart().asSeconds();
            elapsedTime = gameClock.getElapsedTime();

        player.handleInput();
        player.update(deltaTime);

        if (player.getPosition().y < maxHeight)  // Remember: lower y = higher in SFML
            maxHeight = player.getPosition().y;

        // ***** CHECK GAME OVER (PLAYER FALLS) *****
        if (player.getPosition().y > view.getCenter().y + 300.f)
        {
            // Save new highscore if beaten
            if (totalScore > highscore)
            {
                std::ofstream outFile("highscore.txt");
                if (outFile.is_open())
                {
                    outFile << totalScore;  // Save the new highscore
                    outFile.close();
                }
                highscore = totalScore;  // <-- Update the displayed highscore too
                std::cout << "New Highscore! Score: " << totalScore << std::endl;
            }

            else
            {
                std::cout << "Score: " << totalScore << std::endl;
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
                    if (totalScore > highscore)
                    {
                        std::ofstream outFile("highscore.txt");
                        if (outFile.is_open())
                        {
                            outFile << totalScore;  // Save new highscore
                            outFile.close();
                        }
                        highscore = totalScore;  // Update in-memory highscore
                        std::cout << "New Highscore! Score: " << totalScore << std::endl;
                    }
                    else
                    {
                        std::cout << "Score: " << totalScore << std::endl;
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
                    if (totalScore > highscore)
                    {
                        std::ofstream outFile("highscore.txt");
                        if (outFile.is_open())
                        {
                            outFile << totalScore;  // Save new highscore
                            outFile.close();
                        }
                        highscore = totalScore;  // Update in-memory highscore
                        std::cout << "New Highscore! Score: " << totalScore << std::endl;
                    }
                    else
                    {
                        std::cout << "Score: " << totalScore << std::endl;
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
                        killScore += 20; // adds 20 points for each Glob kill
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
                            killScore += 15; // adds 15 points for each enemy2  kill
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
        }
        

        // ***** UPDATE SCORE DISPLAY *****
        int minutes = static_cast<int>(elapsedTime.asSeconds()) / 60;
        int seconds = static_cast<int>(elapsedTime.asSeconds()) % 60;
        // Update HUD (timer and score)
        timerText.setString("Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
        scoreText.setString("Score: " + std::to_string(totalScore));
        highscoreText.setString("Highscore: " + std::to_string(highscore));


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

        // Draw timer and score (updated earlier)
        window.draw(timerText);   // Draw the time (top-left)
        window.draw(scoreText);   // Draw the score (below the time)
        window.draw(highscoreText);  // Draw the highscore


        if (isMuted) window.draw(muteIcon);  // Draw mute icon if muted

        // Switch back to game view (for player/camera)
        window.setView(view);

        // Draw pause text (if paused)
        if (pause) window.draw(pausedText);

        window.display();
    }

    return 0;
}
