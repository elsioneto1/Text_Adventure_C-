#include "FileLoader.h"


FileLoader* FileLoader::instance = NULL;

FileLoader::FileLoader()
{
}


FileLoader::~FileLoader()
{
}

XMLElement* FileLoader::loadXML(std::string xmlName)
{
	XMLElement* xml = NULL;

	std::cout << xmlName << std::endl;

	return xml;
}
