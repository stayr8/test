#pragma once

#include "main.h"
#include "Vector2D.h"



class Vehicle
{

public:
	Vehicle(int x, int y);
	void draw(SDL_Renderer* renderer);
	void update();
	void applyForce(Vector2D* f);
	Vector2D Rotate(float x, float y, float r);
	Vector2D arrive(Vector2D* target);

	Vector2D* getPos() { return pos; }
	Vector2D* getVel() { return vel; }
	float getmaxSpeed() { return maxSpeed; }
	float getRadius() { return r; }
	
private:
	Vector2D* pos;
	Vector2D* vel;
	Vector2D* acc;
	Vector2D* force;
	Vector2D* rotate;

	Vector2D* r1;
	Vector2D* r2;
	Vector2D* r3;

	Vector2D* Desir;

	Vector2D* Zero;

	double maxSpeed;
	double r;
	double radius;

};
