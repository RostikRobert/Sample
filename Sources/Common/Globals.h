#pragma once

#include <unordered_map>
#include <string>
#include "esUtils/esUtil.h"


class Globals
{
public:
	static const int screenWidth = 1280;
	static const int screenHeight = 768;

	static std::unordered_map< std::string, GLuint > Texures;
};
