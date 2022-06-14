#include "Walker.h"
#include <random>
#include "InputHandler.h"


Walker::Walker()
{
	force = new Vector2D(0, 0);
	string = new Vector2D(0, 0);

	vehicle = new Vehicle(100, 200);
	steering = new Steering(200, 200);
	target = new Vector2D(0, 0);
	force2 = new Vector2D(1, 0);

	m_BG.push_back(new BaseGame(100, 100, 25));
	m_BG.push_back(new BaseGame(200, 200, 25));
	m_BG.push_back(new BaseGame(300, 200, 25));
	m_BG.push_back(new BaseGame(300, 300, 25));
	m_BG.push_back(new BaseGame(400, 400, 25));
}

void Walker::update() 
{

	for (int i = 0; i != m_BG.size(); i++)
	{
		m_BG[i]->update();
	}

  target = TheInputHandler::Instance()->getMousePosition(); // �ϱ�
  *force = vehicle->arrive(target);// �ϱ�
  //*force = vehicle->Avoidance(m_BG); // �ֹ� �ϱ�
  vehicle->applyForce(force);// �ϱ�
 //vehicle->applyForce(force2); // �ֹ� �ϱ�
  vehicle->update();

  *string = steering->Hide(vehicle, m_BG);
  // *string = steering->arrive(target); 

  steering->applyForce(string);
  steering->update();

  

}

void Walker::draw(SDL_Renderer* renderer)
{
	for (int i = 0; i != m_BG.size(); i++)
	{
		m_BG[i]->draw(renderer);
	}

	vehicle->draw(renderer);
	steering->draw(renderer);
}
