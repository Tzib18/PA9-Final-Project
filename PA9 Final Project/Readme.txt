/****************************************************************************************

Name: Austin tzib, Amber Conroy, Trent Gamiao, Nour Elsahati *
Lab section: Austin Tzib, Amber Conroy, Trent Gamiao - lab 6,mNour Elsahati - lab 9 *
Date: 4/24/25 *
*****************************************************************************************/
--->Malloc Jump (Inspired by Doodle Jump)<---

Malloc Jump is a vertical platformer game inspired by Doodle Jump. The objective of the game is to climb as high as possible by jumping on various types of platforms while avoiding falling off the screen or colliding with enemies.

In the game, the player can:

Jump between platforms to reach higher altitudes.
Use A and D keys to move left and right.
Shoot bullets in three directions (up, left, right) using the arrow keys to eliminate enemies.
The player earns points based on how high they climb and how many enemies they defeat. The score is updated dynamically during gameplay, and if the player beats the highscore, it gets saved to a file and a "Highscore!" message will be displayed.

--->Platform Types<---

There are three different types of platforms in the game, each with unique behaviors:

Normal Platforms: These stay still as the player jumps on them.
Moving Platforms: These platforms slide left and right, adding a dynamic challenge.
Cracked Platforms: These break after the player jumps on them once, requiring quick thinking to avoid falling.
--->Enemies<---

As the player climbs, enemies appear on the platforms. The player must shoot the enemies to clear them out of the way. If the player collides with an enemy or falls off the screen, the game ends.

--->Scoring<---

Climbing height: The further the player climbs, the more points they earn.
Enemies killed: Every enemy eliminated adds points to the player's score.
Highscore: The highest score achieved is saved to a file, and players are encouraged to beat their previous best.
--->Key Features<---

Movement: Move the player character using the A and D keys, and shoot bullets with the arrow keys.
Platform variety: The game features normal, moving, and cracked platforms that add variety to the gameplay.
Enemies: Enemies appear as the player progresses, adding challenge to the gameplay.
Sound: Background music and sound effects are included (e.g., bullet sounds, player death, enemy death).
Game control: The player can mute/unmute sounds using the M key and pause the game with Escape.
--->Design Decisions<---

One of the key design choices in Malloc Jump was how to handle the different types of platforms. Instead of using a series of if-else statements, we employed inheritance. The base Platform class handles common functions like drawing and collision detection, while the derived classes MovingPlatform and CrackedPlatform implement specific behaviors like sliding and breaking.

--->Polymorphism and Platform Handling<---

Polymorphism is used in the platform system, where each platform type (normal, moving, and cracked) is treated the same way during gameplay.
The behavior of each platform is determined at runtime using the virtual functions defined in the Platform class and its derived classes.
--->Bullet Handling<---

Instead of using a single texture for all bullet directions, we created separate textures for shooting up, left, and right. This decision made the shooting mechanics more visually clear and intuitive, without complicating the code.

--->Game Control Enhancements<---

The ability to mute and unmute sounds enhances the user experience, giving players control over the audio.
The pause feature allows players to stop the game and resume it at their convenience.
--->Controls---<

A and D: Move the player left and right.
Arrow Keys: Shoot bullets in the direction the player is facing (up, left, or right).
M: Mute or unmute the game’s audio.
Escape: Pause or resume the game.

--->youtube video link<---

the video will cover the overal game of malloc jump https://www.youtube.com/shorts/F__YStrRIp4