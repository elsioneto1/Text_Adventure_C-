#include "Room.h"
#include "NPC.h"
#include "Player.h"


NPC::NPC()
{
	dialogs["TESTE"]	= "I am alive";

}

NPC::NPC(std::string id)
	: id(id)
{

}

NPC::~NPC()
{
}

void NPC::modifyEntity(Italk* talkable, std::string string)
{
	if (string == "BYE")
	{
		std::cout << dialogs[string] << std::endl;
		Player *t = dynamic_cast<Player*>(talkable);
		t->modifyStatus(0);
	}
}

void NPC::initialize(XMLElement* node)
{

}

std::string NPC::getStatement(std::string key)
{
	// get dialog message from npc
	std::string returnKey = defaultMessage;
	std::map<std::string, std::string>::iterator it;

	for (it = dialogs.begin(); it != dialogs.end(); ++it)
	{
		if (it->first == key)
		{
			returnKey = it->second;
		}
	}


	return returnKey;
}


std::string NPC::getUnlockStatement(std::string key)
{
	// get dialog message from npc
	std::string returnKey = defaultMessage;
	std::map<std::string, std::string>::iterator it;

	for (it = unlockDialogs.begin(); it != unlockDialogs.end(); ++it)
	{
		if (it->first == key)
		{
			returnKey = it->second;
		}
	}


	return returnKey;
}




std::string NPC::getId()
{
	return id;
}

void NPC::receiveInteraction(Item* item)
{

}

void NPC::setSolvings(int row, int columm, std::string solvingString)
{
	solvingRow		= row;
	solvingColumm	= columm;
	keyPass			= solvingString;
}

void NPC::unlockDoor(std::string key, Room* room)
{
	
		room->state = 1;
		std::cout << "The path " << solvingRow << "  " << solvingColumm << " is able to move forward." << std::endl;
	
}
void NPC::unlockDoor(std::string key, Room** room)
{
	if (key == itemResolving)
	{
		room[solvingRow][solvingColumm].state = 1;
		std::cout << "The path " << solvingRow << "  " << solvingColumm << " is able to move forward." << std::endl;
	}
	else
	{
		std::cout << "This is not what I'm looking for." << std::endl;
	}
}

void* NPC::unlockDoorWithWord(NPC* npc, std::string key, Room** room)
{
	if (npc->keyPass == key) // verify if the item used is the one that activates the event
	{
		room[npc->solvingRow][npc->solvingColumm].state = 1;
		std::cout << "The path " << npc->solvingRow << "  " << npc->solvingColumm << " is able to move forward." << std::endl;
	}
	
	return NULL;
}

bool NPC::accessUnlockMap(std::string key, Room** room)
{

	bool returnBool = false;
	std::map <std::string, std::function<void*(NPC* npc, std::string keyPass, Room** room)>>::iterator it;
	for (it = unlockMap.begin(); it != unlockMap.end(); ++it)
	{
		if (it->first == key)
		{
			unlockMap.find(key)->second(this, key, room);
			returnBool = true;
		}
	}

	return returnBool;
}

bool NPC::isFightable()
{
	return false;
}