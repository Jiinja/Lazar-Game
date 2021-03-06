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
	Object::LazarGun* lazarGun = new Object::LazarGun(800, 350);
	lazarGun->getTransformer()->setTexture(&transformerTexture);

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//getting current mouse position and time for frame calculations
		mousePos = mouse.getPosition(window);
		runTimeTicks = clock();
		curTime = (float)runTimeTicks / CLOCKS_PER_SEC;
		//if its time for another lazar
		if (curTime - lastLazar > 0.2)
		{
			//add a new lazar
			lastLazar = curTime;
			Object::Lazar* newLazar = new Object::Lazar(lazarGun->getLazarGun()->getRotation(), lazarGun->getLazarGun()->getPosition().x, lazarGun->getLazarGun()->getPosition().y);
			lazarList.insert(lazarList.begin(), newLazar);
		}
		//iterating through each lazar
		for (std::list<Object::Lazar*>::iterator lazarIterator = lazarList.begin(); lazarIterator != lazarList.end(); lazarIterator++)
		{
			//checking if each lazar is in bounds
			if ((*lazarIterator)->getLazar()->getPosition().x > 725 || (*lazarIterator)->getLazar()->getPosition().y > 725 || (*lazarIterator)->getLazar()->getPosition().x < -25 || (*lazarIterator)->getLazar()->getPosition().y < -25)
			{
				//if out of bounds, delete lazar, move iterator back, and continue loop
				delete *lazarIterator;
				lazarList.erase(lazarIterator);
				lazarIterator--;
			}
			else
			{
				//if lazar is in bounds
				(*lazarIterator)->update(curTime - lastTime, &wallList);
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
			//if mouse is over rotator of lazarGun
			else if (lazarGun->isSelected() && lazarGun->getTransformer()->getGlobalBounds().contains(mousePos.x, mousePos.y))
			{
				lazarGun->selectTransformer();
				relativeMousePos = sf::Vector2i(mousePos.x - lazarGun->getTransformer()->getPosition().x, mousePos.y - lazarGun->getTransformer()->getPosition().y);
			}
			//if mouse is over the lazarGun
			else if (lazarGun->getLazarGun()->getGlobalBounds().contains(mousePos.x, mousePos.y))
			{
				if(selectedObject != nullptr)
				{
					selectedObject->deselect();
					selectedObject = nullptr;
				}
				lazarGun->select();
				relativeMousePos = sf::Vector2i(mouse.getPosition(window).x - lazarGun->getLazarGun()->getPosition().x, mouse.getPosition(window).y - lazarGun->getLazarGun()->getPosition().y);
			}
			//adding a wall if you click on the wall adder
			else if (wallAdder.getGlobalBounds().contains(mousePos.x, mousePos.y))
			{
				//deselecting old object
				lazarGun->deselect();
				if (selectedObject != nullptr)
				{
					selectedObject->deselect();
				}
				Object::Wall* newWall = new Object::Wall(mousePos.x, mousePos.y);
				newWall->getWall()->setTexture(&wallTexture);
				newWall->getMover()->setTexture(&moverTexture);
				newWall->getTransformer()->setTexture(&transformerTexture);
				//selecting new object so you can drag and drop
				selectedObject = newWall;
				selectedObject->select();
				wallList.push_back(newWall);
			}
			//if mouse is over a wall - will select the wall "on top" of other walls
			else
			{
				//deselecting the lazar
				lazarGun->deselect();
				bool found = false;
				for (std::list<Object::Wall*>::iterator wallIterator = wallList.end(); wallIterator != wallList.begin();)
				{
					wallIterator--;
					if ((*wallIterator)->getMover()->getGlobalBounds().contains(mousePos.x, mousePos.y))
					{
						if (selectedObject != nullptr)
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
			lastFrameClick = true;
		}
		//if you have a selected object and are clicking
		if (selectedObject != nullptr && mouse.isButtonPressed(sf::Mouse::Left))
		{
			selectedObject->move(&mousePos, &relativeMousePos);
		}
		else if (lazarGun->isSelected() && mouse.isButtonPressed(sf::Mouse::Left))
		{
			lazarGun->move(&mousePos, &relativeMousePos);
		}
		//if you just "let go" of object you are moving
		else if (selectedObject != nullptr && !mouse.isButtonPressed(sf::Mouse::Left))
		{
			//check if it should be deleted
			if (wallDeleter.getGlobalBounds().contains(selectedObject->getMover()->getPosition().x, selectedObject->getMover()->getPosition().y))
			{
				for (std::list<Object::Wall*>::iterator wallIterator = wallList.begin(); wallIterator != wallList.end(); wallIterator++)
				{
					//item in the list is found, erased and deleted
					if (*wallIterator == selectedObject)
					{
						wallList.erase(wallIterator);
						delete selectedObject;
						selectedObject = nullptr;
						break;
					}
				}
			}
			//else make sure it is within bounds and accessable to the user
			else
			{
				selectedObject->fixPos();
			}
		}
		//if mouse was just released
		if (lastFrameClick && !mouse.isButtonPressed(sf::Mouse::Left))
		{
			lastFrameClick = false;
		}
		//frame processing is completed, set last time as current time for time between frames
		lastTime = curTime;
		//drawing everything
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
		//if you are moving an object, draw it over the menu so you can move it from the adder and to the deleter
		if (selectedObject != nullptr && mouse.isButtonPressed(sf::Mouse::Left))
		{
			selectedObject->draw(&window);
		}
		lazarGun->draw(&window);
		window.display();
	}

	return 0;
}
