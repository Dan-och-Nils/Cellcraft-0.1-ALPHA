#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <math.h>
#include <vector>
using namespace sf;
using namespace std;


class Menu
{
public:
	Menu()
	{
		arial.loadFromFile("arial.ttf");
		menuarea.setSize(Vector2f(400, 800));
		menuarea.setPosition(1500, 100);
		menuarea.setFillColor(Color::Black);
		menuarea.setOutlineColor(Color::Blue);
		menuarea.setOutlineThickness(5);
		createMenu();
	}
	~Menu() = default;
	void updateOptions(RenderWindow & window, const Vector2f dir, int & activeMenu, int & currentMenu)
	{
		menuarea.move(dir);
		for (const auto & menuIn : menuOptions)
		{
			menuIn->move(dir);
			window.draw(*menuIn);
		}
		int count = 0;
		for (const auto & textIn : menuOptionsText)
		{
			//if (currentMenu == activeMenu) // fix this later
			//{
			//	break;
			//}
			//else
			currentMenu = activeMenu;
			if (currentMenu == 0)
			{
				textIn->setString("");
			}
			else if (activeMenu == 1)
			{
				textIn->setString(menuUnitText[count]);
			}
			else if (activeMenu == 10)
			{
				textIn->setString(menuBuildText[count]);
			}

			++count;
			textIn->move(dir);
			window.draw(*textIn);
		}

	}
	void addButton(shared_ptr<RectangleShape> & in)
	{
		menuOptions.push_back(move(in));
	}
	void addText(shared_ptr<Text> & in)
	{
		menuOptionsText.push_back(move(in));
	}
	void createMenu()
	{
		int column = 0;
		int row = 0;
		int iterator = 0;

		while (iterator++ < 9) // there is nine boxes of options 
		{
			menuBox = make_unique<RectangleShape>(Vector2f(boxwidth, boxheight));
			menuBox->setPosition(Vector2f(menuarea.getPosition().x + column*boxwidth * 2, menuarea.getPosition().y + row*boxheight * 2));
			menuBox->setFillColor(Color::Green);


			textBox = make_unique<Text>();
			textBox->setFont(arial);

			textBox->setFillColor(Color::Red);
			textBox->setOutlineColor(Color::Red);
			textBox->setCharacterSize(20);
			textBox->setPosition(Vector2f(menuarea.getPosition().x + column*boxwidth * 2, menuarea.getPosition().y + row*boxheight * 2));

			addButton(menuBox);
			addText(textBox);

			++column;
			if (column == 3)
			{
				++row;
				column = 0;
			}

		}
	}


private:
	Font arial;
	vector<shared_ptr<RectangleShape>> menuOptions;
	vector<shared_ptr<Text>>           menuOptionsText;
	string menuUnitText[9] = { "Attack","Move","Build","invent","invent" ,"invent" ,"invent" ,"invent" ,"invent" };
	string menuBuildText[9] = { "Build","invent","invent","invent","invent" ,"invent" ,"invent" ,"invent" ,"invent" };
	string selectionText[12] = { "Empty Selection", "Peon","Warrior","","","","","","","","Factory","Armory" };
	shared_ptr<RectangleShape>  menuBox = NULL;
	shared_ptr<Text>		    textBox = NULL;
	const int                   boxwidth = 75;
	const int				    boxheight = 75;
	RectangleShape				menuarea;
};

class Interface
{
private:
	Font arial;
	Text amount;
	Text select;
	Text gold;

	Menu gameMenus;
	int activeMenu = 0; // depends on active unit in selection
	int currentMenu = 0;
public:
	Interface()
	{
		arial.loadFromFile("arial.ttf");
		amount.setFillColor(Color::Red);
		amount.setOutlineColor(Color::Red);
		amount.setCharacterSize(20);
		amount.setFont(arial);
		amount.setPosition(1500, 1000);
		amount.setString("0");


		select.setFillColor(Color::Red);
		select.setOutlineColor(Color::Red);
		select.setCharacterSize(20);
		select.setFont(arial);
		select.setPosition(1650, 1000);
		select.setString("Empty Selection");

		arial.loadFromFile("arial.ttf");
		gold.setFillColor(Color::Yellow);
		gold.setOutlineColor(Color::Yellow);
		gold.setCharacterSize(20);
		gold.setFont(arial);
		gold.setPosition(1600, 1000);
		gold.setString("0");
	}
	~Interface() {}

	void moveInterface(const Vector2f dir)
	{
		amount.move(dir);
		select.move(dir);
		gold.move(dir);
	}
	void update(RenderWindow & window, P & p)
	{
		gold.setString(to_string(p->gold));
		moveInterface(p->viewMoveDirection);
		gameMenus.updateOptions(window, p->viewMoveDirection, activeMenu, currentMenu);

		window.draw(amount);
		window.draw(select);
		window.draw(gold);
	}
	void setSelection(P & p)
	{
		amount.setString("size : " + to_string(p->selectionsize));
		activeMenu = p->typeFirstObject;
		if (p->typeFirstObject == 0)
		{
			select.setString("Empty Selection");
		}
		else if (p->typeFirstObject == 1)
		{
			select.setString("Peon");
		}
		else if (p->typeFirstObject == 10)
		{
			select.setString("Factory");
		}

	}

};
