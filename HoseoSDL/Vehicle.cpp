#include "Vehicle.h"
#include <math.h>
#include <iostream>

Vehicle::Vehicle(int x, int y) : maxSpeed(6), r(16), radius(0)
{
	pos = new Vector2D(x, y);
	vel = new Vector2D(0, 0);
	acc = new Vector2D(0, 0);
	force = new Vector2D(0, 0);
	rotate = new Vector2D(0, 0);

	r1 = new Vector2D(0, 0);
	r2 = new Vector2D(0, 0);
	r3 = new Vector2D(0, 0);

	Desir = new Vector2D(0, 0);

	Zero = new Vector2D(0, 0);
}

void Vehicle::draw(SDL_Renderer* renderer)
{
	filledTrigonRGBA(renderer, r1->getX() + pos->getX(), r1->getY() + pos->getY(),
		r2->getX() + pos->getX(), r2->getY() + pos->getY(),
		r3->getX() + pos->getX(), r3->getY() + pos->getY(),
		255, 255, 255, 255);
}

void Vehicle::update()
{
	*pos += *vel;
	vel->limit(maxSpeed);
	*vel += *acc;
	*acc *= 0;

	radius = atan2(vel->getY(), vel->getX());

	*r1 = Rotate(-r, -r / 2, radius);
	*r2 = Rotate(-r, r / 2, radius);
	*r3 = Rotate(r, 0, radius);
}

void Vehicle::applyForce(Vector2D* f)
{
	*acc += *f;
}

Vector2D Vehicle::Rotate(float x, float y, float r)
{
	rotate = new Vector2D(0, 0);

	rotate->setX((cos(r) * x) - (sin(r) * y));
	rotate->setY((sin(r) * x) + (cos(r) * y));

	return *rotate;
}

Vector2D Vehicle::arrive(Vector2D* target)
{
	*force = *target - *pos;
	double dist = force->length();

	if (dist > 0)
	{
		double speed = dist / (15 * 1.0);
		speed = std::min(speed, maxSpeed);
		*force *= speed;
		*force /= dist;
		Desir = force;

		*Desir -= *vel;
		return *Desir;
	}

	return *Zero;

}


