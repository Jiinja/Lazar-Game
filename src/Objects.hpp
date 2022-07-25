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
		this->wall = sf::RectangleShape(sf::Vector2f(100, 24));
		this->wall.setFillColor(sf::Color::White);
		this->wall.setOutlineColor(sf::Color::Red);
		this->wall.setOrigin(50, 12);
		this->transformer = sf::CircleShape(12);
		this->transformer.setOrigin(12, 12);
		this->mover = sf::CircleShape(12);
		this->mover.setOrigin(12, 12);
		this->wall.setPosition(50, 50);
	}

	sf::RectangleShape* getWall()
	{
		return &this->wall;
	}

	sf::CircleShape* getTransformer()
	{
		return &this->transformer;
	}

	sf::CircleShape* getMover()
	{
		return &this->mover;
	}

	/**
	 * This function takes in the window and draws the wall
	 * if the wall is selected, draw movement objects that scale to object size
	 */
	void draw(sf::RenderWindow* window)
	{
		window->draw(this->wall);
		this->mover.setPosition(this->wall.getPosition());
		this->mover.setRotation(this->wall.getRotation());
		window->draw(this->mover);
		if (selected)
		{
			this->transformer.setPosition(this->wall.getPosition().x + (((this->wall.getSize().x / 2) + 20) * cos(this->wall.getRotation() * PI / 180)), this->wall.getPosition().y + (((this->wall.getSize().x / 2) + 20) * sin(this->wall.getRotation() * PI / 180)));
			this->transformer.setRotation(this->wall.getRotation() + 135);
			window->draw(transformer);
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
		//Transform Selection
		else if (this->selected == 2)
		{
			float newRotation = atan((this->wall.getPosition().y - mouse->y) / (this->wall.getPosition().x - mouse->x)) / PI * 180;
			if (mouse->x - this->wall.getPosition().x <= 0)
			{
				newRotation += 180;
			}
			if (newRotation > 360)
				newRotation -= 360;
			if (newRotation < 0)
				newRotation += 360;
			this->wall.setRotation(newRotation);

			int wallMouseDist = sqrt(pow((mouse->x - offset->x - this->wall.getPosition().x), 2) + pow((mouse->y - offset->y - this->wall.getPosition().y), 2));
			int wallTransDist = sqrt(pow((this->transformer.getPosition().x - this->wall.getPosition().x), 2) + pow((this->transformer.getPosition().y - this->wall.getPosition().y), 2));

			int distance = wallMouseDist - wallTransDist;
			this->wall.setSize(sf::Vector2f(this->wall.getSize().x + distance, 20));
			if (this->wall.getSize().x < 20)
			{
				this->wall.setSize(sf::Vector2f(20, 20));
			}
			this->wall.setOrigin(this->wall.getSize().x / 2, 10);
		}
	}

	void fixPos()
	{
		if (this->wall.getPosition().x > 700)
			this->wall.setPosition(700, this->wall.getPosition().y);
		if (this->wall.getPosition().y > 700)
			this->wall.setPosition(this->wall.getPosition().x, 700);
		if (this->wall.getPosition().x < 0)
			this->wall.setPosition(0, this->wall.getPosition().y);
		if (this->wall.getPosition().y < 0)
			this->wall.setPosition(this->wall.getPosition().x, 0);

	}

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

	void selectTransformer()
	{
		this->selected = 2;
	}

private:
	int selected;				 //this is updated whenever the object is selected or deselected  0 = no, 1 = selected, 2 = stretch1, 3 = stretch2, 4 = rotater
	sf::RectangleShape wall;	 //main wall object
	sf::CircleShape mover;		 //movement object - only drawn while editing
	sf::CircleShape transformer; //transformation object - only drawn while selected
};

/**
 * This class will have one lazar rectangleshape and int speed in pixels/second
 *
 * update() give the vector of walls and check if it hits any of them. If it hits any of them, move back to where it hit the object, change direction, and continue moving
 */

class Lazar
{
public:
	Lazar(int rotation = 0)
	{
		this->velocity = 100;
		this->lazarBeam = sf::RectangleShape(sf::Vector2f(20, 5));
		this->lazarBeam.setOrigin(10, 2.5);
		this->lazarBeam.setPosition(350, 350);
		this->lazarBeam.setFillColor(sf::Color::Red);
		this->lazarBeam.setRotation(rotation);
	}
	/**
	 * This method takes care of all movement - using velocity and time passage to determine speed regardless of frames per second
	 */
	void update(float timePassed, std::list<Object::Wall*>* wallList)
	{
		//this->lazarBeam.setRotation();
		for (std::list<Object::Wall*>::iterator wallIterator = wallList->end(); wallIterator != wallList->begin(); wallIterator++)
		{

		}
		this->lazarBeam.setPosition(this->lazarBeam.getPosition().x + cos(this->lazarBeam.getRotation() * PI / 180) * (float)this->velocity * timePassed, this->lazarBeam.getPosition().y + sin(this->lazarBeam.getRotation() * PI / 180) * (float)this->velocity * timePassed);
	}

	sf::RectangleShape* getLazar()
	{
		return &this->lazarBeam;
	}

private:
	sf::RectangleShape lazarBeam;
	int velocity; //pixels/second
};

}