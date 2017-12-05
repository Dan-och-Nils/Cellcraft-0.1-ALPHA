#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <math.h>
#include <vector>
#include "global.h"
#include "interface.h"
#include "gameState.h"
#include "objectManager.h"

using namespace sf;
using namespace std;

class GameHub
{
private:

	RenderWindow				window;
	shared_ptr<Parameters>		gameParameters;
	Interface					gameInterface;
	Gamestate			        gameState;

	WorldObjects				gameWorldObjects; // mines etc!
	ObjectManager				gameManager;

public:
	GameHub()
	{
		sf::VideoMode desktop = sf::VideoMode().getDesktopMode();
		window.create(desktop, "Half Life");
		gameParameters = make_unique<Parameters>();
		gameParameters->view = window.getView();

		window.setView(gameParameters->view);

		window.setKeyRepeatEnabled(true);
		window.setFramerateLimit(60);

		setupGrid(gameParameters->Area);
		gameParameters->markedgrid = gameParameters->Area.at(1);


		gameParameters->buildProspect.setFillColor(Color::Yellow);
		gameParameters->buildProspect.setSize(Vector2f(sizex, sizey));
		gameParameters->buildProspect.setPosition(gameParameters->mousePositionToWorld);

		connectMines();
	}
	~GameHub()
	{
		gameParameters->Area.clear();
		gameParameters = NULL;
	}
	void			gameInputs()
	{
		while (window.pollEvent(gameParameters->event))
		{
			if (gameParameters->event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (gameParameters->event.type == Event::MouseButtonPressed)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (gameParameters->building)
					{
						gameManager.issueOrderFirst(gameParameters, 29, gameParameters->currentBlueprint);
					}
					gameParameters->buildingmenuActive = false;
					gameParameters->building = false;
					gameState.setBoxing(0);
					gameState.updateBoxPosition(gameParameters);
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					if (gameWorldObjects.intersectMine(gameParameters))
					{
						gameManager.issueOrderAll(gameParameters, 30, "Mine"); // Mine, clicked mine is set to focus
					}
					else
					{
						gameManager.issueOrderAll(gameParameters, 1, "MAR"); // MoveAttackRally
					}
				}
			}
			else if (gameParameters->event.type == Event::MouseButtonReleased)
			{
				if (gameParameters->event.mouseButton.button == sf::Mouse::Left)
				{
					gameState.setBoxing(1);
					gameParameters->selectionsize = gameManager.updateSelection(gameState.Box().getGlobalBounds());
					gameParameters->typeFirstObject = gameManager.first();
					gameInterface.setSelection(gameParameters); // update interface to see how many units/buildings are controlled and what is the first object in the selection
				}
			}
			if (gameParameters->event.type == Event::KeyReleased)
			{
				
				if (gameParameters->event.key.code == (Keyboard::B) && (gameParameters->typeFirstObject == 1))
				{
					if (gameParameters->buildingmenuActive)
					{
						if (gameParameters->gold >= 0) 
						{
							gameParameters->currentBlueprint = "Barrack";
							gameParameters->building = true;
						}
					}
					else
					{ 
					gameParameters->buildingmenuActive = true;
					}
				}
				else if (gameParameters->event.key.code == (Keyboard::F) && (gameParameters->typeFirstObject == 1))
				{
					if (gameParameters->buildingmenuActive && (gameParameters->gold >= 30))
					{
						gameParameters->currentBlueprint = "Factory";
						gameParameters->building = true;
					}
				}
				else if (gameParameters->event.key.code == (Keyboard::P) && (gameParameters->typeFirstObject == 10))
				{
					gameParameters->currentBlueprint = "Peon";
					gameManager.issueOrderFirst(gameParameters, 17, gameParameters->currentBlueprint);
				}
				else if (gameParameters->event.key.code == (Keyboard::W) && (gameParameters->typeFirstObject == 12))
				{
					gameParameters->currentBlueprint = "Warrior";
					gameManager.issueOrderFirst(gameParameters, 25, gameParameters->currentBlueprint);
				}
				else if (gameParameters->event.key.code == (Keyboard::M) && (gameParameters->typeFirstObject == 12))
				{
					gameParameters->currentBlueprint = "Mage";
					gameManager.issueOrderFirst(gameParameters, 14, gameParameters->currentBlueprint);
				}
			}
		}
	}
	void			updateParameters()
	{
		if (gameParameters->frame++ == 1000000) { gameParameters->frame = 0; }
		gameState.setMousePosition(window, gameParameters);
		gameParameters->buildProspect.setPosition(gameParameters->mousePositionToWorld);
		gameInputs();
	}
	void			run()
	{
		try
		{
			while (window.isOpen())
			{
				window.setView(gameParameters->view);
				updateParameters();
				updateGame();
			}
		}
		catch (exception)
		{
			cout << " error occured ";
		}
	}
	void			updateGrid()
	{
		for (const auto & Unit_in : gameParameters->Area)
		{
			if (Unit_in != NULL)
				if (Unit_in->node.getGlobalBounds().contains(gameParameters->mousePositionToWorld))
				{
					if (Unit_in != gameParameters->markedgrid)
					{
						changeColor(gameParameters->markedgrid->node, 0); // change last node to black
						gameParameters->markedgrid = Unit_in;			  // change marked node to this, which is a newly marked area
						changeColor(gameParameters->markedgrid->node, 1); // change this marked node to green
					}
				}

			window.draw(Unit_in->node);
		}
	}
	void		    updateGame()
	{
		window.clear();
		gameState.update(gameParameters, window); // updates scroll

		updateGrid();

		if (gameState.IsBoxing()) { window.draw(gameState.Box()); } // boxing area
		if (gameParameters->building) { window.draw(gameParameters->buildProspect); } // building prospectarea

		gameManager.update(gameParameters, window, gameWorldObjects); // updates unit movement , orders etc.
		gameInterface.update(window, gameParameters);
		gameWorldObjects.draw(window);
		window.display();
	}
	void			setView()
	{
		window.setView(gameParameters->view);
	}
	void            connectMines()
	{
		gameManager.connectMines(gameWorldObjects);

	}
};