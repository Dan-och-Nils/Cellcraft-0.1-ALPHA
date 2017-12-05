#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <math.h>
#include <vector>
#include "objects.h"

using namespace sf;
using namespace std;

class ObjectManager // this class is for maintaing all objects interactable and sending them orders 
{
protected:
	vector<shared_ptr<GameObject>>  allObjects;
	vector<shared_ptr<GameObject>>  selectedObjects;
	shared_ptr<GameObject>			objectCreator;
public:
	ObjectManager()
	{
		//initiates a Factory as a starting building and a Peon to build
		objectCreator = make_shared<Factory>(Vector2f(300, 300));
		allObjects.push_back(move(objectCreator));
		objectCreator = NULL;
		objectCreator = make_shared<Peon>(Vector2f(300, 800));
		allObjects.push_back(move(objectCreator));
	}
	~ObjectManager() = default;
	shared_ptr<GameObject> closestObject(const RectangleShape & compare, const int type)
	{
		shared_ptr<GameObject> temp = NULL;
		for (const auto & objectIn : allObjects)
		{
			if (objectIn != NULL)
			{

				double distance = 1000000;
				if (objectIn->getType() == type) // factory
				{
					RectangleShape id = objectIn->getIdentity();
					double xCenter = (id.getPosition().x + id.getSize().x / 2);
					double yCenter = (id.getPosition().y + id.getSize().y / 2);
					double CompareX = compare.getPosition().x + compare.getSize().x / 2;
					double CompareY = compare.getPosition().y + compare.getSize().y / 2;
					double newDistance = abs(pythagoras(double(CompareX - xCenter), double(CompareY - yCenter)));

					if (newDistance < distance)
					{
						temp = objectIn;
						distance = newDistance;
					}
				}
			}
		}
		if (temp != NULL)
		{
			return temp;
		}
		else
		{
			return NULL;
		}

	}
	void connectMines(WorldObjects & in) // run this in beginning and when making a new factory
	{
		for (auto & mines : in.getGoldMines())
		{
			if (mines != NULL)
			{
				mines->leaveAt = closestObject(mines->goldMine, 10); 
				mines->leaveAt->setFocusMine(mines); // this makes a cross connection, so that the factory sets this mine as the focus mine aswell
			}
		}
	}
	int  updateSelection(const FloatRect box)
	{
		int count = 0;
		selectedObjects.clear();
		for (auto & objectIn : allObjects)
		{
			if (objectIn != NULL)
				if (box.intersects(objectIn->getIdentity().getGlobalBounds()))
				{
					selectedObjects.push_back(objectIn);
					++count;
				}

		}
		return count;
	}
	int  first()
	{
		if (selectedObjects.empty())
		{
			return 0;
		}
		else
		{
			return selectedObjects.at(0)->getType();
		}

	}
	bool emptySelection()
	{
		return selectedObjects.empty();
	}
	void issueOrderFirst(P&p, const int orderId, string info)
	{
		if (!selectedObjects.empty())
		{
			selectedObjects.at(0)->newOrder(p, orderId, info);
		}
	}
	void issueOrderAll(P&p, const int orderId, string info)
	{
		for (auto & objectIn : selectedObjects)
		{
			if (objectIn != NULL)
			{
				objectIn->newOrder(p, orderId, info);
			}

		}
	}
	void update(P&p, RenderWindow & window, WorldObjects & worldObjects)
	{
		vector<shared_ptr<GameObject>>  newBuildings;
		for (auto & objectIn : allObjects)
		{
			if (objectIn != NULL)
			{
				objectIn->draw(window);
				if (objectIn->update(p)) // this gives a true statement back if something needs to happen as a result of their status, as of now only building something
				{
					int t = (objectIn->getType());
					
					if(t == 1)
					{
						string bluePrint = objectIn->getConstructionBluePrint();
						if (bluePrint == "Factory")
						{
							objectCreator = make_shared<Factory>(objectIn->getIdentity().getPosition());
							newBuildings.push_back(move(objectCreator));
						}
						else if (bluePrint == "Barrack")
						{
							objectCreator = make_shared<Barrack>(objectIn->getIdentity().getPosition());
							newBuildings.push_back(move(objectCreator));
						}
						else
						{
							cout << " couldnt find the blueprint : " << bluePrint << endl;
						}
					}
					else if (t == 10)
					{
						string bluePrint = objectIn->getConstructionBluePrint();
						if (bluePrint == "Peon")
						{

							objectCreator = make_shared<Peon>(objectIn->getIdentity().getPosition(), objectIn->getMoveTo());
							newBuildings.push_back(move(objectCreator));
						}
						else
						{
							cout << " couldnt find the blueprint : " << bluePrint << endl;
						}

					}
					else if (t == 12)
					{
						cout << "create something from Barrack" << endl;
						string bluePrint = objectIn->getConstructionBluePrint();
						if (bluePrint == "Warrior")
						{
							objectCreator = make_shared<Warrior>(objectIn->getIdentity().getPosition(), objectIn->getMoveTo());
							newBuildings.push_back(move(objectCreator));
						}
						else if (bluePrint == "Mage")
						{

							objectCreator = make_shared<Mage>(objectIn->getIdentity().getPosition(), objectIn->getMoveTo());
							newBuildings.push_back(move(objectCreator));
						}
						else
						{
							cout << " couldnt find the blueprint : " << bluePrint << endl;
						}
					}
					
				}
			}
		}
		for (auto & objectIn : newBuildings)
		{
			allObjects.push_back(move(objectIn));
		}
	}

};