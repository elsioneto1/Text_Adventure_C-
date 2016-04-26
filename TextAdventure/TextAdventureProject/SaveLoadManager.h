#ifndef _SAVELOADMANAGER_H_
#define _SAVELOADMANAGER_H_
#include <iostream>
#pragma once
class SaveLoadManager
{
	static SaveLoadManager& Instance()
	{
		if (instance == NULL)
		{
			instance = new SaveLoadManager();
		}
		return *instance;
	}

private:
	static SaveLoadManager* instance;

};

#endif