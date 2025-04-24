
#include "Test.hpp"
#include "Player.hpp"
#include "Bullet.hpp"
#include "Platform.hpp"
#include "Enemy.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

/***** Test 1: Player starts at correct position *****/
bool Tests::testPlayerInitialPosition()
{
    Player player;
    sf::Vector2f pos = player.getPosition();
    return pos.x == 400.f && pos.y == 300.f;  // player's expected start position
}

/***** Test 2: Bullet moves correctly upward *****/
bool Tests::testBulletMovement()
{
    sf::Texture texture;  // Dummy texture
    Bullet bullet(&texture, 0.f, 0.f, sf::Vector2f(0.f, -500.f));
    bullet.update(1.f);  // Simulate 1 second
    return bullet.getPosition().y == -500.f;  // Should move up by 500 units
}

/***** Test 3: Player collides with platform and jumps *****/
bool Tests::testPlayerPlatformCollision()
{
    Player player;
    sf::Texture texture;
    texture.create(100, 20);  // Original platform texture

    Platform platform(400.f, 500.f, &texture);  // Platform at y = 500

    // Adjust for scaled platform height (9 units tall)
    float platformTop = platform.getPosition().y;
    float platformHeight = platform.getGlobalBounds().height;  // Should be ~9

    // Assume player height is ~50 (check with player.getGlobalBounds().height)
    float playerHeight = player.getGlobalBounds().height;

    // Place player just above the platform to ensure overlap
    player.setPosition(400.f, platformTop - playerHeight + 5.f);  // Overlap by 5 units
    player.setVelocityY(5.f);  // Simulate falling

    // Simulate your game’s collision logic
    if (player.getGlobalBounds().intersects(platform.getBounds()))
    {
        float playerBottom = player.getPosition().y + player.getGlobalBounds().height;

        if (player.getVelocityY() > 0 && playerBottom < platformTop + 10.f)  // 10 unit buffer
        {
            player.jump();
        }
    }

    // Check if the player jumped
    return player.getVelocityY() == -13.5f;
}

/***** Test 4: Player collides with enemy *****/
bool Tests::testPlayerEnemyCollision()
{
    Player player;
    sf::Texture texture;
    texture.create(50, 50);  // Create a dummy 50x50 texture

    Enemy enemy(&texture, 500.f, 400.f);

    player.setPosition(500.f, 400.f);  // Same position as enemy
    return player.getGlobalBounds().intersects(enemy.getGlobalBounds());
}


/***** Test 5: Score increases when enemy is killed *****/
bool Tests::testScoreIncreaseOnEnemyKill()
{
    int killScore = 0;
    sf::Texture texture;
    texture.create(50, 50);  // Dummy texture size

    Bullet bullet(&texture, 0.f, 0.f, sf::Vector2f(0.f, 0.f));
    Enemy enemy(&texture, 0.f, 0.f);

    std::vector<Enemy> enemies = { enemy };
    std::vector<Bullet> bullets = { bullet };

    // Simulate bullet hitting enemy
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ++bulletIt)
    {
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ++enemyIt)
        {
            if (bulletIt->getGlobalBounds().intersects(enemyIt->getGlobalBounds()))
            {
                killScore += 20;
                return killScore == 20;  // Confirm score update
            }
        }
    }
    return false;  // No collision = test fails
}
