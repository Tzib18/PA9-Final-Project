#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Window.hpp"
// end goal worked i was able to start it on the pc and then transfer it to my laptop
int main(void)
{										// x, y 
	sf::RenderWindow windows(sf::VideoMode(800, 800), "Test run of using sfml", sf::Style::Titlebar | sf::Style::Close); // this sets up the window screen 
	sf::Event event; // event is when something is happening 
	// like if they want to move right you will call event and a keybaord fucntion like A. 
	while (windows.isOpen()) { // if the user compiles and the screen is open 
		while (windows.pollEvent(event)) { // if their is a window open it will proccess this loop
			// and render the window 
			if (event.type == sf::Event::Closed) {// if you hit the X button this will call 
				// to close the entire window itself. 
				windows.close(); 
			}
		}
	}
	// hello  
	// test



	// Testing the test branch
	return 0; 
}