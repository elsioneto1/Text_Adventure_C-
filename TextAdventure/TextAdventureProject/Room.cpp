#include "Room.h"
#include "Parser.h"


Room::Room()
{

	//description["ROOM"] = "This is a Room";

}



Room::Room(int row, int columm)
	:rowId(row), colummId(columm)
{
	//description["ROOM"] = "This is a room";
}


Room::~Room()
{

}


std::string Room::readDescription(std::string key)
{
	// fetch for a description in description map and returns a string
	std::cout << rowId << std::endl;
	std::cout << colummId << std::endl;
	std::cout << "" << std::endl;
	std::string str = "";
	str = "I don't have any particular insight about that";

	std::map<std::string, std::string>::iterator it;
	for (it = description.begin(); it != description.end(); ++it)
	{
		if (it->first == key)
		{
			str = it->second;
		}
	}
	
	return str;
}

NPC* Room::npcInteract(std::string key)
{
	// start talking to a NPC
	NPC* npc = NULL;

	std::map < std::string, NPC*>::iterator it;
	for (it = npcs.begin(); it != npcs.end(); ++it)
	{
		if (it->first == key )
		{
			npc = it->second;
		}
	}

	return npc;
}


Fightable_NPC* Room::checkBattle(std::string key)
{
	//check if it's a fightable NPC
	Fightable_NPC* npc = NULL;

	std::map < std::string, NPC*>::iterator it;
	for (it = npcs.begin(); it != npcs.end(); ++it)
	{
		if (it->second->id == key)
		{
			if (it->second->isFightable() == true)
			{
				npc = dynamic_cast<Fightable_NPC*>(it->second);
			}
		}
	}

	return npc;
}


std::string Room::itemInteractManager(std::string key, Inventory* inventory)
{
	std::string s = "This item don't exist";
	std::map < std::string, Item*>::iterator it;
	for (it = itens.begin(); it != itens.end(); ++it)
	{
		if (it->first == key)
		{
			if (itens[key]->pickable())
			{
				inventory->AddItem(itens[(key)]);
				s = "You picked up ";
				s.append(itens[key]->id);
				itens.erase(it);
				description.erase(key);
			}
			else
			{
				s = "You can't pick up ";
				s.append(itens[key]->id);
			}
			break;
		}
	}
	return s;
}

void Room::setIds(int row, int columm)
{
	colummId = columm;
	rowId = row;

}

void Room::setRoomDescriptions(XMLElement* element)
{


	XMLElement * descriptions = element->FirstChildElement();

	
	while (descriptions != NULL)
	{

		description[descriptions->Value()] = descriptions->FirstChild()->Value();
		descriptions = descriptions->NextSiblingElement();

	}

}

void Room::configureRoomNPCS(XMLElement* element, std::map < std::string, std::function<NPC*() >>*map)
{
	// loads all NPCS in the room
	XMLElement * fnpcsNode	= element->FirstChildElement("Fightable")->FirstChildElement();
	XMLElement * npcsNode	= element->FirstChildElement("Common")->FirstChildElement();
	while (fnpcsNode != NULL)
	{

		std::string a							= fnpcsNode->Attribute("id");
		npcs[fnpcsNode->Attribute("id")]		= map->find("SETFNPC")->second();
		npcs[fnpcsNode->Attribute("id")]->id	= fnpcsNode->Attribute("id");
		std::string npcName						= fnpcsNode->Attribute("id");
		XMLElement * dialogs					= fnpcsNode->FirstChildElement("Dialogs");
		npcs[npcName]->defaultMessage			= dialogs->FirstChildElement("DEFAULT")->FirstChild()->Value();
		XMLElement * dialogsElements			= dialogs->FirstChildElement();
		dialogsElements							= dialogsElements->NextSiblingElement();
		Fightable_NPC* fnpc						= dynamic_cast<Fightable_NPC*>(npcs[npcName]);
		int combatLevel;
		fnpcsNode->QueryIntAttribute("combatLevel",&combatLevel);
		fnpc->setCombatLevel(combatLevel);
		while (dialogsElements != NULL)
		{
			npcs[npcName]->dialogs[dialogsElements->Value()] = dialogsElements->FirstChild()->Value();
			std::string str = dialogsElements->Value();
			if (str == "OPEN" ) // sets where the door will be unlocked
			{
				int row;
				int column;
				dialogsElements->QueryIntAttribute("rowUnlock"		, &row);
				dialogsElements->QueryIntAttribute("columnUnlock"	, &column);
				npcs[npcName]->setSolvings(row, column, "OPEN");
				
			}
			dialogsElements = dialogsElements->NextSiblingElement();
		}

		fnpcsNode = fnpcsNode->NextSiblingElement();

	}
	while (npcsNode != NULL)
	{
		std::string a							= npcsNode->Attribute("id");
		npcs[npcsNode->Attribute("id")]			= map->find("SETNPC")->second();
		npcs[npcsNode->Attribute("id")]->id		= npcsNode->Attribute("id");
		std::string npcName						= npcsNode->Attribute("id");
		XMLElement * dialogs					= npcsNode->FirstChildElement("Dialogs");
		npcs[npcName]->defaultMessage			= dialogs->FirstChildElement("DEFAULT")->FirstChild()->Value();
		XMLElement * dialogsElements			= dialogs->FirstChildElement();
		dialogsElements							= dialogsElements->NextSiblingElement();
		
		const char* c = "rowUnlock";
		//dialogsElements->FindAttribute("rowUnlock");
		int row;
		int column;
		bool interactable = false;
		npcsNode->QueryBoolAttribute("interactable", &interactable);
		if (interactable)
		{
			
			npcs[npcName]->itemResolving = npcsNode->Attribute("solvingItem");
			npcsNode->QueryIntAttribute("rowUnlock", &row);
			npcsNode->QueryIntAttribute("columnUnlock", &column);
			npcs[npcName]->setSolvings(row, column, npcs[npcName]->itemResolving);
			interactables[npcName] = dynamic_cast<IItemInteractable*>(npcs[npcName]);
		}


		while (dialogsElements != NULL)
		{
			// bloody tricky to figure out that this must to be a const
			const XMLAttribute* A = dialogsElements->FirstAttribute(); 
			// special dialog structure that unlocks doors
			if (A != NULL)
			{
				
				//overwrites item resolving, if there's any
				//SET THE RESOLVINGS
				int row;
				int column;
				dialogsElements->QueryIntAttribute("rowUnlock", &row);
				dialogsElements->QueryIntAttribute("columnUnlock", &column);
				std::string key;
				key = dialogsElements->Attribute("key");
				npcs[npcName]->setSolvings(row, column, key);
				npcs[npcName]->unlockMap[key] = NPC::unlockDoorWithWord;
				npcs[npcName]->unlockDialogs[dialogsElements->Value()] = dialogsElements->FirstChild()->Value();
				
			}
			else
			{ 
				// common dialog
				npcs[npcName]->dialogs[dialogsElements->Value()] = dialogsElements->FirstChild()->Value();
			}
			dialogsElements = dialogsElements->NextSiblingElement();
		}

		npcsNode = npcsNode->NextSiblingElement();

	}

	//fnpc->setRoomReference(rooms);
}

void Room::configureRoomItems(XMLElement* element, std::map < std::string, std::function<Item*() >>*map)
{
	// loads all items 
	XMLElement * dynamicItemsNode	= element->FirstChildElement("Dynamic")->FirstChildElement();
	XMLElement * staticItemsNode	= element->FirstChildElement("Static")->FirstChildElement();

	while (dynamicItemsNode != NULL)
	{
		itens[dynamicItemsNode->Value()]		= map->find("SETDYNAMICITEM")->second();
		itens[dynamicItemsNode->Value()]->id	= Parser::Instance().setToUpper(dynamicItemsNode->Value());
		description[dynamicItemsNode->Value()]	= dynamicItemsNode->FirstChild()->Value();
		dynamicItemsNode						= dynamicItemsNode->NextSiblingElement();

	}
	
	while (staticItemsNode != NULL)
	{
		itens[staticItemsNode->Value()] = map->find("SETSTATICITEM")->second();
		itens[staticItemsNode->Value()]->id = Parser::Instance().setToUpper(staticItemsNode->Value());
		interactables[staticItemsNode->Value()] = dynamic_cast<IItemInteractable*>(itens[staticItemsNode->Value()]);
		//interactables[staticItemsNode->Value()]->setSolvings(1, 0, "BLACK_KEY");
		int row;
		int column;
		std::string str;
		staticItemsNode->QueryIntAttribute("rowUnlock", &row);
		staticItemsNode->QueryIntAttribute("columnUnlock", &column);
		str = staticItemsNode->Attribute("solvingItem");
		interactables[staticItemsNode->Value()]->setSolvings(row, column, str);
		description[staticItemsNode->Value()] = staticItemsNode->FirstChild()->Value();
		staticItemsNode = staticItemsNode->NextSiblingElement();
	}


}

void Room::describeItemsAndNpcsOnRoom()
{
	std::string str = "There's nothing relevant to be inspected in the room.";

	if (itens.size() > 0)
	{
		str = "There's some interesting stuff in the room, such as : ";
		std::map < std::string, Item*>::iterator it;
		for (it = itens.begin(); it != itens.end(); ++it)
		{
			str.append(it->second->id);
			str.append(", ");
		}
		std::cout << str << std::endl;
	}

	if (npcs.size() > 0)
	{
		if (itens.size() > 0)
		{
			str = "And there's some people here too, magicly, you know that they're called as : ";
		}
		else
		{

			str = "There's some people here. You know that they're called as : ";
		}

		std::map < std::string, NPC*>::iterator it1;
		for (it1 = npcs.begin(); it1 != npcs.end(); ++it1)
		{
			str.append(it1->second->id);
			str.append(" ");
		}
		std::cout << str << std::endl;
	}

}



IItemInteractable* Room::getInteractable(std::string key)
{

	std::map<std::string, IItemInteractable*>::iterator it;
	for (it = interactables.begin(); it != interactables.end(); ++it)
	{
		if (it->first == key)
		{
			return it->second;
		}
	}
	return NULL;
}


void Room::unloadItemsAndNpcs()
{

	std::map < std::string, Item*>::iterator itItens;
	for (itItens = itens.begin(); 
		itItens != itens.end(); 
		++itItens)
	{
		delete itens[itItens->first]; // releases itens memory
	}

	std::map < std::string, NPC*>::iterator itNpcs; // release npcs memory
	for (itNpcs = npcs.begin();
		itNpcs != npcs.end();
		++itNpcs)
	{
		delete npcs[itNpcs->first];
	}

	// interactables memory is released too, since they all are npcs and itens
	itens.clear();
	npcs.clear();
	interactables.clear();
	description.clear();
	
	
}