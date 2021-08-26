#include "tools.h"

void GLEngine::General::PrintVectorArray(std::vector<GLEngine::Vertex3Df> array)	//	Overload for Vertex3Df
{ 
	for (int x = 0; x < array.size(); x++)
		std::cout << '\n' << x 
		<< "(" <<  array.at(x).Position.X
		<< ", " << array.at(x).Position.Y
		<< ", " << array.at(x).Position.Z
		<< "); ";
}

void GLEngine::General::PrintVectorArray(std::vector<std::string_view> stringViews)
{
	int size = stringViews.size();

	for (int x = 0; x < size; x++)
		std::cout << stringViews.at(x) << '\n';
}

void GLEngine::General::PrintArray(GLEngine::Vertex3Df* array, unsigned int size)
{
	for (int x = 0; x < size; x++)
	{
		std::cout << "Position: "<< array[x].Position.X << ", " << array[x].Position.Y
				<< "\nColor: " << "R: " << array[x].VertexColor.R << "G: " << array[x].VertexColor.G  << "B: " << array[x].VertexColor.B << "A: " << array[x].VertexColor.A
				<< '\n';	
	}
}

void GLEngine::General::PrintVertexFloatArray(float* array, unsigned int size)
{
	for (int x = 0; x < size / 3; x++)
		std::cout << '\n' << x << " (X: " << array[x]
			<< ", Y: " <<  array[x + 1]
			<< ", Z: " <<  array[x + 2] <<  "); ";
}

float* GLEngine::General::PointVectorToFloatArray(std::vector<GLEngine::Point3Df> pointVector)	//	Converts an std::vector<Point3Df> to a T* (probably a float or an int	) array	
{
	size_t VectorSize = pointVector.size();

	float* Array = new float[VectorSize * 3];

	for (int x = 0; x < VectorSize; x += 3)	//	Sets up the matrix rows
	{
		Array[x] = pointVector.at(x).X;  
		Array[x + 1] = pointVector.at(x).Y;  
		Array[x + 2] = pointVector.at(x).Z;
	}

	return Array;	
}


float* GLEngine::General::PointArrayToFloatArray(GLEngine::Point3Df* pointArray, int size)	
{
	float* floatArray = new float[size * 3]; 

	for (int x = 0; x < size; x += 3)	
	{
		floatArray[x] = pointArray->X;
		floatArray[x + 1] = pointArray->Y;
		floatArray[x + 2] = pointArray->Z;
	}

	return floatArray;
}

float* GLEngine::General::VertexVectorToFloatArray(std::vector<GLEngine::Vertex3Df> vertexArray)	
{
	int size;

	float* floatArray = new float[(size = vertexArray.size()) * 5]; 

	for (int x = 0; x < size; x++)	
	{
		floatArray[x + ((size - 1) * x)] = vertexArray.at(x).Position.X;
		floatArray[(x + ((size - 1) * x)) + 1] = vertexArray.at(x).Position.Y;
		floatArray[(x + ((size - 1) * x)) + 2] = vertexArray.at(x).Position.Z;

		floatArray[x + ((size - 1) * x) + 4] = vertexArray.at(x).TextureCoordinates.X;
		floatArray[x + ((size - 1) * x) + 5] = vertexArray.at(x).TextureCoordinates.Y;
	}

	return floatArray;
}


float* GLEngine::General::VertexArrayToFloatArray(GLEngine::Vertex3Df* pointArray, int size)	
{
	float* floatArray = new float[size * 3]; 

	for (int x = 0; x < size; x += 3)	
	{
		floatArray[x] = pointArray->Position.X;
		floatArray[x + 1] = pointArray->Position.Y;
		floatArray[x + 2] = pointArray->Position.Z;
	}

	return floatArray;
}	

std::vector<float> GLEngine::General::VertexVectorToFloatVector(std::vector<GLEngine::Vertex3Df> vertices)
{
	int size = vertices.size();

	std::vector<float> floatVector = std::vector<float>();

	for (int x = 0; x < size; x++)
	{
		floatVector.push_back(vertices.at(x).Position.X);	
		floatVector.push_back(vertices.at(x).Position.Y);	
		floatVector.push_back(vertices.at(x).Position.Z);	
		floatVector.push_back(vertices.at(x).TextureCoordinates.X);	
		floatVector.push_back(vertices.at(x).TextureCoordinates.Y);	
	}

	return floatVector;
}

int GLEngine::General::StringViewToInt(std::string_view strView)
{
	int dbl;

	std::from_chars(strView.data(), strView.data() + strView.size(), dbl);

	return dbl;
}

double GLEngine::General::StringViewToDouble(std::string_view strView)
{
	double dbl; 

	std::from_chars(strView.data(), strView.data() + strView.size(), dbl);

	return dbl;
}


std::vector<std::string> GLEngine::StringTools::Split(const char* str, char splitChar)
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


std::vector<std::string_view> GLEngine::StringTools::QuickSplit(std::string& str, char splitChar)
{
	std::vector<std::string_view> splitStrings = std::vector<std::string_view>();

	std::string::iterator start = str.begin(),
		end = start;

		while (end != str.end())
		{
			if (*end == splitChar)
			{
				splitStrings.push_back(std::string_view(start, end));
				
				start = end + 1;//(int)((end + 1) != str.end()); 		
			}
		
			if ((end  + 1) == str.end())
			{
				splitStrings.push_back(std::string_view(start, end));
				
				start = end;	
			}

			end++;
		}

		if (splitStrings.size() == 0)
			splitStrings.push_back(std::string_view(start, end));

	return splitStrings;
}

std::vector<std::string_view> GLEngine::StringTools::QuickSplit(std::string_view& str, char splitChar)
{
	std::vector<std::string_view> splitStrings = std::vector<std::string_view>();

	std::string_view::iterator start = str.begin(),
		end = start;


		while (end != str.end())
		{
			if (*end == splitChar)
			{
				splitStrings.push_back(std::string_view(start, end));
				
				start = end + 1; 		
			}

			const char* temp;

			if ((end + 1) == str.end() && *(end + 1) != splitChar)	//	to prevent it from skipping the last element
			{
				splitStrings.push_back(std::string_view(start, end + 1));
						
				start = end;	

				Debug->Log<const char*>("temp", temp);
			}

			if ((end + 1) == str.end())
			{
				splitStrings.push_back(std::string_view(start, end));
						
				start = end;	

				Debug->Log<const char*>("temp", temp);
			}



			end++;
		}

		if (splitStrings.size() == 0)
			splitStrings.push_back(std::string_view(start, end));

	return splitStrings;
}

