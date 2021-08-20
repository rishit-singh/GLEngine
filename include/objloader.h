#ifndef OBJLOADER_H

#include <fstream>
#include "fileio.h"
#include "tools.h"

namespace GLEngine
{
	class OBJLoader
	{
	public:
		static std::vector<float> LoadOBJFile(char* filePath)
		{
			std::vector<float> vertexBufferArray = std::vector<float>();

			std::ifstream filePointer = std::ifstream(filePath);

			std::string line;

			std::vector<std::vector<float>> vertices = std::vector<std::vector<float>>(),
									textureCoordinates = std::vector<std::vector<float>>(),
									normals = std::vector<std::vector<float>>();

			std::vector<std::vector<int>>indexArrays = std::vector<std::vector<int>>();

			std::vector<std::string> faces = std::vector<std::string>(), 
							splitBuffer = std::vector<std::string>(),
							splitBuffer1 = std::vector<std::string>(),
							indices = std::vector<std::string>();

			std::vector<float> vertexTemp = std::vector<float>();
		
			std::vector<int> indexTemp = std::vector<int>(), quadIndices = {
				0, 1, 2,
				2, 3, 0
			}; 

			unsigned int sizeTemp, sizeTemp1;


			if (filePointer.is_open())
			{
				while (std::getline(filePointer, line))
				{
					splitBuffer = StringTools::Split(line.c_str(), ' ');

					if (!splitBuffer.at(0).compare("v"))
					{
						for (int x = 1; x < 4; x++)
							vertexTemp.push_back(atof(splitBuffer.at(x).c_str()));

						vertices.push_back(vertexTemp);

						vertexTemp = std::vector<float>();
					}

					if (!splitBuffer.at(0).compare("vt"))
					{
						for (int x = 1; x < 3; x++)
							vertexTemp.push_back(atof(splitBuffer.at(x).c_str()));

						textureCoordinates.push_back(vertexTemp);

						vertexTemp = std::vector<float>();
					}

					// if (!splitBuffer.at(0).compare("vn"))	//	loads the normals
					// {
					// 	for (int x = 1; x < 4; x++)
					// 		vertexTemp.push_back(atof(splitBuffer.at(x).c_str()));

					// 	normals.push_back(vertexTemp);

					// 	vertexTemp = std::vector<float>();
					// }

					if (!splitBuffer.at(0).compare("f"))
					{
						splitBuffer1 = StringTools::Split(line.substr(2, line.length()).c_str(), ' ');	
					
						sizeTemp = splitBuffer1.size();

						for (int x = 0; x < sizeTemp; x++)
							indices.push_back(splitBuffer1.at(x));
					}
					
					line.clear();
				}


				filePointer.close();
			}

			sizeTemp = faces.size();

			float temp = 0;

			for (int x = 0; x < sizeTemp; x++)
			{
				splitBuffer = StringTools::Split(faces.at(x).c_str(), ' ');

				sizeTemp1 = splitBuffer.size();

				for (int y = 0; y < sizeTemp1; y++)
				{
					General::CombineVectorArrays<std::string>({ indices, StringTools::Split(splitBuffer.at(y).c_str(), '/') });

					vertexBufferArray = General::CombineVectorArrays<float>({ vertexBufferArray, vertexTemp });
				}
			}

			sizeTemp = indices.size();	

			for (int x = 0; x < sizeTemp; x++)
			{
				std::vector<std::string> face = StringTools::Split(indices.at(x).c_str(), '/');

				for (int i = 0; i < 3; i++)
				{
					vertexTemp.push_back(temp = vertices.at(atoi(face.at(0).c_str()) - 1).at(i)); 
					// std::cout << temp << ", ";
				}
				for (int i = 0; i < 2; i++)
				{
					vertexTemp.push_back(temp = textureCoordinates.at(atoi(face.at(1).c_str()) - 1).at(i)); 				
 
					// std::cout << temp << '\n'; 
				}	

				vertexBufferArray = General::CombineVectorArrays<float>({ vertexBufferArray, vertexTemp });
			}

			// General::PrintVectorArray<float>(vertexBufferArray, 6);

			return vertexBufferArray;
		}
		
	};
}; 

#endif