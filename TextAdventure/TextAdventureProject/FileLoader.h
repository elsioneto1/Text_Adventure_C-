#ifndef _FILELOADER_H_
#define _FILELOADER_H_
#include <iostream>
#include <string>

#include "../ExternalLibs/tinyXML/tinyxml2.h"



using namespace tinyxml2;

#pragma once
class FileLoader
{
public:
	
	static FileLoader& Instance()
	{
		if (instance == NULL)
		{
			instance = new FileLoader();
		}
		return *instance;
	}

	// return XMLELEMENT pointer
	XMLElement* loadXML(std::string xmlName);

	FileLoader();
	~FileLoader();

private :
	static FileLoader *instance;

};

#endif