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
	window.create(sf::VideoMode(900.0f * screenScalingFactor, 700.0f * screenScalingFactor), "SFML works!", sf::Style::Close);

	platform.setIcon(window.getSystemHandle());

	sf::Texture wallTexture;
	wallTexture.loadFromFile("content/wall.png");
	sf::Texture moverTexture;
	moverTexture.loadFromFile("content/mover.png");
	sf::Texture transformerTexture;
	transformerTexture.loadFromFile("content/transformer.png");
	sf::Texture adderTexture;
	adderTexture.loadFromFile("content/add.png");
	sf::Texture deleterTexture;
	deleterTexture.loadFromFile("content/delete.png");

	sf::Event event;
	sf::Mouse mouse;
	sf::Vector2i mousePos;		   //current mouse pos
	sf::Vector2i relativeMousePos; //mouse pos relative to origin of selected object

	bool lastFrameClick = false; //tells whether the mouse1 was pressed on the previous frame

	//1 wall pointer calculated every frame - pointing to the wall the the mouse is over
	//1 wall pointer calculated every click - pointing to the wall that is selected
	//when clicking, if wall is selected, check if mouse is over a movement object

	sf::RectangleShape menuArea;
	menuArea.setSize(sf::Vector2f(200, 700));
	menuArea.setPosition(700, 0);
	menuArea.setFillColor(sf::Color(128, 128, 128, 255));

	sf::RectangleShape wallAdder;
	wallAdder.setSize(sf::Vector2f(150, 150));
	wallAdder.setPosition(725, 25);
	wallAdder.setTexture(&adderTexture);

	sf::RectangleShape wallDeleter;
	wallDeleter.setSize(sf::Vector2f(150, 150));
	wallDeleter.setPosition(725, 525);
	wallDeleter.setTexture(&deleterTexture);

	//Object::Wall firstWall;
	//firstWall.getWall()->setTexture(&wallTexture);
	//firstWall.getMover()->setTexture(&moverTexture);
	//firstWall.getTransformer()->setTexture(&transformerTexture);

	Object::Wall* selectedObject = nullptr; //object clicked on - NOTE: Wall selected = true AFTER a full click & release

	float lastTime = runTimeTicks / CLOCKS_PER_SEC;
	float curTime = runTimeTicks / CLOCKS_PER_SEC;
	float lastLazar = 0;

	std::list<Object::Lazar*> lazarList;
	std::list<Object::Wall*> wallList;

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
		//if its time for another lazar
		if (curTime - lastLazar > 0.2)
		{
			//add a new lazar
			lastLazar = curTime;
			Object::Lazar* newLazar = new Object::Lazar();
			lazarList.insert(lazarList.begin(), newLazar);
		}
		//iterating through each lazar
		for (std::list<Object::Lazar*>::iterator lazarIterator = lazarList.begin(); lazarIterator != lazarList.end(); lazarIterator++)
		{
			//checking if each lazar is in bounds
			if ((*lazarIterator)->getLazar()->getPosition().x > 725 || (*lazarIterator)->getLazar()->getPosition().y > 725 || (*lazarIterator)->getLazar()->getPosition().x < -25 || (*lazarIterator)->getLazar()->getPosition().y < -25)
			{
				//if out of bounds, delete lazar, move iterator back, and continue loop
				lazarList.erase(lazarIterator);
				lazarIterator--;
			}
			else
			{
				//if lazar is in bounds
				(*lazarIterator)->update(curTime - lastTime);
			}
		}

		//if user just clicked
		if (!lastFrameClick && mouse.isButtonPressed(sf::Mouse::Left))
		{
			//if mouse is over rotater of selected object
			if (selectedObject != nullptr && selectedObject->getTransformer()->getGlobalBounds().contains(mousePos.x, mousePos.y))
			{
				selectedObject->selectTransformer();
				relativeMousePos = sf::Vector2i(mousePos.x - selectedObject->getTransformer()->getPosition().x, mousePos.y - selectedObject->getTransformer()->getPosition().y);
			}
			//adding a wall
			else if (wallAdder.getGlobalBounds().contains(mousePos.x, mousePos.y))
			{
				if(selectedObject != nullptr)
				{
					selectedObject->deselect();
				}
				Object::Wall* newWall = new Object::Wall();
				newWall->getWall()->setTexture(&wallTexture);
				newWall->getMover()->setTexture(&moverTexture);
				newWall->getTransformer()->setTexture(&transformerTexture);
				wallList.push_back(newWall);
			}
			//deleting a wall
			/*else if ()
			{
			}*/
			//if mouse is over the wall - will iterate through a list/vector of walls later
			else
			{
				bool found = false;
				for (std::list<Object::Wall*>::iterator wallIterator = wallList.end(); wallIterator != wallList.begin();)
				{
					wallIterator--;
					if ((*wallIterator)->getMover()->getGlobalBounds().contains(mousePos.x, mousePos.y))
					{
						if(selectedObject != nullptr)
						{
							selectedObject->deselect();
						}
						selectedObject = (*wallIterator);
						selectedObject->select();
						relativeMousePos = sf::Vector2i(mouse.getPosition(window).x - selectedObject->getWall()->getPosition().x, mouse.getPosition(window).y - selectedObject->getWall()->getPosition().y);
						wallIterator = wallList.begin();
						found = true;
					}
				}
				if (!found)
				{
					if (selectedObject != nullptr)
					{
						selectedObject->deselect();
						selectedObject = nullptr;
					}
				}
			}

			/*else if (firstWall.getMover()->getGlobalBounds().contains(mousePos.x, mousePos.y))
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
			} */
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
		for (std::list<Object::Wall*>::iterator wallIterator = wallList.begin(); wallIterator != wallList.end(); wallIterator++)
		{
			(*wallIterator)->draw(&window);
		}
		window.draw(menuArea);
		window.draw(wallAdder);
		window.draw(wallDeleter);
		window.display();
	}

	return 0;
}
