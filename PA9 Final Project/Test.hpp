
#pragma once

class Tests
{
public:

    /***** TEST METHODS *****/

    // Test 1: Check if the player starts at the correct position
    bool testPlayerInitialPosition();

    // Test 2: Check if the bullet moves correctly upward
    bool testBulletMovement();

    // Test 3: Check if the player collides with a platform and jumps
    bool testPlayerPlatformCollision();

    // Test 4: Check if the player collides with an enemy
    bool testPlayerEnemyCollision();

    // Test 5: Check if the score increases when an enemy is killed
    bool testScoreIncreaseOnEnemyKill();
};

