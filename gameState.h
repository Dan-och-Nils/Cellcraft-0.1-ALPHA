#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <math.h>
#include <vector>
using namespace sf;
using namespace std;

class Gamestate // this class is for mousescroll and boxing and updates selection
{
public:
	Gamestate()
	{
		box.setSize(Vector2f(10, 10));
		box.setFillColor(Color::Transparent);
		box.setOutlineColor(Color::Green);
		box.setOutlineThickness(5);
	}
	~Gamestate() = default;

	void					setMousePosition(RenderWindow & window, P&p)
	{
		p->mousePositionToWindow = static_cast<Vector2f> (sf::Mouse::getPosition(window));
		p->mousePositionToWorld = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		p->viewMoveDirection = mouseScroll(p);
	}
	Vector2f				mouseScroll(P & parameters)
	{
		if (parameters->mousePositionToWindow.x <= 5)
		{
			parameters->view.move(Vector2f({ -scrollspeed, 0 }));
			return Vector2f(-scrollspeed, 0);
		}
		else if (parameters->mousePositionToWindow.x >= 1910)
		{
			parameters->view.move(Vector2f({ scrollspeed, 0 }));
			return Vector2f(scrollspeed, 0);
		}
		else if (parameters->mousePositionToWindow.y <= 5)
		{
			parameters->view.move(Vector2f({ 0, -scrollspeed }));
			return Vector2f(0, -scrollspeed);
		}
		else if (parameters->mousePositionToWindow.y >= 1000)
		{
			parameters->view.move(Vector2f({ 0, scrollspeed }));
			return Vector2f(0, scrollspeed);
		}
		else
		{
			return Vector2f(0, 0);
		}
	}

	inline RectangleShape   Box()
	{
		return box;
	}
	void				    setBoxing(const int val)
	{
		if (val == 0)
		{
			boxing = true;
			box.setSize(Vector2f(1, 1));
		}
		else if (val == 1)
		{
			boxing = false;
		}
	}
	bool					IsBoxing()
	{
		return boxing;
	}

	void					updateBoxingArea(P&p)
	{
		float x = box.getPosition().x;
		float y = box.getPosition().y;

		if (p->mousePositionToWorld.x - x > 0 && p->mousePositionToWorld.y - y > 0)
		{
			box.setSize(Vector2f(abs(p->mousePositionToWorld.x - x), abs(p->mousePositionToWorld.y - y)));
		}
		else if (p->mousePositionToWorld.x - x < 0 && p->mousePositionToWorld.y - y > 0)
		{
			box.setSize(Vector2f(-abs(p->mousePositionToWorld.x - x), abs(p->mousePositionToWorld.y - y)));
		}
		else if (p->mousePositionToWorld.x - x < 0 && p->mousePositionToWorld.y - y < 0)
		{
			box.setSize(Vector2f(-abs(p->mousePositionToWorld.x - x), -abs(p->mousePositionToWorld.y - y)));
		}
		else if (p->mousePositionToWorld.x - x > 0 && p->mousePositionToWorld.y - y < 0)
		{
			box.setSize(Vector2f(abs(p->mousePositionToWorld.x - x), -abs(p->mousePositionToWorld.y - y)));
		}
	}
	void					updateBoxPosition(P&p)
	{
		box.setPosition(p->mousePositionToWorld);
	}
	void					update(P&p, RenderWindow & window)
	{
		if (IsBoxing()) { updateBoxingArea(p); } //update box
		window.draw(box);
	}

protected:

	const float		scrollspeed = 10; // sidescrolling speed
	bool			boxing = false; //if we are creating a selection area
	RectangleShape	box; // the area of selection
};