#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <math.h>
#include <vector>
using namespace sf;
using namespace std;


////---global variables---////
const int sizex = 50;
const int sizey = 50;
const int rows = 25;
const int columns = 25;

class GameObject;
class WorldObjects;
struct mine;
struct grid
{
	RectangleShape  node;
	int				indexNumber;
};
struct Parameters
{
	Event						event;
	View						view;
	Vector2f					viewMoveDirection = Vector2f(0, 0);
	vector<shared_ptr<grid>>	Area;
	shared_ptr<grid>			markedgrid;
	int							typeFirstObject = 0;
	int						    selectionsize = 0;
	bool						building = false;
	string						currentBlueprint;
	bool					    buildingmenuActive = false;
	RectangleShape				buildProspect;
	Vector2f					mousePositionToWorld; // takes into account scrolling offset
	Vector2f					mousePositionToWindow; // doesn't
	int							frame = 0;
	int							gold = 0;
	shared_ptr<GameObject>	    focusGameObject = NULL;
	shared_ptr<mine>			focusMine = NULL;
};
struct tasks
{
	// task 0 input doesn't change any task;
	bool taskProspectBuilding = false; // 1
	bool taskBuilding = false; // 2
	bool taskCreateUnit = false; // 3
	bool taskMining = false; // 4 


	void resetTasks()
	{
		taskProspectBuilding = false;
		taskBuilding = false;
		taskCreateUnit = false;
		taskMining = false;
	}
};
struct mine
{
	RectangleShape goldMine;
	shared_ptr<GameObject> leaveAt = NULL;
	int goldLeft = 5000;
};
typedef shared_ptr<Parameters> P;

////---global functions---////
void					   setDefaultColors(RectangleShape & In)
{
	In.setFillColor(Color::Black);
	In.setOutlineColor(Color::Green);
	In.setOutlineThickness(1);


}
void					   changeColor(RectangleShape & In, const int col)
{

	if (col == 0)
	{
		In.setFillColor(Color::Black);
	}
	else if (col == 1)
	{
		In.setFillColor(Color::Red);
	}
	else if (col == 2)
	{
		In.setFillColor(Color::Blue);
	}


}
double					   pythagoras(const int x, const int y)
{
	return pow(pow(x, 2) + pow(y, 2), 0.5);
}
void				       setupGrid(vector<shared_ptr<grid>> & Area)
{
	shared_ptr<grid> NewArea;

	int tempNumber = 0; // index the grid
	for (int k = 0; k < rows; ++k)
	{
		for (int i = 0; i<columns; ++i)
		{

			NewArea = make_unique<grid>();


			NewArea->node.setPosition(sizex*i, sizey*k);

			NewArea->node.setSize(Vector2f(sizex, sizey));
			setDefaultColors(NewArea->node);
			NewArea->indexNumber = tempNumber++;
			Area.push_back(move(NewArea));
		}
	}
}
template<class T>
bool						IS_NULL(shared_ptr<T> arg)
{
	return (arg == NULL);
}
template<class A,class B>
A distance(const A a, const B b)
{
	// hmmm
}