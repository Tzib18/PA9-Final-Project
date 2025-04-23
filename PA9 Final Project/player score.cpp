#pragma once
#include "Player score.hpp"


void PlayerScore(int& score, const std::vector<Enemy>& Enemies1, const std::vector<Enemy>& Enemies2, 
	std:: vector<Bullet> & bullets) {
	// making a loop for the bullet collison so it checks to see if the bullet hit the cahracter
	// bHit (bullet hit)
	for (auto bHit = bullets.begin(); bHit != bullets.end();)// if the bullet hits target start a score counter 
		// if it misses end it.
	{

		bool hit = false; 
		/*		Enemy 1		*/
		// update the score to see if the bullet hit the enemy
		// eHit (enemey hit)
		for (auto eHit = Enemies1.begin(); eHit != Enemies1.end();) {
			if (bHit->getGlobalBounds().intersects(eHit->getGlobalBounds())) {

				//globalDeathSound.play();

				// if the enemey is hit erase th echaracter and update the score
				eHit = const_cast<std::vector<Enemy>&>(Enemies1).erase(eHit);
				score += 15; // every kill is 15 points 
				hit = true; 
				break; // putting this so the bullet doesnt go infinite 

			}
			else {
				++eHit; // if you dont hit anything jsut keep going until you see another 
				// enemy;
			}
		}

		/*		Enemy 2		*/
		// same thing as the first one but for the second enemey
		if (!hit) {
			for (auto eHit = Enemies2.begin(); eHit != Enemies2.end();) {
				if (bHit->getGlobalBounds().intersects(eHit->getGlobalBounds())) {

					eHit = const_cast<std::vector<Enemy>&>(Enemies2).erase(eHit);
					score += 15; // 15 points for the second enemy
					hit = true;
					break; 
				}
				else {
					++eHit; 
				}

			}
		}

		if (hit) {
			bHit = bullets.erase(bHit); // if the bullet hits erase it
		}
		else {
			++bHit; // if the bullet doesnt hit itll just keep reloading 
		}

	}
}
