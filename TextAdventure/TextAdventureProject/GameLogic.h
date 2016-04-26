#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_

#include "Player.h"
#include "Level.h"
#pragma once
class GameLogic
{

	// Class which manages all the interactions with the player rand the current level loaded


public:

	// instance access
	static GameLogic& Instance()
	{
		if (instance == NULL)
		{
			instance = new GameLogic();
		}
		return *instance;
	}

	GameLogic();
	~GameLogic();
	void	GameLoop();
	void	getInput();
	Level*	createLevel(int level, XMLElement* element, Player* p);
	XMLElement * XML;
	void	unloadLevel();
	void	initialize();

private:

	Player* player;
	Level*	level;
	static	GameLogic* instance;

};

#endif