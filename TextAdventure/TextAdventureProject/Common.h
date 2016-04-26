#ifndef _COMMON_H_
#define _COMMON_H_

#include "Item.h"

#pragma once
class Common : public Item
{
public:
	Common();
	~Common();

	void use();
};

#endif