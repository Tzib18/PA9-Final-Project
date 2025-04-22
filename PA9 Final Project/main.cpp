
// SFML PA9 Doodle Jump
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Player.hpp"
#include "Platform.hpp"
#include "Menu.hpp"
#include "BackgroundAudioTrack.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
// running application 
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Doodle Jump - PA9");
    window.setFramerateLimit(60);

    // ***** MUSIC SETUP *****
    SoundTrack musicPlayer;
    musicPlayer.playMusic();

    Menu menu(window);
    menu.run();

    sf::View view = window.getDefaultView();

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

    sf::Texture platformTexture;
    if (!platformTexture.loadFromFile("platform.png")) return -1;
    std::vector<Platform> platforms = {
        Platform(100.f, 500.f, &platformTexture),
        Platform(300.f, 400.f, &platformTexture),
        Platform(150.f, 300.f, &platformTexture),
        Platform(350.f, 200.f, &platformTexture),
        Platform(200.f, 100.f, &platformTexture)
    };
    float highestPlatformY = 100.f;

    Player player;

    sf::Texture purpleGlobTexture, EnemiesTexture;
    if (!purpleGlobTexture.loadFromFile("purple_glob.png")) return -1;
    if (!EnemiesTexture.loadFromFile("enemies.png")) return -1;
    std::vector<Enemy> Enemies1, Enemies2;

    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("Bullet.png")) return -1;
    std::vector<Bullet> bullets;

    bool isMuted = false;
    sf::SoundBuffer deathBuffer, bulletBuffer, globDeathBuffer, enemy2DeathBuffer;
    sf::Sound deathSound, bulletSound, globDeathSound, enemy2DeathSound;
    if (!deathBuffer.loadFromFile("player_death_sound.wav")) return -1;
    if (!bulletBuffer.loadFromFile("bullet_sound.wav")) return -1;
    if (!globDeathBuffer.loadFromFile("purple_glob_death_sound_effect.wav")) return -1;
    if (!enemy2DeathBuffer.loadFromFile("enemie2_death_sound.wav")) return -1;
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

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    sf::Vector2f pos = player.getPosition();
                    bullets.emplace_back(&bulletTexture, pos.x + player.getGlobalBounds().width / 2.f - 5.f, pos.y);
                    bulletSound.play();
                }
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

        float deltaTime = clock.restart().asSeconds();
        player.handleInput();
        player.update(deltaTime);

        sf::Vector2f playerPos = player.getPosition();
        float playerWidth = player.getGlobalBounds().width;
        float yLock = playerPos.y;
        if (playerPos.x > view.getCenter().x + 400.f)
            player.setPosition(view.getCenter().x - 400.f - playerWidth, yLock);
        if (playerPos.x + playerWidth < view.getCenter().x - 400.f)
            player.setPosition(view.getCenter().x + 400.f, yLock);

        for (auto& platform : platforms)
        {
            if (player.getBounds().intersects(platform.getBounds()))
            {
                float playerBottom = player.getPosition().y + player.getGlobalBounds().height;
                float platformTop = platform.getPosition().y;
                if (player.getVelocityY() > 0 && playerBottom < platformTop + 10.f)
                    player.jump();
            }
        }

        if (player.getPosition().y < view.getCenter().y - 100.f)
        {
            view.setCenter(view.getCenter().x, player.getPosition().y + 100.f);
            window.setView(view);
        }

        if (player.getPosition().y > view.getCenter().y + 300.f)
        {
            deathSound.play();  //Play death sound
            std::cout << "Game Over! (fell off)" << std::endl;
            sf::sleep(sf::seconds(1.0f)); // Let the sound finish before closing
            window.close();
        }

        float topOfView = view.getCenter().y - window.getSize().y / 2.f;
        while (topOfView - window.getSize().y < highestBG)
        {
            sf::Sprite newBG(bgTexture);
            newBG.setScale(bgScaleX, bgScaleY);
            highestBG -= window.getSize().y;
            newBG.setPosition(0.f, highestBG);
            backgrounds.push_back(newBG);
        }

        backgrounds.erase(std::remove_if(backgrounds.begin(), backgrounds.end(),
            [&view, &window](const sf::Sprite& bg)
            {
                float bgTop = bg.getPosition().y;
                float viewBottom = view.getCenter().y + window.getSize().y / 2.f;
                return bgTop > viewBottom + window.getSize().y;
            }), backgrounds.end());

        while (highestPlatformY > view.getCenter().y - 300.f)
        {
            highestPlatformY -= static_cast<float>(rand() % 50 + 80);
            float centerX = view.getCenter().x;
            float x = static_cast<float>((rand() % 350) + (centerX - 180));
            platforms.emplace_back(x, highestPlatformY, &platformTexture);
            if ((rand() % 5) == 0)
            {
                if ((rand() % 2) == 0)
                {
                    Enemies1.emplace_back(&purpleGlobTexture, x + 10.f, highestPlatformY - 40.f);
                    Enemies1.back().setScale(0.15f, 0.15f);
                }
                else 
                {
                    Enemies2.emplace_back(&EnemiesTexture, x + 10.f, highestPlatformY - 40.f);
                    Enemies2.back().setScale(0.5f, 0.5f);
                }
            }
        }

        float screenBottom = view.getCenter().y + static_cast<float>(window.getSize().y) / 2.f;
        platforms.erase(std::remove_if(platforms.begin(), platforms.end(),
            [&](const Platform& p)
            {
                return p.getPosition().y > screenBottom + 100.f;
            }), platforms.end());

        for (auto& enemy : Enemies1) enemy.update(deltaTime);
        for (auto& enemy : Enemies2) enemy.update(deltaTime);
        for (auto& bullet : bullets) bullet.update(deltaTime);

        float screenTop = view.getCenter().y - window.getSize().y / 2.f;
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [screenTop](const Bullet& b) { return b.isOffScreen(screenTop); }), bullets.end());

        for (auto& enemy : Enemies1)
        {
            if (player.getGlobalBounds().intersects(enemy.getGlobalBounds()))
            {
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
                deathSound.play();
                std::cout << "Game Over! (hit enemy)" << std::endl;
                sf::sleep(sf::seconds(1.0f));
                window.close();
            }
        }

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

        muteIcon.setPosition(view.getCenter().x - window.getSize().x / 2.f + 10.f,
            view.getCenter().y + window.getSize().y / 2.f - 60.f);

        window.clear();
        for (auto& bg : backgrounds) window.draw(bg);
        for (auto& platform : platforms) window.draw(platform);
        for (auto& enemy : Enemies1) window.draw(enemy);
        for (auto& enemy : Enemies2) window.draw(enemy);
        for (auto& bullet : bullets) window.draw(bullet);
        window.draw(player);
        if (isMuted) window.draw(muteIcon);
        window.display();
    }

    return 0;
}

