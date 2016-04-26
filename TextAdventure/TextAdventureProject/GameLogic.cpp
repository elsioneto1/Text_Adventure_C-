#include "GameLogic.h"
#include "../ExternalLibs/Lua/LuaState.h"
#include "Parser.h"
#include "..\externallibs\tinyXML\tinyxml2.h"
#include <iostream>
#include <list>

GameLogic* GameLogic::instance = NULL;
GameLogic::GameLogic()
{
}


GameLogic::~GameLogic()
{
}

using namespace tinyxml2;

void GameLogic::initialize()
{

	player	= new Player();
	player->initialize();

	level	= createLevel(1, NULL, player);
	player->setLevelReference(*level);
	
}

void GameLogic::GameLoop()
{
	while (true)
	{


		//std::cout << player->currentMapRow << player->currentMapColumm << std::endl;
		std::cout << " " << std::endl;
		std::cout << player->getCurrentstatus() << std::endl;
	
		std::string command;
		std::getline(std::cin, command);
		std::list<std::string> list;

		Parser::Instance().parseCommand(command, list);
		player->receiveCommands(list);

		command = "";
	}

	delete player;
	
	delete level;
}

void GameLogic::getInput()
{

}

Level* GameLogic::createLevel(int level, XMLElement* element, Player* p)
{
	Level* l = new Level();
	l->initialize(); 
	XMLDocument* xml = new XMLDocument();
	xml->LoadFile("../Resources/TextAdventureGame.xml");
	XMLElement* rootNode = xml->FirstChildElement("TextAdventure");
	l->loadLevel(l->currentLevel	, rootNode,p); 
	l->setXMLDocRef(xml);

	return l;
}

void GameLogic::unloadLevel()
{

}




