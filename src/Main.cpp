#include "Objects.hpp"
#include "Platform/Platform.hpp"
#include "list"

int main()
{
	util::Platform platform;

#if defined(_DEBUG)
	std::cout << "Hello World!" << std::endl;
#endif

	clock_t runTimeTicks = clock();

	sf::RenderWindow window;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	window.create(sf::VideoMode(700.0f * screenScalingFactor, 700.0f * screenScalingFactor), "SFML works!", sf::Style::Close);

	platform.setIcon(window.getSystemHandle());

	sf::Texture wallTexture;
	wallTexture.loadFromFile("content/wall.png");
	sf::Texture moverTexture;
	moverTexture.loadFromFile("content/mover.png");
	sf::Texture transformerTexture;
	transformerTexture.loadFromFile("content/transformer.png");

	sf::Event event;
	sf::Mouse mouse;
	sf::Vector2i mousePos;		   //current mouse pos
	sf::Vector2i relativeMousePos; //mouse pos relative to origin of selected object

	bool lastFrameClick = false; //tells whether the mouse1 was pressed on the previous frame

	//1 wall pointer calculated every frame - pointing to the wall the the mouse is over
	//1 wall pointer calculated every click - pointing to the wall that is selected
	//when clicking, if wall is selected, check if mouse is over a movement object

	Object::Wall firstWall;
	firstWall.getWall()->setTexture(&wallTexture);
	firstWall.getMover()->setTexture(&moverTexture);
	firstWall.getTransformer()->setTexture(&transformerTexture);

	//Object::Wall* mouseOver = &firstWall; //object directly under the mouse
	Object::Wall* selectedObject = nullptr; //object clicked on - NOTE: Wall selected = true AFTER a full click & release

	float lastTime = runTimeTicks / CLOCKS_PER_SEC;
	float curTime = runTimeTicks / CLOCKS_PER_SEC;
	float lastLazar = 0;

	std::list<Object::Lazar*> lazarList;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		mousePos = mouse.getPosition(window);
		runTimeTicks = clock();
		curTime = (float)runTimeTicks / CLOCKS_PER_SEC;
		std::cout << "Cur time: " << curTime << "  Last Lazar: " << lastLazar << std::endl;
		//if its time for another lazar
		if (curTime - lastLazar > 0.1)
		{
			//add a new lazar
			lastLazar = curTime;
			Object::Lazar* newLazar = new Object::Lazar;
			lazarList.insert(lazarList.end(), newLazar);
		}
		//iterating through each lazar
		for (std::list<Object::Lazar*>::iterator lazarIterator = lazarList.begin(); lazarIterator != lazarList.end(); lazarIterator++)
		{
			//checking if each lazar is in bounds
			if ((*lazarIterator)->getLazar()->getPosition().x > 700 || (*lazarIterator)->getLazar()->getPosition().y > 700)
			{
				//if out of bounds, delete lazar, move iterator back, and continue loop
				lazarList.erase(lazarIterator);
				lazarIterator--;
			}
			else
			{
				//if lazar is in bounds
				(*lazarIterator)->update(curTime - lastTime, firstWall.getWall()->getRotation());
			}
		}

		//if user just clicked
		if (!lastFrameClick && mouse.isButtonPressed(sf::Mouse::Left))
		{
			//if mouse is over rotater of selected object
			if (selectedObject != nullptr && selectedObject->getTransformer()->getGlobalBounds().contains(mouse.getPosition(window).x, mouse.getPosition(window).y))
			{
				selectedObject->selectTransformer();
				relativeMousePos = sf::Vector2i(mouse.getPosition(window).x - selectedObject->getTransformer()->getPosition().x, mouse.getPosition(window).y - selectedObject->getTransformer()->getPosition().y);
			}
			//if mouse is over the wall - will iterate through a list/vector of walls later
			//else if (firstWall.getWall()->getGlobalBounds().contains(mouse.getPosition(window).x, mouse.getPosition(window).y))
			else if (firstWall.getMover()->getGlobalBounds().contains(mousePos.x, mousePos.y))
			{
				selectedObject = &firstWall;
				selectedObject->select();
				relativeMousePos = sf::Vector2i(mouse.getPosition(window).x - selectedObject->getWall()->getPosition().x, mouse.getPosition(window).y - selectedObject->getWall()->getPosition().y);
			}
			//if mouse is over nothing
			else
			{
				if (selectedObject != nullptr)
				{
					selectedObject->deselect();
					selectedObject = nullptr;
				}
			}
			lastFrameClick = true;
		}

		if (selectedObject != nullptr && mouse.isButtonPressed(sf::Mouse::Left))
		{
			selectedObject->move(&mousePos, &relativeMousePos);
		}
		if (lastFrameClick && !mouse.isButtonPressed(sf::Mouse::Left)) //if mouse was just released
		{
			lastFrameClick = false;
		}

		lastTime = curTime;
		window.clear();
		for (std::list<Object::Lazar*>::iterator lazarIterator = lazarList.begin(); lazarIterator != lazarList.end(); lazarIterator++)
		{
			window.draw(*(*lazarIterator)->getLazar());
		}
		firstWall.draw(&window);
		window.display();
	}

	return 0;
}
