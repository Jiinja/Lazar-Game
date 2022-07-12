#include "math.h"

#define PI 3.14159265

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
		this->wall.setFillColor(sf::Color::White);
		this->wall.setOutlineColor(sf::Color::Red);
		this->wall.setOrigin(50, 10);
		this->stretch1 = this->stretch2 = this->rotate = sf::CircleShape(10);
		this->stretch1.setOrigin(10, 10);
		this->stretch2.setOrigin(10, 10);
		this->rotate.setOrigin(10, 10);
		this->stretch1.setFillColor(sf::Color::White);
		this->stretch2.setFillColor(sf::Color::White);
		this->rotate.setFillColor(sf::Color::Red);
	}

	sf::RectangleShape* getWall()
	{
		return &this->wall;
	}

	sf::CircleShape* getRotater()
	{
		return &this->rotate;
	}

	sf::CircleShape* getStretch1()
	{
		return &this->stretch1;
	}

	sf::CircleShape* getStretch2()
	{
		return &this->stretch2;
	}

	/**
	 * This function takes in the window and draws the wall
	 * if the wall is selected, draw movement objects that scale to object size
	 */
	void draw(sf::RenderWindow* window)
	{
		//this->wall.setRotation(this->wall.getRotation() + 0.01);
		/**
		 * stretchers need to be 1/2 of x size distance from the center, rotated
		 * rotator needs to be 40 pixels from center also rotated
		 */

		window->draw(wall);
		if (selected)
		{
			this->stretch1.setRotation(this->wall.getRotation());
			this->stretch2.setRotation(this->wall.getRotation() + 180);
			this->stretch2.setRotation(this->wall.getRotation() + 90);
			this->stretch1.setPosition(this->wall.getPosition().x + (((this->wall.getSize().x / 2) + 20) * cos(this->wall.getRotation() * PI / 180)), this->wall.getPosition().y + (((this->wall.getSize().x / 2) + 20) * sin(this->wall.getRotation() * PI / 180)));
			this->stretch2.setPosition(this->wall.getPosition().x - (((this->wall.getSize().x / 2) + 20) * cos(this->wall.getRotation() * PI / 180)), this->wall.getPosition().y - (((this->wall.getSize().x / 2) + 20) * sin(this->wall.getRotation() * PI / 180)));
			this->rotate.setPosition(this->wall.getPosition().x + this->wall.getSize().x / 2 * sin((this->wall.getRotation() * PI / 180)), this->wall.getPosition().y - this->wall.getSize().x / 2 * cos((this->wall.getRotation() * PI / 180)));
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
		//normal selection -> move object around
		if (this->selected == 1)
		{
			this->wall.setPosition(mouse->x - offset->x, mouse->y - offset->y);
		}
		//stretch1 selection -> increase length and adjust origin
		else if (this->selected == 2)
		{
			//use rotation and trig to calculate amount of y/x axis shift.
		}
		//stretch2 selection -> opposite of stretch1
		else if (this->selected == 3)
		{
		}
		//rotater selection -> rotate object based on mouse position
		else if (this->selected == 4)
		{
			float newRotation = atan((this->wall.getPosition().y - mouse->y) / (this->wall.getPosition().x - mouse->x)) / PI * 180 - 90;
			if ((this->wall.getPosition().x - mouse->x) < 0)
				newRotation += 180;
			if (newRotation > 360)
				newRotation -= 360;
			if (newRotation < 0)
				newRotation += 360;
			this->wall.setRotation(newRotation);
		}
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

	/**
	 * This function changes the selection status
	 */
	void select()
	{
		this->wall.setOutlineThickness(2);
		this->selected = 1;
	}

	void deselect()
	{
		this->wall.setOutlineThickness(0);
		this->selected = 0;
	}

	void selectStretch1()
	{
		this->selected = 2;
	}

	void selectStretch2()
	{
		this->selected = 3;
	}

	void selectRotater()
	{
		this->selected = 4;
	}

private:
	int selected;			  //this is updated whenever the object is selected or deselected  0 = no, 1 = selected, 2 = stretch1, 3 = stretch2, 4 = rotater
	sf::RectangleShape wall;  //main wall object
	sf::CircleShape rotate;	  //rotation object
	sf::CircleShape stretch1; //increase length object 1 side
	sf::CircleShape stretch2; //increase length object other side
};

}