#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Window.hpp"
#include <iostream>
#include "Player.hpp"

// end goal worked i was able to start it on the pc and then transfer it to my laptop
int main(void)
{										// x, y 
// Create the game window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Doodle Jump -  PA9");
    // Keeps a steady frame rate
    window.setFramerateLimit(60);

    // Load and scale the background image
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("paper_background.jpg"))
    {
        std::cerr << "Error: Could not load paper_background.jpg\n";
        return -1;
    }

    sf::Sprite background(bgTexture);

    // Scale the background to match the window size
    sf::Vector2u textureSize = bgTexture.getSize();   // Image size
    sf::Vector2u windowSize = window.getSize();       // Window size
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    background.setScale(scaleX, scaleY);

    // Set up the player
    Player player;

    sf::Clock clock; // For delta time used later to keep score

    // Game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Time between frames
        float deltaTime = clock.restart().asSeconds();

        // Handle player input and update movement
        player.handleInput();
        player.update(deltaTime);

        // Draw everything
        window.clear();                 // Clear the screen
        window.draw(background);       // Draw scaled background
        window.draw(player);           // Draw player on top
        window.display();              // Show the result
    }

    return 0;
}