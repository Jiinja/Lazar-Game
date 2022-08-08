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
	Wall(int x = 50, int y = 50)
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
		this->wall.setPosition(x, y);
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
	 * This function takes in the window and draws the wall + movement object
	 * if the wall is selected, draw transformer object that scales to object size
	 */
	void draw(sf::RenderWindow* window, bool movable)
	{
		window->draw(this->wall);
		//if lazars aren't shooting
		if (movable)
		{
			this->mover.setPosition(this->wall.getPosition());
			this->mover.setRotation(this->wall.getRotation());
			window->draw(this->mover);
		}
		//if clicked on / selected
		if (selected)
		{
			//updating transformer location & drawing
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
		//Transform Selection -> rotate & resize object
		else if (this->selected == 2)
		{
			//calculating new rotation based on mouse position
			double newRotation = atan((this->wall.getPosition().y - mouse->y) / (this->wall.getPosition().x - mouse->x)) / PI * 180;
			if (mouse->x - this->wall.getPosition().x <= 0)
			{
				newRotation += 180;
			}
			if (newRotation > 360)
				newRotation -= 360;
			if (newRotation < 0)
				newRotation += 360;
			this->wall.setRotation(newRotation);

			//calculating new size & origin based on mouse distance
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

	/**
	 * This function is used to make sure that walls are always movable and never lost outside of the play area
	 */
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
	 * This function updates selected to true & updates the outline
	 */
	void select()
	{
		this->wall.setOutlineThickness(2);
		this->selected = 1;
	}

	/**
	 * This function deselects the object and removes the outline
	 */
	void deselect()
	{
		this->wall.setOutlineThickness(0);
		this->selected = 0;
	}

	/**
	 * This function updates the selection status so the move function knows that the transformer has been selected
	 */
	void selectTransformer()
	{
		this->selected = 2;
	}

protected:
	int selected;				 //this is updated whenever the object is selected or deselected  0 = no, 1 = selected, 2 = stretch1, 3 = stretch2, 4 = rotater
	sf::RectangleShape wall;	 //main wall object
	sf::CircleShape mover;		 //movement object - only drawn while editing
	sf::CircleShape transformer; //transformation object - only drawn while selected
};

class LazarGun : public Wall
{
public:
	LazarGun(int x = 350, int y = 350)
	{
		this->selected = false;
		this->wall = sf::RectangleShape(sf::Vector2f(36, 24));
		this->wall.setFillColor(sf::Color::White);
		this->wall.setOutlineColor(sf::Color::Red);
		this->wall.setOrigin(18, 12);
		this->transformer = sf::CircleShape(12);
		this->transformer.setOrigin(12, 12);
		this->wall.setPosition(x, y);
	}

	/**
	 * This function draws the lazargun object and the transformer if selected
	 */
	void draw(sf::RenderWindow* window)
	{
		window->draw(this->wall);
		if (selected)
		{
			this->transformer.setPosition(this->wall.getPosition().x + (((this->wall.getSize().x / 2) + 20) * cos(this->wall.getRotation() * PI / 180)), this->wall.getPosition().y + (((this->wall.getSize().x / 2) + 20) * sin(this->wall.getRotation() * PI / 180)));
			this->transformer.setRotation(this->wall.getRotation() + 135);
			window->draw(transformer);
		}
	}

	/**
	 * This function moves the lazargun object depending on the selection status
	 *
	 */
	void move(sf::Vector2i* mouse, sf::Vector2i* offset)
	{
		//normal selection -> move object around
		if (this->selected == 1)
		{
			this->wall.setPosition(mouse->x - offset->x, mouse->y - offset->y);
		}
		//Transform Selection -> rotate object
		else if (this->selected == 2)
		{
			double newRotation = atan((this->wall.getPosition().y - mouse->y) / (this->wall.getPosition().x - mouse->x)) / PI * 180;
			if (mouse->x - this->wall.getPosition().x <= 0)
			{
				newRotation += 180;
			}
			if (newRotation > 360)
				newRotation -= 360;
			if (newRotation < 0)
				newRotation += 360;
			this->wall.setRotation(newRotation);
		}
	}

	/**
	 * This function updates selection and outlines the lazargun
	 */
	void select()
	{
		//this->lazarGun.setOutlineThickness(2);
		this->selected = 1;
	}

	/**
	 * this function is used to determine the selection state
	 */
	bool isSelected()
	{
		return this->selected;
	}
};

/**
 * This class will have one lazar rectangleshape and int speed in pixels/second
 *
 * update() give the vector of walls and check if it hits any of them. If it hits any of them, move back to where it hit the object, change direction, and continue moving
 */

class Lazar
{
public:
	Lazar(int rotation = 0, int x = 350, int y = 350, int newVelocity = 100)
	{
		this->velocity = newVelocity;
		this->lazarBeam = sf::RectangleShape(sf::Vector2f(20, 5));
		this->lazarBeam.setOrigin(10, 2.5);
		this->lazarBeam.setPosition(x, y);
		this->lazarBeam.setFillColor(sf::Color::White);
		this->lazarBeam.setRotation(rotation);
		this->reflectionReady = false;
		this->nextReflectionDistance = 1000;
		this->lastReflection = nullptr;
	}

	/**
	 * This function takes care of all movement - using velocity and time passage to determine speed regardless of frames per second
	 * This function additionally checks locations of each wall and takes care of reflections
	 */
	void update(double timePassed, std::list<Object::Wall*>* wallList)
	{
		//updating standard movement parameters
		double nextDistance = (double)this->velocity * timePassed;

		while (nextDistance != 0)
		{
			//recalculate reflection?
			if (!reflectionReady)
			{
				calculateReflection(wallList);
			}
			//if the reflection will happen in this frame
			if (this->nextReflectionDistance <= nextDistance)
			{
				//move lazar to the reflection point & update angle
				this->lazarBeam.setPosition(this->lazarBeam.getPosition().x + cos(this->lazarBeam.getRotation() * PI / 180) * this->nextReflectionDistance, this->lazarBeam.getPosition().y + sin(this->lazarBeam.getRotation() * PI / 180) * this->nextReflectionDistance);
				this->lazarBeam.setRotation(this->nextReflectionAngle);
				//updating distance travelled
				nextDistance -= this->nextReflectionDistance;
				reflectionReady = false;
			}
			else
			{
				this->lazarBeam.setPosition(this->lazarBeam.getPosition().x + cos(this->lazarBeam.getRotation() * PI / 180) * nextDistance, this->lazarBeam.getPosition().y + sin(this->lazarBeam.getRotation() * PI / 180) * nextDistance);
				this->nextReflectionDistance = this->nextReflectionDistance - nextDistance;
				nextDistance = 0;
			}
		}
	}

	sf::RectangleShape* getLazar()
	{
		return &this->lazarBeam;
	}

private:
	/**
 	* This function takes in a list of walls and determines when the next reflection will be
 	* It adjusts member variables so no return is necessary
	*/
	void calculateReflection(std::list<Object::Wall*>* wallList)
	{
		//coordinates used for equation of lazar line
		double nextX = this->lazarBeam.getPosition().x + cos(this->lazarBeam.getRotation() * PI / 180) * 100;
		double nextY = this->lazarBeam.getPosition().y + sin(this->lazarBeam.getRotation() * PI / 180) * 100;

		//variables for determining next reflection
		double minInterceptDist = 1000;
		int wallAngle = 0;
		Object::Wall* reflector = nullptr;

		//lazar "line" = a1x + b1y = c1
		double a1 = this->lazarBeam.getPosition().y - nextY;
		double b1 = nextX - this->lazarBeam.getPosition().x;
		double c1 = a1 * nextX + b1 * nextY;

		//iterating through each wall finding the closest wall on course to intercept/reflect
		for (std::list<Object::Wall*>::iterator wallIterator = wallList->begin(); wallIterator != wallList->end(); wallIterator++)
		{
			Object::Wall* currentWall = *wallIterator;
			//ensuring lazars cannot reflect off of the same wall twice
			if (currentWall != this->lastReflection)
			{
				//each wall has 4 sides
				for (int i = 0; i < 4; i++)
				{
					int p1 = i;
					int p2 = i + 1;
					if (p2 > 3)
						p2 = 0;
					//getting coordinates of wall corners
					double x1 = currentWall->getWall()->getTransform().transformPoint(currentWall->getWall()->getPoint(p1)).x;
					double y1 = currentWall->getWall()->getTransform().transformPoint(currentWall->getWall()->getPoint(p1)).y;
					double x2 = currentWall->getWall()->getTransform().transformPoint(currentWall->getWall()->getPoint(p2)).x;
					double y2 = currentWall->getWall()->getTransform().transformPoint(currentWall->getWall()->getPoint(p2)).y;

					//wall side "line" = a2x + b2y = c2
					double a2 = y1 - y2;
					double b2 = x2 - x1;
					double c2 = a2 * x2 + b2 * y2;

					double determinant = a1 * b2 - a2 * b1;
					//parallel check
					if (determinant != 0.00)
					{
						//finding possible intercept/reflection point
						double xIntercept = (b2 * c1 - b1 * c2) / determinant;
						double yIntercept = (a1 * c2 - a2 * c1) / determinant;
						//checking if possible intercept/reflection point is within bounds of the wall
						if (((xIntercept <= x1 && xIntercept >= x2) || (xIntercept <= x2 && xIntercept >= x1)) && ((yIntercept <= y1 && yIntercept >= y2) || (yIntercept <= y2 && yIntercept >= y1)))
						{
							//checking if its the correction direction
							if (this->directionCheck(xIntercept, yIntercept))
							{
								//checking if a closer reflection was found
								double interceptDist = sqrt(pow(this->lazarBeam.getPosition().x - xIntercept, 2) + pow(this->lazarBeam.getPosition().y - yIntercept, 2));
								if (interceptDist < minInterceptDist)
								{
									//updating temp interception distance
									minInterceptDist = interceptDist;
									reflector = currentWall;
									//if its hitting the short edge of wall
									if (i % 2 == 1)
									{
										wallAngle = currentWall->getWall()->getRotation() + 90;
									}
									//if its hitting the long edge of wall
									else
									{
										wallAngle = currentWall->getWall()->getRotation();
									}
								}
							}
						}
					}
				}
			}
		}
		//setting up reflection member variables for next reflection
		this->lastReflection = reflector;
		this->nextReflectionDistance = minInterceptDist;
		this->calculateReflectionAngle(wallAngle);
		this->reflectionReady = true;
	}

	/**
	 * This function takes the possible xIntercept and yIntercept coordinate from
	 * the calculate reflection function and determines if the possible intercept/reflection
	 * point is in the direction that the lazar is travelling
	 *
	 * @return true if it is moving in the correct direction, else false
	 */
	bool directionCheck(double xIntercept, double yIntercept)
	{
		bool correctDirection = false;
		//moving down to the right
		if (this->lazarBeam.getRotation() <= 90)
		{
			if (xIntercept - this->lazarBeam.getPosition().x >= 0 && yIntercept - this->lazarBeam.getPosition().y >= 0)
			{
				correctDirection = true;
			}
		}
		//moving down to the left
		else if (this->lazarBeam.getRotation() <= 180)
		{
			if (xIntercept - this->lazarBeam.getPosition().x <= 0 && yIntercept - this->lazarBeam.getPosition().y >= 0)
			{
				correctDirection = true;
			}
		}
		//moving up to the left
		else if (this->lazarBeam.getRotation() <= 270)
		{
			if (xIntercept - this->lazarBeam.getPosition().x <= 0 && yIntercept - this->lazarBeam.getPosition().y <= 0)
			{
				correctDirection = true;
			}
		}
		//moving up to the right
		else
		{
			if (xIntercept - this->lazarBeam.getPosition().x >= 0 && yIntercept - this->lazarBeam.getPosition().y <= 0)
			{
				correctDirection = true;
			}
		}
		return correctDirection;
	}

	/**
	 * This function takes in the wall angle and finds the appropriate reflection angle
	 * It updates a member variable so no return is necessary
	 */
	void calculateReflectionAngle(int wallAngle)
	{
		int wallAngle2 = wallAngle + 180;
		if (wallAngle2 > 360)
			wallAngle2 -= 360;
		if (abs(wallAngle - this->lazarBeam.getRotation()) < abs(wallAngle2 - this->lazarBeam.getRotation()))
		{
			this->nextReflectionAngle = wallAngle + (wallAngle - this->lazarBeam.getRotation());
		}
		else
		{
			this->nextReflectionAngle = wallAngle2 + (wallAngle2 - this->lazarBeam.getRotation());
		}
	}

	sf::RectangleShape lazarBeam;
	Object::Wall* lastReflection;
	int velocity; //pixels/second
	bool reflectionReady;
	double nextReflectionDistance;
	int nextReflectionAngle;
};
}