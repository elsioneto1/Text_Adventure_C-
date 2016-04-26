#include "Item_Static.h"
#include "Room.h"


Item_Static::Item_Static()
{
}


Item_Static::~Item_Static()
{
}


bool Item_Static::pickable()
{
	return false;
}

std::string Item_Static::getId()
{
	return id;
}

void Item_Static::receiveInteraction(Item* item)
{

}

void Item_Static::setSolvings(int row, int columm, std::string solvingString)
{
	solvingRow		= row;
	solvingColumm	= columm;
	keyPass			= solvingString;
}

void Item_Static::unlockDoor(std::string key, Room** room)
{
	if (key == keyPass)
	{
		room[solvingRow][solvingColumm].state = 1;
		std::cout << "The path " << solvingRow << "  " << solvingColumm << " is able to move forward." << std::endl;
	}
}


void Item_Static::unlockDoor(std::string key, Room* room)
{

}