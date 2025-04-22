
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class SoundTrack 
{
private:
	sf::Music backgroundMusic; // makes the object for the background Music
	std::vector<std::string> musicFiles; // paths to the music files
	std::string	currentTrack; // the current track that is being played 
public:
	SoundTrack() {
		// initializes the time seed for the music files (randomizes) 
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		// initialized the sound track to my vector of strings 
		musicFiles.push_back("007 Track.mp3");
		musicFiles.push_back("Boomin Track.mp3");
		musicFiles.push_back("Mario Track.mp3");
		musicFiles.push_back("Lofi Track.mp3");
		// still need the load music 
		loadTrack();
	}

	// this function plays the music 
	void playMusic() {
		backgroundMusic.setLoop(true); // set the loop until infinity or unitl the user clicks off the game 

		// saftey code to make sure where the erros is coming from 
		if (!backgroundMusic.openFromFile(currentTrack)) {
			std::cout << "couldnt open music files" << currentTrack << std::endl;
		}
		else {
			backgroundMusic.play();
		}

	}
	// this stops the music 
	void stopMusic() {
		backgroundMusic.stop();
	}

	void loadTrack() {
		int random = std::rand() % musicFiles.size();
		currentTrack = musicFiles[random];
	}
	
};