#pragma once

#include "main.h"


class Walker 
{
  
public:
  Walker(int x, int y);
  void draw(SDL_Renderer* renderer);
  void update();
};