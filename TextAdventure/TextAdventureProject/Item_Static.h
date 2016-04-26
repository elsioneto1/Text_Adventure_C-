#ifndef _STATICITEM_H_
#define _STATICITEM_H_

#include "Item.h"
#include "IItemInteractable.h"

#pragma once

class Item_Static : public Item, public IItemInteractable
{
	

public:
	int solvingRow, solvingColumm;
	std::string keyPass;
	static inline Item_Static* _ItemStatic() {
		return new Item_Static();
	}
	Item_Static();
	~Item_Static();

	//void openPath

	bool pickable() override;
	std::string getId() override;
	void receiveInteraction(Item* item);
	void setSolvings(int row, int columm, std::string solvingString);


	void unlockDoor(std::string key, Room** room);
	void unlockDoor(std::string key, Room* room);

};

#endif