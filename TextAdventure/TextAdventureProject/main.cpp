#include <iostream>

#include "GameLogic.h"
#include "FileLoader.h"

void main()
{
	GameLogic::Instance().initialize();
	GameLogic::Instance().GameLoop();


}