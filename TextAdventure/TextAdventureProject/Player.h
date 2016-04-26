#ifndef _PLAYER_H_
#define _PLAYER_H_
#pragma once

#include <functional>
#include "Inventory.h"
#include "LevelUpManager.h"
#include <map>
#include "Level.h"
#include "Entity.h"
#include "IAttack.h"
#include "ItalkNPC.h"	
#include "Parser.h"	
#include <list>	

class Player : public Entity, public Italk, public IAttack
{
	iTalkNPC * talkingTo		= NULL;
	IAttack  * battllingWith	= NULL;
	Inventory inventory;
	//LevelUpManager lvlUpMnger;
	int currentMapRow,currentMapColumm;
	
	std::string lastTalkedPhrase;
	std::map < std::string, std::function<void*(std::string, iTalkNPC* italk) >> levelRaiser;

	int state; //0 -> exploring 1-talking 2-battle with

public:
	//int currentMapRow;
	//int currentMapColumm;
	void receiveCommands(std::list<std::string> commands);
	std::string changeRoom(std::string direction, bool restriction);
	void setLevelReference(Level& level);
	void setPlayerRoom(int row, int column);
	Level* pLevel;

	Inventory* getInventory(){ return &inventory; }
	Player();
	Player(Level& level);
	Player(int mapRow, int mapColumm);
	~Player();
	void modifyEntity(Italk* talkable, std::string string) override;
	std::string getId() override;
	std::string getCurrentstatus();
	void initialize();
	void increaseLevel(std::list < std::string> list);
	void modifyStatus(int status);
	void useItem(std::list<std::string> stringListCommand);
	bool receiveAttack(Player* p);
	void verifyEndReach();

	//last minute fix, sorry <3
	int getLocation(std::string s)
	{
		if (s == "row")
			return currentMapRow;

		if (s == "column")
			return currentMapColumm;

		return 0;
	}

};

#endif