
// dont know if i need to incldue all of this its differnent from all the other files 
#pragma
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
//#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Player.hpp"
#include "Platform.hpp"
#include "Menu.hpp"
#include "BackgroundAudioTrack.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"

void PlayerScore(int& score, const std::vector<Enemy>& Enemies1, const std::vector<Enemy>& Enemies2,
	std::vector<Bullet>& bullets);