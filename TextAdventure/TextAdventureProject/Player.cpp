#include "Player.h"


Player::Player()
{
	state				= 0;
	currentMapRow		= 0;
	currentMapColumm	= 0;

}

Player::Player(Level& level)
{



}

Player::Player(int mapRow = 0, int mapColumm = 0)
	:currentMapRow(mapRow), currentMapColumm(mapColumm)
{



}

Player::~Player()
{
}

void Player::initialize()
{

	levelRaiser["SPEAK"]	= LevelUpManager::_increaseSpeak;
	levelRaiser["MOVE"]		= LevelUpManager::_increaseMove;
	levelRaiser["COMBAT"]	= LevelUpManager::_increaseCombat;
}


void Player::verifyEndReach()
{
	


}

void Player::setPlayerRoom(int row, int column)
{
	currentMapRow		= row;
	currentMapColumm	= column;
}

void Player::receiveCommands(std::list<std::string> commands)
{
	if (state == 0) // exploring
	{
		if (commands.size() == 1 && commands.back() == "")
			return;

		if (commands.size() == 1 && commands.back() == "HELP")
		{

			std::cout << "TYPE 'SEE' AND WHAT YOU WANT TO SEE" << std::endl;
			std::cout << "TYPE 'TALK WITH' AND WHO WITH YOU WANT TO TALK TO" << std::endl;
			std::cout << "TYPE 'MOVE' AND THE DIRECTION YOU WANNA GO" << std::endl;
			std::cout << "TYPE 'ATTACK' AND WHO YOU WANT TO ATTACK. WHEN IN BATTLE, TYPE ATTACK AGAIN TO ATTACK, OR RUN TO QUIT." << std::endl;
			std::cout << "TYPE 'USE ' AND WHAT YOU WANT TO USE AND THE WHERE YOU WANT TO USE USING 'WITH'" << std::endl;
			std::cout << "TYPE 'DISPLAY ' TO SEE YOU INVENTORY AND YOUR STATUS!" << std::endl;

		}
		else if (commands.front() == "SEE" )
		{
			std::cout << pLevel->rooms[currentMapRow][currentMapColumm].readDescription(commands.back()) << std::endl;
			pLevel->rooms[currentMapRow][currentMapColumm].describeItemsAndNpcsOnRoom();
		}
		else if (commands.front() == "MOVE" || commands.front() == "MOVE TO")
		{
			std::cout << changeRoom(commands.back(), true) << std::endl;
		}
		else if (commands.front() == "DISPLAY" || commands.front() == "DISPLAY ITEMS")
		{
			inventory.showInventory();
			
			
		}
		else if (commands.front() == "SAVE")
		{
			pLevel->saveGame(this);
			std::cout << "Game Saved." << std::endl;
		}
		else if (commands.front() == "LOAD")
		{
			pLevel->loadGame(" ", this);

			std::cout << "Game Loaded." << std::endl;

		}
		else if (commands.front() == "PICK" || commands.front() == "PICK UP")
		{

			std::cout <<
				pLevel->rooms[currentMapRow][currentMapColumm].itemInteractManager(commands.back(), &inventory)
				<< std::endl;

		}
		else if (commands.front() == "USE")
		{
			// use an item
			useItem(commands);

		}
		else if (commands.front() == "MAP")
		{
			// display map
			std::cout << std::endl;
			std::cout << "Current level map. Room status in parenthesis" << std::endl;
			for (int i = pLevel->getNumOfRows()  ; i >= 0; i--)
			{

				std::cout << std::endl;
				for (int j = 0; j <= pLevel->getNumOfColums(); j++)
				{
					std::cout << i << j << "("<< pLevel->rooms[i][j].state << ")      ";
				}
				std::cout << std::endl;
			}

			std::cout << std::endl;
			std::cout << "Your actual location :"<< currentMapRow << currentMapColumm <<std::endl;

		}
		else if (commands.front() == "BATTLE" || commands.front() == "ATTACK")
		{
			battllingWith = pLevel->rooms[currentMapRow][currentMapColumm].checkBattle(commands.back());
			if (battllingWith != NULL)
			{
				state = 2;// fighting!
				std::cout << "You're battling with " << (battllingWith->getId()) << std::endl;
				//std::cout << battllingWith->getId() << " : " << (battllingWith->getStatement("TESTE")) << std::endl; //HC
				//lastTalkedPhrase = battllingWith->getStatement("TESTE"); //HC
			}

		}
		else if (commands.front() == "TALK" || commands.front() == "TALK TO")
		{
			talkingTo = pLevel->rooms[currentMapRow][currentMapColumm].npcInteract(commands.back());
			if (talkingTo != NULL)
			{
				state = 1; // talking!
				std::cout << "You're talking to " << (talkingTo->getId()) << std::endl;
				std::cout << talkingTo->getId() << " : " << (talkingTo->getStatement("ENTRY")) << std::endl; 
				lastTalkedPhrase = talkingTo->getStatement("ENTRY"); 
			}
		}
		else
		{
			std::cout << "I don't know that" << std::endl;
		}
	}
	else if (state == 1) // talking
	{ 

		if (commands.front() == "LEARN" )
		{
			//you can gain level by detecting keywords that the npcs use
			//the game uses the concepts of "memetic", that you can mimic relevant
			// behaviours to increase your ability in the environment.
			//If you try to learn a keyword from a NPC that never used it, you gain a level
			// related to the keyword you are learning
			// if you already learned the keyword from that NPC, it throws a default message 

			std::list < std::string> parsedString;
			Parser::Instance().parseCommand(lastTalkedPhrase, parsedString);
			increaseLevel(parsedString);
		}
		else if (commands.front() == "BYE" || commands.front() == "GOODBYE")
		{
			talkingTo->modifyEntity(this, "BYE"); // the npc says farewell
		}
		else
		{
			if (LevelUpManager::Instance().speakLevel < commands.size()) // the speak level delegates how many words you can speak in a sentence!
			{
				std::cout << "You feel a sense of dizziness because you're trying to speak more than you can. You need to increase your speak skills" << std::endl;
				return;// if you dont have the required level, stop the command
			}
			
				std::string string = Parser::Instance().joinString(commands);
			
				//verify if the command its to open the door
			if (talkingTo->accessUnlockMap(Parser::Instance().joinString(commands), pLevel->rooms))
			{
				std::cout << talkingTo->getId() << " : " << talkingTo->getUnlockStatement(Parser::Instance().joinString(commands)) << std::endl;
			}
			else
			{
				
				//else its a common dialog key
				lastTalkedPhrase = talkingTo->getStatement(commands.back());
				if (lastTalkedPhrase == talkingTo->getStatement("DEFAULT")) // may be two words, verify if the string exists again
				{
					lastTalkedPhrase = talkingTo->getStatement(Parser::Instance().joinString(commands));
				}
				std::cout << talkingTo->getId() << " : " << lastTalkedPhrase << std::endl;
			}
			
		}

	}
	else if (state == 2) //battle
	{
		
		if (commands.front() == "RUN")
		{
			state = 0;
			Fightable_NPC* fnpc = dynamic_cast<Fightable_NPC*>(battllingWith);
			std::cout << fnpc->getId() << " :" << fnpc->getStatement("run") << std::endl;
		}
		if (commands.front() == "ATTACK")
		{
			if (battllingWith->receiveAttack(this))
			{
				Fightable_NPC* fnpc = dynamic_cast<Fightable_NPC*>(battllingWith);
				std::string str = "OPEN";
				fnpc->accessUnlockMap(str, pLevel->rooms);
				std::cout << fnpc->getId() << " :" << fnpc->getStatement("OPEN") << std::endl;
			}
		}
	}
}

void Player::increaseLevel(std::list < std::string> list)
{
	std::list < std::string>::iterator it;
	std::map<std::string, std::function<void*(std::string, iTalkNPC* italk)>>::iterator raiserIt;
	for (it = list.begin(); it != list.end(); ++it)
	{
		//std::cout << *it << std::endl;
		raiserIt = levelRaiser.find(*it);
		if (raiserIt != levelRaiser.end())
		{
			if (talkingTo != NULL && talkingTo->getId() != "")
			{
				raiserIt->second(talkingTo->getId(), talkingTo);
				
				
				
			}
		}
	}
}

std::string Player::changeRoom(std::string direction, bool restriction)
{
	//retriction verifies if the player its able to move from more than one unity from his current position.
	// useful for loading and teleporting
	// if restricition is true, it means he's on regular play, otherwise, it can be in other state (e.g. load state)
	
	// default string value

	if (LevelUpManager::Instance().moveLevel < 1)
	{

		return "You try to move, but nothing happens.";
	}

	int currentRow		= currentMapRow;
	int currentColumm	= currentMapColumm;
	
	// default message if anything goes wrong ( Never knows...)
	std::string feedback = "I can't go there";

	if (pLevel->rooms[currentRow][currentColumm].endingLevelRoom)
	{
		if (pLevel->rooms[currentRow][currentColumm].endingDirection == direction)
		{
			feedback = "Entering new floor";

			pLevel->unloadLevel();
			pLevel->currentLevel++;
			pLevel->loadLevel(pLevel->currentLevel, pLevel->getXMLDocRef()->FirstChildElement("TextAdventure"),this);

			return feedback;
		}
	}

	if (direction == "SOUTH" || direction == "NORTH" || direction == "EAST" || direction == "WEST")
	{
		if (direction == "SOUTH")
		{
			currentMapRow--;
			feedback = "newRoom";
			if (currentMapRow < 0)
			{
				currentMapRow = 0;
				feedback = "There's just a wall there";
				return feedback;
			}
		}
		else if (direction == "NORTH")
		{
			currentMapRow++;
			feedback = "newRoom";

			if (currentMapRow > pLevel->getNumOfRows())
			{
				currentMapRow = pLevel->getNumOfRows();
				feedback = "There's just a wall there";
				return feedback;
			}
		}
		else if (direction == "WEST")
		{
			currentMapColumm--;
			feedback = "newRoom";

			if (currentMapColumm < 0)
			{
				currentMapColumm = 0;
				feedback = "There's just a wall there";
				return feedback;
			}
		}
		else if (direction == "EAST")
		{
			currentMapColumm++;
			feedback = "newRoom";

			if (currentMapColumm > pLevel->getNumOfColums())
			{
				currentMapColumm = pLevel->getNumOfColums();
				feedback = "There's just a wall there";
				return feedback;
			}
			
		}
		// accessibility levels
		if (pLevel->rooms[currentMapRow][currentMapColumm].state == -1)
		{
			feedback = "There's just a wall there";
			currentMapRow = currentRow;
			currentMapColumm = currentColumm;
		}
		if (pLevel->rooms[currentMapRow][currentMapColumm].state == 0)
		{
			feedback = "The path is locked.";
			currentMapRow = currentRow;
			currentMapColumm = currentColumm;
		}
		if (pLevel->rooms[currentMapRow][currentMapColumm].state == 3 && LevelUpManager::Instance().moveLevel < 2)
		{
			feedback = "The path is unreachable at the current move level.";
			currentMapRow = currentRow;
			currentMapColumm = currentColumm;
		}
		if (pLevel->rooms[currentMapRow][currentMapColumm].state == 4 )
		{
			feedback = "The path is too high.";
			currentMapRow = currentRow;
			currentMapColumm = currentColumm;
		}

	}

	if (feedback == "newRoom")
	{
		feedback = pLevel->rooms[currentMapRow][currentMapColumm].description["ENTRY"];
	}
	return feedback;

}

void Player::setLevelReference(Level& level)
{

	pLevel = &level;

}
void Player::modifyEntity(Italk* talkable, std::string string)
{




}

std::string Player::getId()
{


	return "Player";

}


bool Player::receiveAttack(Player* p)
{

	
	return false;

}
std::string Player::getCurrentstatus()
{
	std::string str = "";

	switch (state)
	{
	case 0:
		str = "Insert a command here ";
		break;
	case 1:
		str = "Insert talk command";
		break;
	case 2:
		str = "Insert attack command";
		break;
	}


	return str;
}

void Player::modifyStatus(int status)
{
	if (status == 0 || status == 1 || status == 2)
	{
		state = status; 
	}
}

void Player::useItem(std::list<std::string> stringListCommand)
{
	std::string item	= "";
	std::string object	= "";

	std::list<std::string>::iterator it;

	for (it = stringListCommand.begin(); it != stringListCommand.end(); ++it)
	{
		if ((*it) == "USE")
		{
			std::list<std::string>::iterator it2 = it;
			it2++;
			if (it2 != stringListCommand.end())
			{
				
				item = (*it2);
			}
		}
		else if ((*it) == "WITH" || (*it) == "ON")
		{
			std::list<std::string>::iterator it2 = it;
			it2++;
			if (it2 != stringListCommand.end())
			{
				
				object = (*it2);
			}
		}
	}
	// verify if the player tiped anything for item
	if (item == "")
	{

		std::cout << "Item is undefined." << std::endl;
		return;
	}
	else if (inventory.GetItem(item) == NULL) // verify if the item exists in the inventory
	{
		std::cout << "This item don't exist in the inventory" << std::endl;
		return;
	}
	else if (object == "") // verify if the player is using the item with something/someone
	{
		std::cout << "You can't use an item without something to use with!" << std::endl;
		return;
	}
	else
	{
		IItemInteractable * interactable = pLevel->rooms[currentMapRow][currentMapColumm].getInteractable(object);
		if (interactable != NULL)
		{

			std::cout << "You used the " << item << " with the "<< interactable->getId() << std::endl;
			interactable->unlockDoor(item, pLevel->rooms); // calls a function to try to unlock the door
			delete inventory.itemMap[item]; // remove from memory
			delete pLevel->rooms[currentMapRow][currentMapColumm].itens[object];
			pLevel->rooms[currentMapRow][currentMapColumm].itens.erase(object);
			
			inventory.itemMap.erase(item);

		}
		else
		{
			std::cout << "This doesn't seems right..." << std::endl;

		}
	}



}