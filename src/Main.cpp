#include "Objects.hpp"
#include "Platform/Platform.hpp"

int main()
{
	util::Platform platform;

#if defined(_DEBUG)
	std::cout << "Hello World!" << std::endl;
#endif

	sf::RenderWindow window;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	window.create(sf::VideoMode(700.0f * screenScalingFactor, 700.0f * screenScalingFactor), "SFML works!", sf::Style::Close);
	platform.setIcon(window.getSystemHandle());

	sf::Texture shapeTexture;
	sf::Texture defaultObject;
	defaultObject.loadFromFile("content/Wall.png");

	sf::Event event;
	sf::Mouse mouse;
	sf::Vector2i mousePos;		   //current mouse pos
	sf::Vector2i lastMouse;		   //last frame mouse pos
	sf::Vector2i relativeMousePos; //mouse pos relative to origin of selected object

	bool lastClick = false; //tells whether the mouse1 was pressed on the previous frame

	//1 wall pointer calculated every frame - pointing to the wall the the mouse is over
	//1 wall pointer calculated every click - pointing to the wall that is selected
	//when clicking, if wall is selected, check if mouse is over a movement object

	Object::Wall firstWall;
	firstWall.getWall()->setTexture(&defaultObject);

	//Object::Wall* mouseOver = &firstWall; //object directly under the mouse
	Object::Wall* Selected = &firstWall; //object clicked on - NOTE: Wall selected = true AFTER a full click & release

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		lastMouse = mousePos;
		mousePos = mouse.getPosition(window);
		if (firstWall.getWall()->getGlobalBounds().contains(mouse.getPosition(window).x, mouse.getPosition(window).y))
		{
			//mouseOver = &firstWall;
			if (mouse.isButtonPressed(sf::Mouse::Left) /*&& mouse is not over a movement object */)
			{
				std::cout << "Wall is clicked on" << std::endl;
				Selected = &firstWall;
				if (lastClick == false) //if first frame of click, save mouse position relative to selected object
				{
					relativeMousePos = sf::Vector2i(mouse.getPosition(window).x - firstWall.getWall()->getPosition().x, mouse.getPosition(window).y - firstWall.getWall()->getPosition().y);
					std::cout << "selected at: "
							  << "(" << mousePos.x << "," << mousePos.y << ")" << std::endl;
				}
				lastClick = true;
			}
			else
				Selected = nullptr;
		}
		if (Selected != nullptr && mouse.isButtonPressed(sf::Mouse::Left))
			Selected->move(&mousePos, &relativeMousePos);
		if (lastClick && !mouse.isButtonPressed(sf::Mouse::Left)) //if mouse was just released
			lastClick = false;

		window.clear();
		firstWall.draw(&window);
		window.display();
	}

	return 0;
}
