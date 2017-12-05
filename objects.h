#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <math.h>
#include <vector>
#include "global.h"
using namespace sf;
using namespace std;

class Factory;
class WorldObjects
{

public:
	WorldObjects()
	{
		shared_ptr<mine> newMine = make_unique<mine>();
		newMine->goldMine.setSize(Vector2f(50, 50));
		newMine->goldMine.setPosition(Vector2f(500, 500));
		newMine->goldMine.setFillColor(Color::Yellow);
		goldMines.push_back(move(newMine));

	}
	~WorldObjects() {}
	void draw(RenderWindow & window)
	{
		for (const auto & mineIn : goldMines)
		{
			window.draw(mineIn->goldMine);
		}

	}
	void deductGold()
	{
		//gameMine.goldLeft -= 10;
	}
	bool intersectMine(P & p)
	{
		for (auto & mineIn : goldMines)
		{
			if (mineIn != NULL)
			{
				if (mineIn->goldMine.getGlobalBounds().contains(p->mousePositionToWorld))
				{
					p->focusMine = mineIn;
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		//return gameMine.goldMine.getGlobalBounds().contains(mPos);
	}
	vector<shared_ptr<mine>>& getGoldMines()
	{
		return goldMines;
	}


private:
	vector<shared_ptr<mine>> goldMines;
};
class GameObject
{
public:
	GameObject()
	{
		mission = make_unique<tasks>();
	}
	~GameObject() {}
	void						 setMissionType(const int missionType) // use 1 for prospectbuilding , 2 for building, 3 for creating unit, 4 for going to mine, 5 for mining
	{
		mission->resetTasks();
		if (missionType == 1)
		{
			mission->taskProspectBuilding = true;
		}
		else if (missionType == 2)
		{
			mission->taskBuilding = true;
		}
		else if (missionType == 3)
		{
			mission->taskCreateUnit = true;
		}
		else if (missionType == 4)
		{
			mission->taskMining = true;
		}
	}
	int						     getMissionType()
	{
		if (mission->taskProspectBuilding)
		{
			return 1;
		}
		else if (mission->taskBuilding)
		{
			return 2;
		}
		else if (mission->taskCreateUnit)
		{
			return 3;
		}
		else if (mission->taskMining)
		{
			return  4;
		}
	}
	void					     draw(RenderWindow & window)
	{
		window.draw(thisID);
		window.draw(timerBar);
		window.draw(healthbar);
	}
	inline int					 getType()
	{
		return type;
	}
	inline RectangleShape        getIdentity()
	{
		return thisID;
	}
	virtual void				 newOrder(P&p, const int orderId, string info) = 0;
	virtual bool				 update(P&p) = 0;
	string						 getConstructionBluePrint()
	{
		return constructionPlan;
	}
	void						 setupBars()
	{
		int xSize = thisID.getSize().x;
		int ySize = thisID.getSize().y;
		int xPos = thisID.getPosition().x;
		int yPos = thisID.getPosition().y;

		timerBar.setFillColor(Color::Blue);
		timerBar.setOutlineColor(Color::Magenta);
		timerBar.setOutlineThickness(5);
		timerBar.setPosition(Vector2f(xPos - xSize / 2, yPos + ySize*1.5));
		timerBar.setSize(Vector2f(xSize * 2, ySize*0.2));


		healthbar.setFillColor(Color::Blue);
		healthbar.setOutlineColor(Color::Magenta);
		healthbar.setOutlineThickness(5);
		healthbar.setPosition(Vector2f(xPos - xSize / 2, yPos - ySize / 2));
		healthbar.setSize(Vector2f(xSize * 2, ySize*0.2));
	}
	virtual Vector2f			 getMoveTo() = 0;
	virtual void				 setMoveTo(const Vector2f & where) = 0;
	virtual void				 setMoveTo() = 0;
	shared_ptr<mine>			 getFocusMine()
	{
		return focusMine;
	}
	void					     setFocusMine(shared_ptr<mine> in)
	{
		focusMine = in;
	}
	

protected:
	unsigned short					attackStrength;
	unsigned short					healthPool;
	int							    type = 0;
	RectangleShape				    thisID;
	shared_ptr<tasks>				mission = NULL;
	string				            constructionPlan = "";
	int								timer = 0;
	RectangleShape					timerBar;
	RectangleShape					healthbar;
	shared_ptr<mine>				focusMine = NULL;
	shared_ptr<GameObject>			focusGameObject = NULL;
};
class Building : public GameObject
{
public:
	Building() { mission = make_unique<tasks>(); }
	~Building() = default;
	Building(Vector2f & where)
	{
		mission = make_unique<tasks>();
		thisID.setSize(Vector2f(sizex, sizey));
		thisID.setPosition(Vector2f(where));
	}
	Building(const Vector2f & where)
	{
		mission = make_unique<tasks>();
		thisID.setSize(Vector2f(sizex, sizey));
		thisID.setPosition(Vector2f(where));
	}
	virtual void				 newOrder(P&p, const int orderId, string info) = 0;
	virtual bool				 update(P&p) = 0;
	virtual Vector2f			 getMoveTo() override
	{
		return rallyTo;
	}
	virtual void				 setMoveTo(const Vector2f & where) override
	{
		rallyTo = where;
	}
	virtual void				 setMoveTo() override
	{
		if (focusGameObject != NULL)
			rallyTo = focusGameObject->getIdentity().getPosition();
	}
protected:

	Vector2f	   rallyTo = Vector2f(thisID.getPosition().x, thisID.getPosition().y);
	unsigned short healthPool;

};
class Factory : public Building
{
public:
	Factory() {
		type = 10;
		thisID.setFillColor(Color::Red);
		thisID.setSize(Vector2f(sizex, sizey));
		mission = make_unique<tasks>();
	}
	Factory(Vector2f & where)
	{
		type = 10;
		thisID.setFillColor(Color::Red);
		mission = make_unique<tasks>();
		thisID.setSize(Vector2f(sizex, sizey));
		thisID.setPosition(Vector2f(where));
		setupBars();
	}

	Factory(Vector2f & where, WorldObjects & goldMines)
	{
		type = 10;
		thisID.setFillColor(Color::Red);
		mission = make_unique<tasks>();
		thisID.setSize(Vector2f(sizex, sizey));
		thisID.setPosition(Vector2f(where));
		setupBars();
	}


	Factory(const Vector2f where)
	{
		type = 10;
		thisID.setFillColor(Color::Red);
		mission = make_unique<tasks>();
		thisID.setSize(Vector2f(sizex, sizey));
		thisID.setPosition(Vector2f(where));
		setupBars();
	}
	Factory(const Vector2f where, WorldObjects goldMines)
	{
		type = 10;
		thisID.setFillColor(Color::Red);
		mission = make_unique<tasks>();
		thisID.setSize(Vector2f(sizex, sizey));
		thisID.setPosition(Vector2f(where));
		setupBars();
		setDesignatedMine(goldMines);
	}


	virtual void			     newOrder(P&p, const int orderId, string info) override
	{
		if (orderId == 17)
		{
			if (!mission->taskCreateUnit)  // cause trouble, no order stack
			{
				timer = 10;
				setMissionType(3);
				constructionPlan = info;
			}
		}
		else if (orderId == 1)
		{
			rallyTo = p->mousePositionToWorld;
		}

	}
	virtual bool				 update(P&p) override
	{
		if (mission->taskCreateUnit) // we aint moving if we're tasked to build something
		{
			if ((p->frame % 30) == 0)
			{
				--timer;
				timerBar.setSize(Vector2f(timerBar.getSize().x*0.9, timerBar.getSize().y));
			}
			if (timer != 0)
			{
				return false;
			}
			else // we finished the creation!
			{
				cout << "Create it" << endl;
				timerBar.setSize(Vector2f(thisID.getSize().x * 2, timerBar.getSize().y));
				mission->resetTasks();
				return true;
			}

			return false;
		}
		else
		{
			return false;
		}
	}
	void			             setDesignatedMine(WorldObjects & goldMines)
	{
		shared_ptr<mine> temp = NULL;
		for (auto & objectIn : goldMines.getGoldMines())
		{
			if (objectIn != NULL)
			{
				double distance = 1000000;

				RectangleShape id = objectIn->goldMine;
				double xCenter = (id.getPosition().x + id.getSize().x / 2);
				double yCenter = (id.getPosition().y + id.getSize().y / 2);
				double CompareX = thisID.getPosition().x + thisID.getSize().x / 2;
				double CompareY = thisID.getPosition().y + thisID.getSize().y / 2;
				double newDistance = pythagoras(double(CompareX - xCenter), double(CompareY - yCenter));
				if (newDistance < distance)
				{
					temp = objectIn;
					distance = newDistance;
				}
			}
		}
		focusMine = temp;
	}
	~Factory() {};

};
class Barrack : public Building
{
public:
	Barrack() {
		type = 12;
		thisID.setFillColor(Color(166,	149,80));
		thisID.setSize(Vector2f(sizex, sizey));
		mission = make_unique<tasks>();
	}
	Barrack(Vector2f & where)
	{
		type = 12;
		thisID.setFillColor(Color(166, 149, 80));
		mission = make_unique<tasks>();
		thisID.setSize(Vector2f(sizex, sizey));
		thisID.setPosition(Vector2f(where));
		setupBars();
	}
	Barrack(const Vector2f where)
	{
		type = 12;
		thisID.setFillColor(Color(166, 149, 80));
		mission = make_unique<tasks>();
		thisID.setSize(Vector2f(sizex, sizey));
		thisID.setPosition(Vector2f(where));
		setupBars();
	}

	virtual void			     newOrder(P&p, const int orderId, string info) override
	{
		if (orderId == 17) // P
		{
			if (!mission->taskCreateUnit)  // cause trouble, no order stack
			{
				timer = 10;
				setMissionType(3);
				constructionPlan = info;
			}
		}
		else if (orderId == 1)
		{
			rallyTo = p->mousePositionToWorld;
		}
		else if (orderId == 25) // W(arrior)
		{
			if (!mission->taskCreateUnit)  // cause trouble, no order stack
			{
				timer = 3;
				setMissionType(3);
				constructionPlan = info;
			}
		}
		else if (orderId == 14) // M(age)
		{
			cout << " create mage";
			if (!mission->taskCreateUnit)  // cause trouble, no order stack
			{
				timer = 5;
				setMissionType(3);
				constructionPlan = info;
			}
		}

	}
	virtual bool				 update(P&p) override
	{
		if (mission->taskCreateUnit) // we aint moving if we're tasked to build something
		{
			if ((p->frame % 30) == 0)
			{
				--timer;
				timerBar.setSize(Vector2f(timerBar.getSize().x*0.9, timerBar.getSize().y));
			}
			if (timer != 0)
			{
				return false;
			}
			else // we finished the creation!
			{
				cout << "Create it" << endl;
				timerBar.setSize(Vector2f(thisID.getSize().x * 2, timerBar.getSize().y));
				mission->resetTasks();
				return true;
			}

			return false;
		}
		else
		{
			return false;
		}
	}

	~Barrack() {};

};
class Armory : public Building
{
public:
	Armory()
	{
		type = 11;
		thisID.setFillColor(Color::Cyan);
		thisID.setSize(Vector2f(sizex, sizey));
		mission = make_unique<tasks>();
	}
	Armory(const Vector2f where)
	{
		type = 11;
		thisID.setFillColor(Color::Cyan);
		thisID.setSize(Vector2f(sizex, sizey));
		mission = make_unique<tasks>();
		thisID.setPosition(Vector2f(where));
	}
	Armory(Vector2f & where) {
		type = 11;
		thisID.setFillColor(Color::Cyan);
		thisID.setSize(Vector2f(sizex, sizey));
		thisID.setPosition(Vector2f(sizex * 10, sizey * 5));
		mission = make_unique<tasks>();
	}
	~Armory() {};

	virtual void			     newOrder(P&p, const int orderId, string info) override
	{

	}
	virtual bool				 update(P&p) override
	{
		return 0;
	}
};
class Unit : public GameObject
{
public:
	Unit() {
		mission = make_unique<tasks>();
	}
	~Unit() = default;

	virtual void newOrder(P&p, const int orderId, string info) = 0;
	virtual bool				 update(P&p) = 0;
	virtual void			     move(P & p)
	{
		if (goTo != NULL)
		{
			deltax = thisID.getPosition().x - goTo->x;
			deltay = thisID.getPosition().y - goTo->y;
			z = pythagoras(deltax, deltay);
			if (z > 1)
			{
				thisID.move((-deltax / z)*moveSpeed, (-deltay / z)*moveSpeed);
				timerBar.move((-deltax / z)*moveSpeed, (-deltay / z)*moveSpeed);
				healthbar.move((-deltax / z)*moveSpeed, (-deltay / z)*moveSpeed);
			}
			else // this is practically saying we've arrived!
			{
				goTo = NULL;
			}
		}
	}
	virtual Vector2f			 getMoveTo() override
	{
		return *goTo;
	}
	virtual void				 setMoveTo(const Vector2f & where) override
	{
		goTo = make_unique<Vector2f>(where);
	}
	virtual void				 setMoveTo() override
	{
		goTo = make_unique<Vector2f>(focusGameObject->getIdentity().getPosition());
	}
		
protected:
	int				           attackPower;
	//movement variables
	shared_ptr<Vector2f>	   goTo = NULL;
	double	deltax;
	double	deltay;
	double  z;
	int	    moveSpeed = 2;


	//movement variables
};
class Peon : public Unit
{
public:
	Peon()
	{
		type = 1;
		attackStrength = 1;
		healthPool = 10;
		thisID.setFillColor(Color::Cyan);
		thisID.setSize(Vector2f(sizex, sizey));
		mission = make_unique<tasks>();

	}
	Peon(Vector2f & where)
	{
		type = 1;
		attackStrength = 1;
		healthPool = 10;
		thisID.setFillColor(Color::Cyan);
		thisID.setSize(Vector2f(sizex, sizey));
		thisID.setPosition(where);
		mission = make_unique<tasks>();
		setupBars();
	}
	Peon(Vector2f & where, Vector2f & moveTo)
	{
		type = 1;
		attackStrength = 1;
		healthPool = 10;
		thisID.setFillColor(Color::Cyan);
		thisID.setSize(Vector2f(sizex, sizey));
		thisID.setPosition(where);
		mission = make_unique<tasks>();
		setupBars();

		setMoveTo(moveTo);
	}
	Peon(const Vector2f where, Vector2f & moveTo)
	{
		type = 1;
		attackStrength = 1;
		healthPool = 10;
		thisID.setFillColor(Color::Cyan);
		thisID.setSize(Vector2f(sizex, sizey));
		thisID.setPosition(where);
		mission = make_unique<tasks>();
		setupBars();
		setMoveTo(moveTo);
	}
	~Peon() {}

	virtual void			     newOrder(P&p, const int orderId, string info) override
	{
		if (orderId == 29) // move to building area to prospect building
		{
			constructionPlan = info;
			goTo = make_unique<Vector2f>(p->mousePositionToWorld);
			if (!mission->taskBuilding)
			{
				setMissionType(1); // be careful here if you're already building something, how to change this while building?!
			}

		}
		else if (orderId == 0) // Left Mouse
		{

		}
		else if (orderId == 1) // Right Mouse
		{
			goTo = make_unique<Vector2f>(p->mousePositionToWorld);
		}
		else if (orderId == 30)
		{	
			goTo = make_unique<Vector2f>(p->mousePositionToWorld);
			setMissionType(4);
			focusMine = p->focusMine; // sets the mine
			focusGameObject = focusMine->leaveAt; //sets the factory to leave at
			if (IS_NULL(focusGameObject) || IS_NULL(focusMine) || IS_NULL(focusGameObject->getFocusMine()) || IS_NULL(focusMine->leaveAt))
			{
				cout << "error null pointers deteceted " << endl;
			}
			
		}
	}
	virtual bool				 update(P&p) override
	{
		if (mission->taskBuilding) // not able to move, building
		{
			return TimerDecrement(p);
		}
		else if (mission->taskMining)
		{
			return Mining(p);
		}
		else
		{
			move(p);
			return false;
		}
	}
	virtual void			     move(P & p) override // dont need bool here!
	{
		if (goTo != NULL)
		{
			deltax = thisID.getPosition().x - goTo->x;
			deltay = thisID.getPosition().y - goTo->y;
			z = pythagoras(deltax, deltay);
			if (z > 1)
			{
				thisID.move((-deltax / z)*moveSpeed, (-deltay / z)*moveSpeed);
				timerBar.move((-deltax / z)*moveSpeed, (-deltay / z)*moveSpeed);
				healthbar.move((-deltax / z)*moveSpeed, (-deltay / z)*moveSpeed);
			}
			else // saying we'e arrived = |z| <= 1 (with some crappy precision)
			{
				if (mission->taskProspectBuilding)
				{
					if (constructionPlan == "Factory")
					{
						p->gold -= 30;
					}
					else if (constructionPlan == "Barrack")
					{
						p->gold -= 50;
					}
					timer = 10;
					setMissionType(2);
					goTo = NULL;
				}
				else
				{
					goTo = NULL;
				}
			}
		}
	}
	bool						 TimerDecrement(P&p) // used in update function, bool return is to send parameter to gameManager to start building or not!
	{
		if ((p->frame % 30) == 0)
		{
			timer--;
			timerBar.setSize(Vector2f(timerBar.getSize().x*0.9, timerBar.getSize().y));
		}
		if (timer != 0)
		{
			return false;
		}
		else // finished building
		{
			timerBar.setSize(Vector2f(thisID.getSize().x * 2, timerBar.getSize().y));
			mission->resetTasks();
			return true;
		}
	}
	bool			             Mining(P&p)
	{
		if (carryingGold)
		{
			if (thisID.getGlobalBounds().intersects(focusGameObject->getIdentity().getGlobalBounds())) // left minerals
			{
				*goTo = focusMine->goldMine.getPosition(); // change move to the mine
				carryingGold = false;
				p->gold += 10;
				return false;
			}
			else
			{
				move(p);
				return false;
			}
		}
		else
		{
			if (thisID.getGlobalBounds().intersects(focusMine->goldMine.getGlobalBounds())) // getting minerals
			{
				*goTo = focusGameObject->getIdentity().getPosition(); // change move to the designated leave position!
				carryingGold = true;
				return false;
			}
			else
			{
				move(p);
				return false;
			}

		}
	}
private:
	bool						 carryingGold = false;
};
class Warrior : public Unit
{
	public:
		Warrior()// not really used
		{
			GENERIC_INIT_WARRIOR();
		}
		Warrior(Vector2f & where)
		{
			thisID.setPosition(where);
			GENERIC_INIT_WARRIOR();
		}
		Warrior(const Vector2f where, Vector2f & moveTo)
		{
			thisID.setPosition(where);
			GENERIC_INIT_WARRIOR();
			setMoveTo(moveTo);
			
		}
		void GENERIC_INIT_WARRIOR()
		{
			type = 2;
			attackStrength = 10;
			healthPool = 100;
			thisID.setFillColor(Color(60, 160, 88));
			thisID.setSize(Vector2f(sizex, sizey));
			mission = make_unique<tasks>();
			setupBars();
		}
	virtual void			     newOrder(P&p, const int orderId, string info) override
	{
		if (orderId == 0)
		{
			//genericMove();
			//if (mission->attackMove)
			//{
				//if(detect.enemy){ // change focus and attack it (later code)}
				
			//}
		}
		else if (orderId == 1)
		{
			goTo = make_unique<Vector2f>(p->mousePositionToWorld);
		}
		else if (orderId == 29)
		{
			cout << "warriors cant build";
		}
		else if (orderId == 30)
		{
			cout << "warriors cant mine";
		}

	}
	virtual bool				 update(P&p) override
	{
		move(p);
		return false;
	}
	~Warrior() = default;
};
class Mage : public Unit
{
public:
	Mage()// not really used
	{
		GENERIC_INIT_MAGE();
	}
	Mage(Vector2f & where)
	{
		thisID.setPosition(where);
		GENERIC_INIT_MAGE();
		
	}
	Mage(const Vector2f where, Vector2f & moveTo)
	{
		thisID.setPosition(where);
		GENERIC_INIT_MAGE();
		setMoveTo(moveTo);

	}
	void GENERIC_INIT_MAGE()
	{
		type = 3;
		attackStrength = 2;
		healthPool = 60;
		thisID.setFillColor(Color(174, 88, 88));
		thisID.setSize(Vector2f(sizex, sizey));
		mission = make_unique<tasks>();
		setupBars();
	}
	
	virtual void			     newOrder(P&p, const int orderId, string info) override
	{
		if (orderId == 0)
		{
			//genericMove();
			//if (mission->attackMove)
			//{
			//if(detect.enemy){ // change focus and attack it (later code)}

			//}
		}
		else if (orderId == 1)
		{
			goTo = make_unique<Vector2f>(p->mousePositionToWorld);
		}
		else if (orderId == 29)
		{
			cout << "mages cant build";
		}
		else if (orderId == 30)
		{
			cout << "mages cant mine";
		}

	}
	virtual bool				 update(P&p) override
	{
		move(p);
		return false;
	}
	~Mage() = default;
};
