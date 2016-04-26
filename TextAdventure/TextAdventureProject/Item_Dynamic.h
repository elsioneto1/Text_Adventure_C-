#ifndef _DYNAMICITEM_H_
#define _DYNAMICITEM_H_
#include "IItemInteractable.h"
#include "Item.h"

#pragma once
class Item_Dynamic : public Item
{
public:

	static inline Item_Dynamic* _DyanamicItem() {
		return new Item_Dynamic();
	}
	Item_Dynamic();
	~Item_Dynamic();

	bool pickable() override;
};

#endif