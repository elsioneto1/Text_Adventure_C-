#include "Level.h"
#include "Player.h"


Level::Level()
{	
	
	currentLevel = 1;
}


Level::~Level()
{
	//GOODBYE XML!
	delete XMLDOC;
}

// sets the main XML ref
void Level::setXMLDocRef(XMLDocument * doc)
{
	XMLDOC = doc;

}

//gets the XML ref
XMLDocument* Level::getXMLDocRef()
{
	return XMLDOC;
}

void Level::initialize()
{
	// set function pointers to be used to instantiate level structures
	creationalNPCS["SETNPC"]			= NPC::_createNPC;
	creationalNPCS["SETFNPC"]			= Fightable_NPC::_createFNPC;
	creationalItems["SETITEM"]			= Item::_Item;
	creationalItems["SETSTATICITEM"]	= Item_Static::_ItemStatic;
	creationalItems["SETDYNAMICITEM"]	= Item_Dynamic::_DyanamicItem;
}

void Level::loadLevel(int level, XMLElement* element, Player* p)
{
	//default values initialized creating the rooms array
	rows	= 0;
	columns = 0;

	int lvl = -1;
	// reads the level info for number of rowns and columns in the XML
	XMLElement * levelNode = element->FirstChildElement("Level");
	int startingRow;
	int startingColumn;
	while (levelNode != NULL)
	{
		int xmlLevel;
		levelNode->QueryIntAttribute("id",&xmlLevel);
		if (level == xmlLevel)
		{
			levelNode->QueryIntAttribute("Rows", &rows);
			levelNode->QueryIntAttribute("Columns", &columns);
			
			levelNode->QueryIntAttribute("playerStartingRow"	, &startingRow);
			levelNode->QueryIntAttribute("playerStartingColumn"	, &startingColumn);
			p->setPlayerRoom(startingRow,startingColumn);
			lvl = xmlLevel;
		}


		if (lvl != -1)
			break;

		levelNode = levelNode->NextSiblingElement();

		
	}
	// if cant find the level, there's an error
	if (lvl == -1)
	{
		std::cout << "There's a problem in loading your game. Talk to the awesome programmer, he probably have a fix for that" << std::endl;
		return;
	}

	




	//create the rooms!
	rooms = new Room*[rows];
	
	for (int i = rows-1; i >= 0; i--)
	{

		rooms[i] = new Room[columns];
		for (int j = 0; j < columns; j++)
		{

			rooms[i][j].setIds(i, j);

			bool found = false; // boolean to verify if found the room
			XMLElement* roomNode = levelNode->FirstChildElement("Room");
			while (!found && roomNode != NULL)
			{

				int roomRow		= -1;
				int roomColumn	= -1;

				roomNode->QueryIntAttribute("Row"	 , &roomRow);
				roomNode->QueryIntAttribute("Column" , &roomColumn);

				if (roomRow == i && roomColumn == j) // found the room in the xml
				{
					found = true;
					int nodeState;
					roomNode->QueryIntAttribute("state", &nodeState);
					rooms[i][j].state = nodeState; // set the room state
					bool b = false; // verify if the is the final one for this level
					roomNode->QueryBoolAttribute("final",&b);
					if (b)
					{
						rooms[i][j].endingLevelRoom = true;
						rooms[i][j].endingDirection = roomNode->Attribute("finalDirection");
						std::string s = rooms[i][j].endingDirection;
					}

				}
				if (!found)
					roomNode = roomNode->NextSiblingElement();

			}
			if (found)
			{
				rooms[i][j].setRoomDescriptions(roomNode->FirstChildElement("Description"));
				rooms[i][j].configureRoomNPCS(roomNode->FirstChildElement("Npcs"), &creationalNPCS);
				rooms[i][j].configureRoomItems(roomNode->FirstChildElement("Items"), &creationalItems);
				rooms[i][j].rooms = rooms;
			}
		}
	

	}
	std::cout << rooms[startingRow][startingColumn].description["ENTRY"] << std::endl;
	
}



void Level::unloadLevel()
{

	for (int i = rows - 1; i >= 0; i--)
	{
		for (int j = 0; j < columns; j++)
		{
			rooms[i][j].unloadItemsAndNpcs();
		}
	}


	for (int i = 0; i < rows; i++)
	{
		delete[] rooms[i];
	}
	
	delete[] rooms;
	

	
}

int Level::getNumOfRows()
{
	return rows-1;
}

int Level::getNumOfColums()
{
	return columns-1;
}

void Level::saveGame(Player * p)
{


	XMLDocument document;
	XMLNode * pRoot = document.NewElement("LoadStructure");
	document.InsertFirstChild(pRoot);
	XMLElement * pLevel = document.NewElement("LevelData");
	

	pLevel->SetAttribute("rows", rows);
	pLevel->SetAttribute("columns", columns);

	pRoot->InsertEndChild(pLevel);
	
	//level
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			XMLElement * pRoom = document.NewElement("RoomData");
			pLevel->InsertEndChild(pRoom);
			std::map < std::string, Item*>::iterator		Itens;
			std::map < std::string, NPC*>::iterator			Npcs;
			std::map < std::string, std::string>::iterator	descriptions;

			pRoom->SetAttribute("rowId", i);
			pRoom->SetAttribute("rowColumn", j);
			pRoom->SetAttribute("state", rooms[i][j].state);

			if (rooms[i][j].endingLevelRoom)
			{
				pRoom->SetAttribute("final", 1);
				pRoom->SetAttribute("finalDirection", rooms[i][j].endingDirection.c_str());
			}

			//itens
			XMLElement * pItems = document.NewElement("Items");
			pRoom->InsertEndChild(pItems);

			XMLElement * pStatic = document.NewElement("Static");
			pItems->InsertEndChild(pStatic);

			XMLElement * pDynamic = document.NewElement("Dynamic");
			pItems->InsertEndChild(pDynamic);
			for (Itens = rooms[i][j].itens.begin(); Itens != rooms[i][j].itens.end(); ++Itens)
			{
				std::string s = Itens->first;
				XMLElement * pItem = document.NewElement(s.c_str());
				
				IItemInteractable* iPointer = NULL;
				iPointer = rooms[i][j].interactables[Itens->first]; // verify if it is a interactable item

				if (iPointer == NULL)
				{
					pDynamic->InsertEndChild(pItem);
				}
				else
				{
					Item_Static * sI = dynamic_cast<Item_Static*>(Itens->second);
					pStatic->InsertEndChild(pItem);
					pItem->SetAttribute("solvingItem",	sI->keyPass.c_str());
					pItem->SetAttribute("rowUnlock",	sI->solvingRow);
					pItem->SetAttribute("columnUnlock", sI->solvingColumm);
				}
				
				pItem->SetText(rooms[i][j].description[Itens->first].c_str());

				
			}

			//npcs 

			XMLElement * pNpcs = document.NewElement("Npcs");
			pRoom->InsertEndChild(pNpcs);

			XMLElement * pFightable = document.NewElement("Fightable");
			pNpcs->InsertEndChild(pFightable);

			XMLElement * pCommon = document.NewElement("Common");
			pNpcs->InsertEndChild(pCommon);

			for (Npcs = rooms[i][j].npcs.begin(); Npcs != rooms[i][j].npcs.end(); ++Npcs)
			{
				std::string s				= Npcs->first;
				XMLElement * pNpc			= document.NewElement("Npc");
				pNpc->SetAttribute("id", s.c_str());
				XMLElement * pNpcDialogs	= document.NewElement("Dialogs");
				pNpc->InsertEndChild(pNpcDialogs);

				if (Npcs->second->isFightable())
				{

					pFightable->InsertEndChild(pNpc);
					Fightable_NPC * fnpc = dynamic_cast<Fightable_NPC*>(Npcs->second);
					pNpc->SetAttribute("combatLevel", fnpc->combatLevel);


				}
				else
				{
					pCommon->InsertEndChild(pNpc);
					if (Npcs->second->itemResolving != "") // it means it's an item interactable NPC
					{
						pNpc->SetAttribute("interactable", 1);
						pNpc->SetAttribute("solvingItem", Npcs->second->itemResolving.c_str());
						pNpc->SetAttribute("rowUnlock", Npcs->second->solvingRow);
						pNpc->SetAttribute("columnUnlock", Npcs->second->solvingColumm);
					}
					//verify and inserts any event dialogs
					std::map <std::string, std::string>::iterator unlockIt;
					for (unlockIt = Npcs->second->unlockDialogs.begin(); unlockIt != Npcs->second->unlockDialogs.end(); ++unlockIt)
					{
						XMLElement * dialogs = document.NewElement(unlockIt->first.c_str());
						pNpcDialogs->InsertEndChild(dialogs);

						dialogs->SetAttribute("key", unlockIt->first.c_str());
						dialogs->SetAttribute("rowUnlock", Npcs->second->solvingRow);
						dialogs->SetAttribute("columnUnlock", Npcs->second->solvingColumm);
						dialogs->SetText(Npcs->second->unlockDialogs[unlockIt->first].c_str());
					}
				}

				XMLElement * dialogs = document.NewElement("DEFAULT");
				dialogs->SetText(Npcs->second->defaultMessage.c_str());
				pNpcDialogs->InsertEndChild(dialogs);

				std::map <std::string, std::string>::iterator dialogsIt;
				for (dialogsIt = Npcs->second->dialogs.begin(); dialogsIt != Npcs->second->dialogs.end(); ++dialogsIt)
				{
					XMLElement * dialogs = document.NewElement(dialogsIt->first.c_str());
					pNpcDialogs->InsertEndChild(dialogs);

					dialogs->SetText(Npcs->second->dialogs[dialogsIt->first].c_str());
					if (dialogsIt->first == "OPEN") // special key
					{
						dialogs->SetAttribute("rowUnlock", Npcs->second->solvingRow);
						dialogs->SetAttribute("columnUnlock", Npcs->second->solvingColumm);
					}
				}

			}

			// dialogs

			XMLElement * pDescriptions = document.NewElement("Descriptions");
			pRoom->InsertEndChild(pDescriptions);

			for (descriptions = rooms[i][j].description.begin(); descriptions != rooms[i][j].description.end(); ++descriptions)
			{

				XMLElement * element = document.NewElement(descriptions->first.c_str());
				element->SetText(descriptions->second.c_str());
				pDescriptions->InsertEndChild(element);

			}

		}

	}
	//player
	XMLElement * pPlayer = document.NewElement("Player");
	pRoot->InsertEndChild(pPlayer);

	pPlayer->SetAttribute("currentFloor", p->pLevel->currentLevel);

	pPlayer->SetAttribute("currentRow", p->getLocation("row"));
	pPlayer->SetAttribute("currentColumn", p->getLocation("column"));

	pPlayer->SetAttribute("speakLevel",		LevelUpManager::Instance().speakLevel);
	pPlayer->SetAttribute("combatLevel",	LevelUpManager::Instance().combatLevel);
	pPlayer->SetAttribute("moveLevel",		LevelUpManager::Instance().moveLevel);

	std::map<std::string, Item*>::iterator inventoryIt;
	for (inventoryIt = p->getInventory()->itemMap.begin(); inventoryIt != p->getInventory()->itemMap.end(); ++inventoryIt)
	{
		XMLElement * inventoryItem = document.NewElement("Item");
		inventoryItem->SetAttribute("id", inventoryIt->first.c_str());
		pPlayer->InsertEndChild(inventoryItem);
	}



	document.SaveFile("TextAdventureGameSaveStructure.xml");
	

}

void Level::loadGame(std::string filePath, Player * p)
{
	// unload all data before load again
	unloadLevel();
	p->getInventory()->clearItems();

	XMLDocument* xml = new XMLDocument();
	xml->LoadFile("TextAdventureGameSaveStructure.xml");

	XMLElement* root = xml->FirstChildElement("LoadStructure");

	// load player
	int currentPRow		= 0;
	int currentPColumn	= 0;
	root->FirstChildElement("Player")->QueryIntAttribute("currentRow", &currentPRow);
	root->FirstChildElement("Player")->QueryIntAttribute("currentColumn", &currentPColumn);
	root->FirstChildElement("Player")->QueryIntAttribute("currentFloor", &p->pLevel->currentLevel);

	p->setPlayerRoom(currentPRow,currentPColumn);

	root->FirstChildElement("Player")->QueryIntAttribute("speakLevel", &LevelUpManager::Instance().speakLevel);
	root->FirstChildElement("Player")->QueryIntAttribute("combatLevel", &LevelUpManager::Instance().combatLevel);
	root->FirstChildElement("Player")->QueryIntAttribute("moveLevel", &LevelUpManager::Instance().moveLevel);

	XMLElement* ItemsNode = root->FirstChildElement("Player")->FirstChildElement();
	while (ItemsNode != NULL)
	{
		Item* i = new Item();
		i->id = ItemsNode->Attribute("id");
		p->getInventory()->AddItem(i);

		ItemsNode = ItemsNode->NextSiblingElement();
	}

	// load rooms
	rows	= 0;
	columns = 0;
	root->FirstChildElement("LevelData")->QueryIntAttribute("rows", &rows);
	root->FirstChildElement("LevelData")->QueryIntAttribute("columns", &columns);

		
	rooms = new Room*[rows];
	for (int i = 0; i < rows; i++)
	{
		rooms[i] = new Room[columns];
		for (int j = 0; j < columns; j++)
		{
			rooms[i][j].setIds(i, j);

			bool found = false; // boolean to verify if the room is found
			XMLElement* roomNode = root->FirstChildElement("LevelData")->FirstChildElement("RoomData");
			while (!found && roomNode != NULL)
			{

				int roomRow = -1;
				int roomColumn = -1;

				roomNode->QueryIntAttribute("rowId", &roomRow);
				roomNode->QueryIntAttribute("rowColumn", &roomColumn);

				if (roomRow == i && roomColumn == j) // found the room in the xml
				{
					found = true;
					int nodeState;
					roomNode->QueryIntAttribute("state", &nodeState);
					rooms[i][j].state = nodeState; // set the room state
					bool b = false; // verify if the is the final one for this level
					roomNode->QueryBoolAttribute("final", &b);
					if (b)
					{
						rooms[i][j].endingLevelRoom = true;
						rooms[i][j].endingDirection = roomNode->Attribute("finalDirection");
						std::string s = rooms[i][j].endingDirection;
					}

				}
				if (!found)
					roomNode = roomNode->NextSiblingElement();

			}
			if (found)
			{
				
				
				rooms[i][j].configureRoomItems(roomNode->FirstChildElement("Items"), &creationalItems);
				rooms[i][j].configureRoomNPCS(roomNode->FirstChildElement("Npcs"), &creationalNPCS);
				rooms[i][j].setRoomDescriptions(roomNode->FirstChildElement("Descriptions"));
				rooms[i][j].rooms = rooms;
			}
		}

	}

	delete xml;
}