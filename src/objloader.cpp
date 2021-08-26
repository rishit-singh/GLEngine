#include "objloader.h"

std::vector<float> GLEngine::OBJLoader::LoadOBJFile(char* filePath)
{
	std::vector<float> vertexBufferArray = std::vector<float>();

	// std::ifstream filePointer = std::ifstream(filePath);

	std::string fileDump = FileIO::Read(filePath), line;

	std::vector<float> vertices = std::vector<float>(),
					textureCoordinates = std::vector<float>(),
					normals = std::vector<float>();

	std::vector<std::vector<int>>indexArrays = std::vector<std::vector<int>>();

	std::vector<std::string_view> fileSplit = StringTools::QuickSplit(fileDump, '\n'),
					faces = std::vector<std::string_view>(), 
					splitBuffer = std::vector<std::string_view>(),
					splitBuffer1 = std::vector<std::string_view>(),
					indices = std::vector<std::string_view>();

	std::vector<int> indexTemp = std::vector<int>(), quadIndices = {
		0, 1, 2,
		2, 3, 0
	}; 

	unsigned int sizeTemp = fileSplit.size(),
			sizeTemp1;

	
	// if (filePointer.is_open())
	// {
		for (int x = 0; x < sizeTemp; x++)
		{
			splitBuffer = StringTools::QuickSplit(fileSplit.at(x), ' ');

			// if (splitBuffer.back() == nullptr)
			// 	break;

			if (!splitBuffer[0].compare("v"))
			{
				for (int x = 0; x < 3; x++)
					vertices.push_back(General::StringViewToDouble(splitBuffer.at(x + 1)));
			}

			if (!splitBuffer[0].compare("vt"))
				for (int x = 0; x < 2; x++)
					textureCoordinates.push_back(General::StringViewToDouble(splitBuffer[x + 1]));

			// if (!splitBuffer.at(0).compare("vn"))	//	loads the normals
			// {
			// 	for (int x = 1; x < 4; x++)
			// 		vertexTemp.push_back(atof(splitBuffer.at(x).c_str()));

			// 	normals.push_back(vertexTemp);

			// 	vertexTemp = std::vector<float>();
			// }

			if (!splitBuffer[0].compare("f"))
			{
				for (int x = 0; x < 3; x++)
					faces.push_back(splitBuffer[x + 1]); 
			}
			
			line.clear();
		}

		// filePointer.close();
	// }

	sizeTemp = faces.size();

	float temp = 0;

	for (int x = 0; x < sizeTemp; x++)
	{
		splitBuffer = StringTools::QuickSplit(faces.at(x), '/');

		for (int i = 0; i < 3; i++)
			vertexBufferArray.push_back(vertices.at((3 * (General::StringViewToInt(splitBuffer[0]) - 1)) +  i));
		
		for (int i = 0; i < 2; i++)
			vertexBufferArray.push_back(textureCoordinates.at((2 * (General::StringViewToInt(splitBuffer[1]) - 1)) +  i));	
	}

	// General::PrintVectorArray<float>(vertexBufferArray, 6);

	return vertexBufferArray;
}