#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

#define MAX_STRING_LENGTH 1000


template<typename T>
static std::vector<T> CombineVectorArrays(std::vector<std::vector<T>> vectorArrays)
{
	std::vector<T> combinedArray = std::vector<T>();

	int sizeTemp = vectorArrays.size(), sizeTemp1;
	
	for (int x = 0; x < sizeTemp; x++)
	{
		sizeTemp1 = vectorArrays.at(x).size();

		for (int y = 0; y < sizeTemp1; y++)
			combinedArray.push_back(vectorArrays.at(x).at(y));
	}
	return combinedArray;
}


static std::vector<std::string> Split(const char* str, char splitChar)
{
	std::vector<std::string> splittedStrings = std::vector<std::string>();

	std::string tempString;

	unsigned int size = strlen(str);

	for (int x = 0; x < size; x++)
	{
		if (str[x] == splitChar)
		{
			splittedStrings.push_back(tempString);

			tempString.clear();

			continue;
		}

		if (x == (size - 1))
		{
			tempString += str[x];

			splittedStrings.push_back(tempString);

			tempString.clear();

			continue;
		}

		tempString += str[x];
	}

	return splittedStrings;
}

static std::vector<float> LoadOBJFile(char* filePath)
{
	std::vector<float> vertexBufferArray = std::vector<float>();

	std::ifstream filePointer = std::ifstream(filePath);

	std::string line;

	std::vector<std::vector<float>> vertices = std::vector<std::vector<float>>(),
							textureCoordinates = std::vector<std::vector<float>>(),
							normals = std::vector<std::vector<float>>();

	std::vector<std::string> faces = std::vector<std::string>(), 
					splitBuffer = std::vector<std::string>(),
					indices= std::vector<std::string>();

	std::vector<float> vertexTemp = std::vector<float>(); 

	unsigned int sizeTemp, sizeTemp1;


	if (filePointer.is_open())
	{
		while (std::getline(filePointer, line))
		{
			splitBuffer = Split(line.c_str(), ' ');

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


			if (!splitBuffer.at(0).compare("vn"))
			{
				for (int x = 1; x < 4; x++)
					vertexTemp.push_back(atof(splitBuffer.at(x).c_str()));

				normals.push_back(vertexTemp);

				vertexTemp = std::vector<float>();
			}

			if (!splitBuffer.at(0).compare("f"))
			{
				faces.push_back(line.substr(2, line.length()));
			}
			

			line.clear();
		}

		filePointer.close();
	}
	
	sizeTemp = faces.size();

	for (int x = 0; x < sizeTemp; x++)
	{
		splitBuffer = Split(faces.at(x).c_str(), ' ');

		sizeTemp1 = splitBuffer.size();

		for (int y = 0; y < sizeTemp1; y++)
		{
			indices = Split(splitBuffer.at(y).c_str(), '/');

			vertexBufferArray = CombineVectorArrays<float>({ vertexBufferArray, { vertices.at(atoi(indices.at(0).c_str()) - 1).at(0), vertices.at(atoi(indices.at(0).c_str()) - 1).at(1), vertices.at(atoi(indices.at(0).c_str()) - 1).at(2), textureCoordinates.at(atoi(indices.at(1).c_str()) - 1).at(0), textureCoordinates.at(atoi(indices.at(1).c_str()) - 1).at(1), normals.at(atoi(indices.at(2).c_str()) - 1).at(0), normals.at(atoi(indices.at(2).c_str()) - 1).at(1), normals.at(atoi(indices.at(2).c_str()) - 1).at(2) } });
			// vertexBufferArray = General::CombineVectorArrays<float>({ vertexBufferArray, Vertex3Df({ vertices.at(atoi(indices.at(0).c_str()) - 1).at(0), vertices.at(atoi(indices.at(0).c_str()) - 1).at(1), vertices.at(atoi(indices.at(0).c_str()) - 1).at(2) }, Point2Df(textureCoordinates.at(atoi(indices.at(1).c_str()) - 1).at(0), textureCoordinates.at(atoi(indices.at(1).c_str()) - 1).at(1)), { normals.at(atoi(indices.at(0).c_str()) - 1).at(0), normals.at(atoi(indices.at(0).c_str()) - 1).at(1), normals.at(atoi(indices.at(0).c_str()) - 1).at(2) }).GetSingleBuffer() });
		}
	}

	return vertexBufferArray;
}

int main()
{
	LoadOBJFile("/media/rishit/HDD0/src/repos/stable/GLEngine/resources/untitled.obj");
	// LoadOBJFile("./test.txt");

	// std::string str = "hello world!";

	// std::ifstream file = std::ifstream("/media/rishit/HDD0/src/repos/stable/GLEngine/resources/untitled.obj");

	// while (std::getline(file, str))
	// {
	// 	std::cout << Split(str.c_str(), ' ').at(0) << '\n';
	// }

	// file.close();



	return 0;
}
