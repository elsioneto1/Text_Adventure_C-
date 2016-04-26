#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Room.h"
#include <iostream>
#include <map>
#include <functional>
#include "..\externallibs\tinyXML\tinyxml2.h"



class GameLogic;

#pragma once
class Level
{
	int rows ;
	int columns;
	
	//int currentPlayerRow, currentPlayerColum;

	std::map < std::string, std::function<NPC*()  >> creationalNPCS; // map that gathers function pointers to create npcs, items, rooms
	std::map < std::string, std::function<Item*() >> creationalItems;
	GameLogic* pGameLogic;
	XMLDocument* XMLDOC = NULL;

public:
	int currentLevel;
	Room **rooms;
	Level();
	~Level();
	void setXMLDocRef(XMLDocument * doc);
	XMLDocument* getXMLDocRef();
	void loadLevel(int level, XMLElement* element, Player* p);
	void saveGame(Player* p);
	void loadGame(std::string filePath, Player * p);
	void unloadLevel();
	int getNumOfRows();
	int getNumOfColums();
	void initialize();
};

#endif