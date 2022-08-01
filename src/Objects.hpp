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

private:
	int selected;				 //this is updated whenever the object is selected or deselected  0 = no, 1 = selected, 2 = stretch1, 3 = stretch2, 4 = rotater
	sf::RectangleShape wall;	 //main wall object
	sf::CircleShape mover;		 //movement object - only drawn while editing
	sf::CircleShape transformer; //transformation object - only drawn while selected
};

class LazarGun
{
public:
	LazarGun(int x = 350, int y = 350)
	{
		this->selected = false;
		this->lazarGun = sf::RectangleShape(sf::Vector2f(36, 24));
		this->lazarGun.setFillColor(sf::Color::White);
		this->lazarGun.setOutlineColor(sf::Color::Red);
		this->lazarGun.setOrigin(18, 12);
		this->transformer = sf::CircleShape(12);
		this->transformer.setOrigin(12, 12);
		this->lazarGun.setPosition(x, y);
	}

	sf::RectangleShape* getLazarGun()
	{
		return &this->lazarGun;
	}

	sf::CircleShape* getTransformer()
	{
		return &this->transformer;
	}

	/**
	 * This function draws the lazargun object and the transformer if selected
	 */
	void draw(sf::RenderWindow* window)
	{
		window->draw(this->lazarGun);
		if (selected)
		{
			this->transformer.setPosition(this->lazarGun.getPosition().x + (((this->lazarGun.getSize().x / 2) + 20) * cos(this->lazarGun.getRotation() * PI / 180)), this->lazarGun.getPosition().y + (((this->lazarGun.getSize().x / 2) + 20) * sin(this->lazarGun.getRotation() * PI / 180)));
			this->transformer.setRotation(this->lazarGun.getRotation() + 135);
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
			this->lazarGun.setPosition(mouse->x - offset->x, mouse->y - offset->y);
		}
		//Transform Selection -> rotate object
		else if (this->selected == 2)
		{
			double newRotation = atan((this->lazarGun.getPosition().y - mouse->y) / (this->lazarGun.getPosition().x - mouse->x)) / PI * 180;
			if (mouse->x - this->lazarGun.getPosition().x <= 0)
			{
				newRotation += 180;
			}
			if (newRotation > 360)
				newRotation -= 360;
			if (newRotation < 0)
				newRotation += 360;
			this->lazarGun.setRotation(newRotation);
		}
	}

	/**
	 * This function is called to ensure the lazargun always remains accessable in the play area
	 */
	void fixPos()
	{
		if (this->lazarGun.getPosition().x > 900)
			this->lazarGun.setPosition(900, this->lazarGun.getPosition().y);
		if (this->lazarGun.getPosition().y > 700)
			this->lazarGun.setPosition(this->lazarGun.getPosition().x, 700);
		if (this->lazarGun.getPosition().x < 0)
			this->lazarGun.setPosition(0, this->lazarGun.getPosition().y);
		if (this->lazarGun.getPosition().y < 0)
			this->lazarGun.setPosition(this->lazarGun.getPosition().x, 0);
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
	 * this function removes the outline and deselects
	 */
	void deselect()
	{
		this->lazarGun.setOutlineThickness(0);
		this->selected = 0;
	}

	/**
	 * This function updates the selection and lets the move function know that the transformer has been selected
	 */
	void selectTransformer()
	{
		this->selected = 2;
	}

	/**
	 * this function is used to determine the selection state
	 */
	bool isSelected()
	{
		return this->selected;
	}

private:
	int selected;				 //this is updated whenever the object is selected or deselected  0 = no, 1 = selected, 2 = stretch1, 3 = stretch2, 4 = rotater
	sf::RectangleShape lazarGun; //main wall object
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
	Lazar(int rotation = 0, int x = 350, int y = 350, int newVelocity = 100)
	{
		this->velocity = newVelocity;
		this->lazarBeam = sf::RectangleShape(sf::Vector2f(20, 5));
		this->lazarBeam.setOrigin(10, 2.5);
		this->lazarBeam.setPosition(x, y);
		this->lazarBeam.setFillColor(sf::Color::White);
		this->lazarBeam.setRotation(rotation);
		this->reflectionCoolDown = 0;
		this->recalculateReflection = false;
		this->nextReflectionDistance = 1000;
	}

	/**
	 * This function takes care of all movement - using velocity and time passage to determine speed regardless of frames per second
	 * This function additionally checks locations of each wall and takes care of reflections
	 */
	void update(double timePassed, std::list<Object::Wall*>* wallList)
	{
		//updating standard movement parameters
		double nextDistance = (double)this->velocity * timePassed;
		double nextX = this->lazarBeam.getPosition().x + cos(this->lazarBeam.getRotation() * PI / 180) * 100;
		double nextY = this->lazarBeam.getPosition().y + sin(this->lazarBeam.getRotation() * PI / 180) * 100;

		//if its time to calculate the next reflection
		if (reflectionCoolDown == 0)
		{
			this->reflectionCoolDown = 10;
			this->recalculateReflection = false;
			//values used for reflection calculation
			double minInterceptDist = -1;
			int wallAngle = 0;

			//lazar "line" = a1x + b1y = c1
			double a1 = this->lazarBeam.getPosition().y - nextY;
			double b1 = nextX - this->lazarBeam.getPosition().x;
			double c1 = a1 * nextX + b1 * nextY;

			//iterating through each wall finding the closest wall on course to intercept/reflect
			for (std::list<Object::Wall*>::iterator wallIterator = wallList->begin(); wallIterator != wallList->end(); wallIterator++)
			{
				Object::Wall* currentWall = *wallIterator;
				//each wall has 2 sides
				for (int i = 0; i < 2; i++)
				{
					int p1 = 2 * i;
					int p2 = (2 * i) + 1;
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
						double xIntercept = (b2 * c1 - b1 * c2) / determinant;
						//checking if possible xIntercept is within bounds of the wall
						if ((xIntercept <= x1 && xIntercept >= x2) || (xIntercept <= x2 && xIntercept >= x1))
						{

							//finding the rest of the intercept/reflection location details
							double yIntercept = (a1 * c2 - a2 * c1) / determinant;
							double interceptDist = sqrt(pow(this->lazarBeam.getPosition().x - xIntercept, 2) + pow(this->lazarBeam.getPosition().y - yIntercept, 2));
							//making sure the interception is the right direction
							bool correctDirection = false;
							//lazar moving right
							if (nextX - this->lazarBeam.getPosition().x >= 0)
							{
								//lazar moving down
								if (nextY - this->lazarBeam.getPosition().y >= 0)
								{
									if (xIntercept - this->lazarBeam.getPosition().x >= 0 && yIntercept - this->lazarBeam.getPosition().y >= 0)
									{
										correctDirection = true;
									}
								}
								//lazar moving up
								else
								{
									if (xIntercept - this->lazarBeam.getPosition().x >= 0 && yIntercept - this->lazarBeam.getPosition().y < 0)
									{
										correctDirection = true;
									}
								}
							}
							//lazar moving left
							else
							{
								//lazar moving down
								if (nextY - this->lazarBeam.getPosition().y >= 0)
								{
									if (xIntercept - this->lazarBeam.getPosition().x < 0 && yIntercept - this->lazarBeam.getPosition().y >= 0)
									{
										correctDirection = true;
									}
								}
								//lazar moving up
								else
								{
									if (xIntercept - this->lazarBeam.getPosition().x < 0 && yIntercept - this->lazarBeam.getPosition().y < 0)
									{
										correctDirection = true;
									}
								}
							}
							//checking if its the correction direction and if a closer reflection/interception was found
							if (correctDirection)
							{
								if (interceptDist < minInterceptDist || minInterceptDist == -1)
								{
									//updating temp interception distance
									minInterceptDist = interceptDist;
									wallAngle = currentWall->getWall()->getRotation();
								}
							}
						}
					}
				}
			}
			//if and intercept/reflection was found
			if (minInterceptDist != -1)
			{
				this->nextReflectionDistance = minInterceptDist;
				int wallAngle1 = wallAngle;
				int wallAngle2 = wallAngle1 + 180;
				if (wallAngle2 > 360)
					wallAngle2 -= 360;
				if (abs(wallAngle1 - this->lazarBeam.getRotation()) < abs(wallAngle2 - this->lazarBeam.getRotation()))
				{
					this->nextReflectionAngle = wallAngle1 + (wallAngle1 - this->lazarBeam.getRotation());
				}
				else
				{
					this->nextReflectionAngle = wallAngle2 + (wallAngle2 - this->lazarBeam.getRotation());
				}
			}
			//otherwise the lazar will continue until off the map
			else
			{

				//the play area is 990 pixels at the diagonal so this "next reflection" will never happen
				this->nextReflectionDistance = 1000;
			}
			//calculated next reflection, so no need to do this again until after next reflection
		}
		//if the reflection will happen in this frame
		if (nextDistance >= this->nextReflectionDistance)
		{
			//move lazar to the reflection point
			this->lazarBeam.setPosition(this->lazarBeam.getPosition().x + cos(this->lazarBeam.getRotation() * PI / 180) * this->nextReflectionDistance, this->lazarBeam.getPosition().y + sin(this->lazarBeam.getRotation() * PI / 180) * this->nextReflectionDistance);
			//reflect and continue moving the rest of the appropriate distance
			this->lazarBeam.setRotation(this->nextReflectionAngle);
			this->lazarBeam.setPosition(this->lazarBeam.getPosition().x + cos(this->lazarBeam.getRotation() * PI / 180) * (nextDistance - this->nextReflectionDistance), this->lazarBeam.getPosition().y + sin(this->lazarBeam.getRotation() * PI / 180) * (nextDistance - this->nextReflectionDistance));
			this->recalculateReflection = true;
			this->nextReflectionDistance = 1000;
		}
		else
		{
			if (recalculateReflection)
			{
				this->reflectionCoolDown--;
			}
			this->lazarBeam.setPosition(this->lazarBeam.getPosition().x + cos(this->lazarBeam.getRotation() * PI / 180) * nextDistance, this->lazarBeam.getPosition().y + sin(this->lazarBeam.getRotation() * PI / 180) * nextDistance);
			this->nextReflectionDistance = this->nextReflectionDistance - nextDistance;
		}
	}

	sf::RectangleShape* getLazar()
	{
		return &this->lazarBeam;
	}

private:
	sf::RectangleShape lazarBeam;
	int velocity; //pixels/second
	bool recalculateReflection;
	int reflectionCoolDown;
	double nextReflectionDistance;
	int nextReflectionAngle;
};

}