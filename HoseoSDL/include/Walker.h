#pragma once

#include "main.h"
#include "Vector2D.h"
#include "Vehicle.h"
#include "BaseGame.h"
#include "Steering.h"
#include <vector>



class Walker 
{
 
public:
  Walker();
  void draw(SDL_Renderer* renderer);
  void update();

private:
	Vector2D* force;
	Vector2D* string;
	Vector2D* target;
	Vector2D* force2;

	Vehicle* vehicle;
	Steering* steering;
	std::vector<BaseGame*> m_BG;
};