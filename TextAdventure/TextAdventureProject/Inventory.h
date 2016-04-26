#ifndef _INVENTORY_H_
#define _INVENTORY_H_

#include <map>
#include "Item.h"

#pragma once
class Inventory
{
public:
	std::map<std::string , Item*> itemMap;

	Inventory();
	~Inventory();

	void AddItem(Item* item);
	void RemoveItem(Item* item);
	void showInventory();
	Item* GetItem(std::string key);
	void clearItems();
};

#endif