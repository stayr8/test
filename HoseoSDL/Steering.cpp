#include "Steering.h"
#include <math.h>
#include <iostream>

Steering::Steering(int x, int y) : maxSpeed(6), r(16), radius(0)
{
	pos = new Vector2D(x, y);
	vel = new Vector2D(0, 0);
	acc = new Vector2D(0, 0);
	force = new Vector2D(0, 0);
	rotate = new Vector2D(0, 0);

	r1 = new Vector2D(0, 0);
	r2 = new Vector2D(0, 0);
	r3 = new Vector2D(0, 0);

	DesirVel = new Vector2D(0, 0);

	Desir = new Vector2D(0, 0);

	Topursuer = new Vector2D(0, 0);
	VelTime = new Vector2D(0, 0);

	ToOb = new Vector2D(0, 0);

	Hiding = new Vector2D(0, 0);
	Distance = new Vector2D(0, 0);
	BaseHiding = new Vector2D(0, 0);

	Zero = new Vector2D(0, 0);
}

void Steering::draw(SDL_Renderer* renderer)
{
	filledTrigonRGBA(renderer, r1->getX() + pos->getX(), r1->getY() + pos->getY(),
		r2->getX() + pos->getX(), r2->getY() + pos->getY(),
		r3->getX() + pos->getX(), r3->getY() + pos->getY(),
		255, 255, 255, 255);
}

void Steering::update()
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

void Steering::applyForce(Vector2D* f)
{
	*acc += *f;
}

Vector2D Steering::Rotate(float x, float y, float r)
{
	rotate = new Vector2D(0, 0);

	rotate->setX((cos(r) * x) - (sin(r) * y));
	rotate->setY((sin(r) * x) + (cos(r) * y));

	return *rotate;
}

Vector2D Steering::arrive(Vector2D* target)
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

Vector2D Steering::Flee(Vector2D* Target)
{
	*DesirVel = *pos - *Target;
	DesirVel->normalize();
	*DesirVel *= maxSpeed;
	*DesirVel -= *vel;

	return *DesirVel;
}

Vector2D Steering::Evade(Vehicle* pursuer)
{
	*Topursuer = *pursuer->getPos() - *pos;
	double Th = 100.0;

	if (Topursuer->length() * Topursuer->length() > Th * Th)
	{
		return *Zero;
	}
	double Look = Topursuer->length() / (maxSpeed + pursuer->getmaxSpeed());

	*VelTime = *pursuer->getPos() + (*pursuer->getVel() * Look);

	return Flee(VelTime);
}

Vector2D Steering::GetHiding(Vector2D* posOb, double radiusOb, Vector2D* hunterOb)
{
	double Distance = 30.0;
	double DistAway = radiusOb + Distance;

	*ToOb = *posOb - *hunterOb;
	ToOb->normalize();
	*ToOb *= DistAway;
	*ToOb += *posOb;

	return *ToOb;
}

Vector2D Steering::Hide(Vehicle* hunter, std::vector<BaseGame*> obsclose)
{
	double DistToclose = 70.0;

	std::vector<BaseGame*>::const_iterator curOb = obsclose.begin();
	std::vector<BaseGame*>::const_iterator close;

	while (curOb != obsclose.end())
	{
		*Hiding = GetHiding((*curOb)->getPos(), (*curOb)->getRadius(), hunter->getPos());
		*Distance = *Hiding - *pos;
		double dist = Distance->length();
		if (dist < DistToclose)
		{
			DistToclose = dist;
			BaseHiding = Hiding;
			close = curOb;
		}
		++curOb;
	}

	if (DistToclose == 70)
	{
		return Evade(hunter);
	}

	return arrive(BaseHiding);

}
