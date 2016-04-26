#ifndef _INTERACTABLEINTERFACE_H_
#define _INTERACTABLEINTERFACE_H_
#pragma once

#include <string>

class Room;
class Item;
class IItemInteractable
{
public:

	virtual void receiveInteraction(Item * item) = 0; // receives an item and verify if its setted the interaction to do so
	virtual std::string getId() = 0 ; // returns the entity ID
	virtual void unlockDoor(std::string key, Room* room) = 0; // open the doors
	virtual void unlockDoor(std::string key, Room** room) = 0; // open the doors
	virtual void setSolvings(int row,int columm, std::string solvingString) = 0; // sets which room is going to be unlocked and what keyword it expected to unlock it

};

#endif