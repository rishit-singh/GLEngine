
#pragma once

#define TOOLS_H

#include <iostream>
#include <initializer_list>
#include "gltypes.h"
#include "globjects.h"
#include "globals.h"

#define MAX_STRING_LENGTH 1000

namespace GLEngine
{
	class General
	{  
	public:
		static long Power(long num, long power)	//	Returns the provided power of the provided number. 
		{
			for (int x = 0; x < power; x++)
				num *= num;

			return num; 				
		}

		template<typename T>
		static void Swap(T* val, T* val1)
		{
			T* temp = val;

			val = val1; 
			val1 = temp; 
		}
		
		template<typename T>
		static void SetRange(T& value, T min, T max)	//	Sets the value to max or min if it exceeds the range
		{
			if (value < min)
				value = min;
				
			if (value > max)
				value = max;	
		}
		
		template<typename T>
		static void PrintArray(T* array, unsigned int size)
		{	
			for (int x = 0; x < size; x++) 
			{
				std::cout << array[x] << " ";
			}
		}
		
		template<typename T>
		static void PrintArray(std::vector<T> array)
		{	
			for (int x = 0; x < array.size(); x++) 
				std::cout << array.at(x);
		}

		
		template<typename T>
		static void PrintVectorArray(std::vector<T> array, int newlineNumber)
		{
			unsigned int size = array.size(); 

			for (int x  = 0; x < size; x++)
			{
				if (!(x % newlineNumber))
					std::cout << "\n\n";
				
				std::cout << array.at(x)
				<< ", ";	
			}
		}
		
		static void PrintVectorArray(std::vector<Vertex3Df> array)
		{ 
			for (int x = 0; x < array.size(); x++)
				std::cout << '\n' << x 
				<< "(" <<  array.at(x).Position.X
				<< ", " << array.at(x).Position.Y
				<< ", " << array.at(x).Position.Z
				<< "); ";
		}

		static void PrintArray(Vertex3Df* array, unsigned int size)
		{
			for (int x = 0; x < size; x++)
			{
				std::cout << "Position: "<< array[x].Position.X << ", " << array[x].Position.Y
						<< "\nColor: " << "R: " << array[x].VertexColor.R << "G: " << array[x].VertexColor.G  << "B: " << array[x].VertexColor.B << "A: " << array[x].VertexColor.A
						<< '\n';	
			}
		}

		static void PrintVertexFloatArray(float* array, unsigned int size)
		{
			for (int x = 0; x < size / 3; x++)
				std::cout << '\n' << x << " (X: " << array[x]
				 << ", Y: " <<  array[x + 1]
				 << ", Z: " <<  array[x + 2] <<  "); ";
		}

		static float* PointVectorToFloatArray(std::vector<Point3Df> pointVector)	//	Converts an std::vector<Point3Df> to a T* (probably a float or an int	) array	
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

		static float* PointArrayToFloatArray(Point3Df* pointArray, int size)	
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

		static float* VertexVectorToFloatArray(std::vector<Vertex3Df> vertexArray)	
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

		static float* VertexArrayToFloatArray(Vertex3Df* pointArray, int size)	
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

		static std::vector<float> VertexVectorToFloatVector(std::vector<Vertex3Df> vertices)
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

		template<typename T>
		static bool IsElement(T val, T* array, int size)
		{
			for (int x = 0; x < size; x++)
				if (array[x] == val)
					return true;

			return false;   
		}
		
		template<typename T>
		static bool IsElement(T val, std::vector<T> array, int size)
		{
			for (int x = 0; x < size; x++)
				if (array.at(x) == val)
					return true;

			return false;   
		}

		template<typename T>
		std::vector<T> GetMaxArray(std::vector<T>& array, std::vector<T>& array1)	//	Returns the array with a greater size among the both provided arrays.
		{
			return ((array.size() > array1.size()) ? array : array1);
		}

		template<typename T>
		static void CopyArray(T* destination, T* source, unsigned int size)
		{
			for (int x = 0; x < size; x++)
				destination[x] = source[x];
		}
		// static float* VertexArrayToFloatArray(Vertex3Df* vertexArray, int size)
		// {	
		// 	float* array = new float[size * 3]; 

		// 	for (int x = 0; x < size; x++)
		// 	{
		// 		array[x] = vertexArray[x].Position.X;
		// 		array[x + 1] = vertexArray[x].Position.Y;
		// 		array[x + 2] = vertexArray[x].Position.Z;
		// 	}

		// 	return array;
		// }

		template<typename T>
		static std::vector<T> ArrayToVector(T* array, unsigned int size)
		{
			std::vector<T> vectorTemp = std::vector<T>();

			for (int x = 0; x < size; x++)
				vectorTemp.push_back(array[x]);

			return vectorTemp;
		}

		template<typename T>
		static std::vector<T> GetRangeArray(T max)	//	Creates an array containing ints within the provided range
		{
			std::vector<T> NumberStack = std::vector<T>();

			for (T x = 0; x < max; x++)
				NumberStack.push_back(x);

			return NumberStack;
		}
		
		template<typename T>
		static std::vector<T> GetRangeArray(T min, T max)	//	Creates an array containing ints within the provided range
		{
			std::vector<T> NumberStack = std::vector<T>();

			if (min > max)
				return NumberStack;	//	null array

			for (T x = min; x < max; x++)
				NumberStack.push_back(x);

			return NumberStack;
		}		
		
		template<typename T>
		static std::vector<T> GetRangeArray(T min, T max, T increament)	//	Creates an array containing ints within the provided range with the difference of the provided increament variable
		{
			std::vector<T> NumberStack = std::vector<T>();

			if (min > max)
				return NumberStack;	//	null array

			for (T x = min; x < max; x += increament)
				NumberStack.push_back(x);

			return NumberStack;
		}

		template<typename T>
		static T* VectorToArray(std::vector<T> vectorArray)
		{
			T* Array = new T[vectorArray.size()];
		}
		
		static bool InRange(int num, int start, int end)
		{
			return (num >= start || num <= end);
		}

		static std::vector<int> GetDigitArray(int num)	//	returns a std::vector<int> array containing the digits of the provided number as elements
		{
			std::vector<int> DigitArray = std::vector<int>();

			int Temp = NULL;

			for (int x = 1; (Temp = (int)(num / x)); x *= 10)
				DigitArray.push_back(Temp); 
			
			Temp = DigitArray.size();

			for (int x = 0; x < Temp; x++)
				DigitArray.at(x) = (int)((int)DigitArray.at(x) - ((int)DigitArray.at(x + 1) * General::Power(10, x + 1)));

			return DigitArray; 
		}

		template<typename T>
		static std::vector<T> CombineVectorArrays(std::vector<T> array, std::vector<T> array1)
		{
			int sizeTemp = array1.size();
			
			for (int x = 0; x < sizeTemp; x++)
				array.push_back(array1.at(x));

			return array;
		}

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

		// template<typename T>
		// static std::vector<T> GetCommonElements()
		// {
			
		// }

		// static std::vector<GLEngine::Point3Df> GetCommonElements()
		// {
		// 	std::vector<GLEngine::Point3Df> CommonElements = std::vector<GLEngine::Point3Df>();

		// 	// for ()

		// 	return CommonElements;
		// }
		

		static String IntToString(int num)	//	Converts provided integer to string. 
		{
			String IntegerString = new char[MAX_STRING_LENGTH];
			char charTemp[1]; 

			std::vector<int> DigitArray = General::GetDigitArray(num);	//	gets the digit array

			int size = DigitArray.size();

			for (int x = 0; x < size; x++)
				strcat(IntegerString, &(charTemp[0] = (char)(DigitArray.at(x) + 48)));
			
			return IntegerString; 
		}
	};

	class StringTools
	{
	public:
		// static std::vector<char*> Split(char* str, char splitChar)
		// {
		// 	std::vector<char*> splittedStrings = std::vector<char*>();

		// 	char* bufferTemp = new char[MAX_STRING_LENGTH];

		// 	unsigned int size = strlen(str);

		// 	for (int x = 0; x < size; x++)
		// 	{
		// 		if (str[x] == splitChar || x == (size - 1))
		// 		{
		// 			splittedStrings.push_back(strcat(new char[MAX_STRING_LENGTH], bufferTemp));

		// 			memset(bufferTemp, 0, MAX_STRING_LENGTH);	//	nulls out the buffer

		// 			continue;
		// 		}

		// 		bufferTemp[x] = str[x];
		// 	}

		// 	delete bufferTemp;

		// 	return splittedStrings;
		// }

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
	};

	class Algorithms 
	{
	public: 	
		template<typename T>
		static void BubbleSort(T* array, int size)  //  Sorts the provided array
		{
			for (int x = 0; x < size - 1; x++)
				for (int y = 0;  y < size - x - 1; x++)
					if (array[y] > array[y + 1])
						General::Swap(&array[y], &array[y + 1]); 
		}

		template<typename T>
		static void BubbleSort(std::vector<T> array)    //  Sorts the provided std::vector<T>/array
		{
			int size = array.size();

			for (int x = 0; x < size - 1; x++)
				for (int y = 0;  y < size - x - 1; x++)
					if (array.at(y) > array.at(y + 1))
						General::Swap(&array.at(y), &array.at(y + 1)); 
		}

		template<typename T>
		static int BinarySearch(T val, T* array, int start, int end)	//	Binary search implementation for T array
		{
			int mid = start + end - 1 / 2;

			if (start <= end)
			{
				if (array[mid] == val)
					return array[mid]; 
				
				if (array[mid] > val)
					return Algorithms::BinarySearch<T>(val, array, mid, end);
				
				if (array[mid] < val)
					return Algorithms::BinarySearch<T>(val, array, start, mid); 
			}

			return -1; 
		}

		template<typename T>
		static int BinarySearch(T val, std::vector<T> array, int start, int end)	//	Binary search implementation for std::vector<T>
		{
			int mid = start + end - 1 / 2;

			if (start <= end)
			{
				if (array.at(mid) == val)
					return array.at(mid); 
				
				if (array.at(mid) > val)
					return Algorithms::BinarySearch<T>(val, array, mid, end);
				
				if (array.at(mid) < val)
					return Algorithms::BinarySearch<T>(val, array, start, mid); 
			}

			return -1; 
		}

		template<typename T>
		static int BinarySearch(T val, std::vector<T> array)	//	Binary search implementation for std::vector<T>, 1 instruction slower than the traditional implementation
		{
			int start = 0, end = array.size(),  mid = start + end - 1 / 2;

			if (start <= end)
			{
				if (array.at(mid) == val)
					return array.at(mid); 
				
				if (array.at(mid) > val)
					return Algorithms::BinarySearch<T>(val, array, mid, end);
				
				if (array.at(mid) < val)
					return Algorithms::BinarySearch<T>(val, array, start, mid); 
			}

			return -1; 
		}

		template<typename T>
		static int GetIndex(T* val, T* array, int size)		//	Finds the index of the provided value (if exists) in the array
		{
			return Algorithms::BinarySearch(val, array, 0, size); 
		}
		
		template<typename T>
		static int GetIndex(T* val, std::vector<T> array)		//	Finds the index of the provided value (if exists) in the std::vector<T>/array
		{
			return Algorithms::BinarySearch(val, array, 0, array.size()); 
		}

		template<typename T>
		static bool IsElement(T val, T* array, int size)	//	Checks if the given element is present in the given array, returns a bool represnting the same
		{
			return (Algorithms::BinarySearch(val, array, 0, size) >= 0);	
		} 
	
		template<typename T>
		static bool IsElement(T val, std::vector<T> array)	//	Checks if the given element is present in the given array, returns a bool represnting the same
		{
			return (Algorithms::BinarySearch(val, array, 0, array.size()) >= 0);	
		}
	}; 
}
