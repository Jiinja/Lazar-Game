namespace Object
{

struct lineCoords
{
	int x1;
	int y1;
	int x2;
	int y2;
};

class Wall
{
public:
	Wall()
	{
		this->selected = false;
		this->wall = sf::RectangleShape(sf::Vector2f(100, 20));
		this->wall.setFillColor(sf::Color::Magenta);
	}

	sf::RectangleShape* getWall()
	{
		return &this->wall;
	}

	sf::RectangleShape* getRotater()
	{
		return &this->rotate;
	}

	sf::RectangleShape* getStretch1()
	{
		return &this->stretch1;
	}

	sf::RectangleShape* getStretch2()
	{
		return &this->stretch2;
	}

	/**
	 * This function takes in the window and draws the wall
	 * if the wall is selected, draw movement objects
	 */
	void draw(sf::RenderWindow* window)
	{
		window->draw(wall);
		if (selected)
		{
			window->draw(rotate);
			window->draw(stretch1);
			window->draw(stretch2);
		}
	}

	/**
	 * This function is only called when the object is on top and mouse1 is held
	 * Takes in mouse location and location of the mouse relative to the object
	 * moves the object so that the mouse location on the object remains unchanged
	 */
	void move(sf::Vector2i* mouse, sf::Vector2i* offset)
	{
		this->wall.setPosition(mouse->x - offset->x, mouse->y - offset->y);
	}

	/**
	 * This function is only called when the object is selected via a click
	 * this function takes the mouse location and checks if mouse1 is held while on top of the movement objects
	 * if mouse1 is held and over movement objects, wall is stretched or rotated
	 */
	/*void transform(sf::Vector2i* mouse)
	{
	}*/

	/**
	 * This function is called when exiting edit mode
	 * calculates the angle/slope of the wall so that it can be used for reflections
	 */
	void updateReflection();

private:
	bool selected;				 //this is updated whenever the object is selected or deselected
	sf::RectangleShape wall;	 //main wall object
	sf::RectangleShape rotate;	 //rotation object
	sf::RectangleShape stretch1; //increase length object 1 side
	sf::RectangleShape stretch2; //increase length object other side
};

}