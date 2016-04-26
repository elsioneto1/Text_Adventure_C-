#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "..\externallibs\tinyXML\tinyxml2.h"
#pragma once

using namespace tinyxml2;
class Entity
{
public:
	int life;

	Entity();
	~Entity();
};

#endif