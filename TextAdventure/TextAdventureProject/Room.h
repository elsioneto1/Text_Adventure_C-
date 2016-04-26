#ifndef _ROOM_H_
#define _ROOM_H_


#include "Item_Dynamic.h"
#include "Item_Static.h"
#include "Inventory.h"
#include "..\externallibs\tinyXML\tinyxml2.h"
#include "Fightable_NPC.h"
#include <map>
#include <iostream>
#include <functional>

using namespace tinyxml2;
#pragma once
class Room
{



private:
	
	int rowId;
	int colummId;
	

public:

	std::map < std::string, Item*>				itens;
	std::map < std::string, NPC*>				npcs;
	std::map < std::string, std::string>		description;
	std::map < std::string, IItemInteractable*>	interactables; // can have elements from NPC and ITEM


	Room** rooms;
	Room(int row, int columm);
	Room();
	~Room();
	void setIds(int row, int columm);
	void configureRoomNPCS	(XMLElement* element,
		std::map < std::string, std::function<NPC*() >>*map);
	void configureRoomItems	(XMLElement* element,
		std::map < std::string, std::function<Item*()>>*map);

	bool endingLevelRoom			= false;
	std::string endingDirection		= "";
	

	int state = -1;  //-1 inexistent / 0 closed / 1 open / 2 climbable / 3 unreachable
	std::string readDescription(std::string key);
	NPC* npcInteract(std::string key);
	std::string itemInteractManager(std::string key, Inventory* inventory);
	void describeItemsAndNpcsOnRoom();
	IItemInteractable* getInteractable(std::string key);
	void setRoomDescriptions(XMLElement* element);

	Fightable_NPC* checkBattle(std::string key);
	void unloadItemsAndNpcs();

};

#endif