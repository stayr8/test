#pragma once

#include "main.h"
#include "Vector2D.h"
#include "Vehicle.h"
#include "BaseGame.h"
#include <vector>


class Steering
{

public:
	Steering(int x, int y);
	void draw(SDL_Renderer* renderer);
	void update();
	void applyForce(Vector2D* f);
	Vector2D Rotate(float x, float y, float r);
	Vector2D arrive(Vector2D* target);
	Vector2D Flee(Vector2D* Target);
	Vector2D Evade(Vehicle* pursuer);
	Vector2D GetHiding(Vector2D* posOb, double radiusOb, Vector2D* hunterOb);
	Vector2D Hide(Vehicle* hunter, std::vector<BaseGame*>obsclose);
	void edges();

private:
	Vector2D* pos;
	Vector2D* vel;
	Vector2D* acc;
	Vector2D* force;
	Vector2D* rotate;

	Vector2D* r1;
	Vector2D* r2;
	Vector2D* r3;

	Vector2D* DesirVel;

	Vector2D* Desir;

	Vector2D* Topursuer;
	Vector2D* VelTime;

	Vector2D* ToOb;

	Vector2D* Hiding;
	Vector2D* Distance;
	Vector2D* BaseHiding;

	Vector2D* Zero;

	double maxSpeed;
	double r;
	double radius;


};
