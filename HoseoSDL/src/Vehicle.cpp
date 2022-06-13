#define WIDTH (600)
#define HEIGHT (400)

#include "Vehicle.h"
#include <math.h>
#include <iostream>

Vehicle::Vehicle(int x, int y) : maxSpeed(6), r(16), radius(0), m_dDBoxLength(0)
, speed(0)
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
	ToOb = new Vector2D(0, 0);
	Hiding = new Vector2D(0, 0);

	Obs = new BaseGame(0, 0, 0);
	Local = new Vector2D(0, 0);

	steeringForce = new Vector2D(0, 0);
}

void Vehicle::draw(SDL_Renderer* renderer)
{
	filledTrigonRGBA(renderer, r1->getX() + pos->getX(), r1->getY() + pos->getY(),
		r2->getX() + pos->getX(), r2->getY() + pos->getY(),
		r3->getX() + pos->getX(), r3->getY() + pos->getY(),
		255, 255, 255, 255);
	boxRGBA(renderer,  pos->getX() - r,  pos->getY() - r/2,
		 pos->getX() + r + m_dDBoxLength,  pos->getY() + r / 2, 0, 0, 255, 100);
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

	edges();
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

/**/
Vector2D Vehicle::arrive(Vector2D* target)
{
	*force = *target - *pos;
	double dist = force->length();

	if (dist > 0)
	{
		speed = dist / (15 * 1.0);
		speed = std::min(speed, maxSpeed);
		*force *= speed;
		*force /= dist;
		*Desir = *force;

		*Desir -= *vel;
		return *Desir;
	}

	return *Zero;

}

Vector2D Vehicle::GetHiding(Vector2D* posOb, double radiusOb, Vector2D* hunterOb)
{
	double Distance = 30.0;
	double DistAway = radiusOb + Distance;

	*ToOb = *posOb - *hunterOb;
	ToOb->normalize();
	*ToOb *= DistAway;
	*ToOb += *posOb;

	return *ToOb;
}

// 장애물 피하기이지만 실패했습니다.
Vector2D Vehicle::Avoidance(std::vector<BaseGame*> obstacles)
{
	m_dDBoxLength = 40 + ((speed / maxSpeed) * 40);

	double DistToclose = 70;

	std::vector<BaseGame*>::const_iterator curOb = obstacles.begin();

	while (curOb != obstacles.end())
	{
		*Hiding = GetHiding((*curOb)->getPos(), (*curOb)->getRadius(), pos);
		if (Hiding->getX() >= 0)
		{
			double ExpandedRadius = (*curOb)->getRadius() + r;

			if (fabs(Hiding->getY()) < ExpandedRadius)
			{
				double cX = Hiding->getX();
				double cY = Hiding->getY();

				double sqrtPart = sqrt(ExpandedRadius * ExpandedRadius - cY * cY);
				double ip = cX - sqrtPart;

				if (ip <= 0.0)
				{
					ip = cX + sqrtPart;
				}

				if (ip < DistToclose)
				{
					DistToclose = ip;
					Obs = (*curOb);
					*Local = *Hiding;
				}
			}
		}
		++curOb;
	}

	if (Obs)
	{
		double multiplier = 1.0 + (m_dDBoxLength - (Local->getX() / m_dDBoxLength));

		steeringForce->setY(Obs->getRadius() - Local->getY() * multiplier);

		double BrakingWeight = 0.2f;

		steeringForce->setX(Obs->getRadius() - Local->getX() * multiplier);

	}
	return *steeringForce;

}

void Vehicle::edges()
{
	if (pos->getX() > WIDTH + r)
	{
		pos->setX(r);
	}
	else if (pos->getX() < -r)
	{
		pos->setX(WIDTH - r);
	}

	if (pos->getY() > HEIGHT + r)
	{
		pos->setY(r);
	}
	else if (pos->getY() < -r)
	{
		pos->setY(HEIGHT - r);
	}
}

