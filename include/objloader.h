#ifndef OBJLOADER_H

#include <fstream>
#include <charconv>
#include "fileio.h"
#include "tools.h"

namespace GLEngine
{
	class OBJLoader
	{
	public:
		static std::vector<float> LoadOBJFile(char*);
	};
}; 

#endif
