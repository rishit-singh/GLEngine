#ifndef MATRIX_H 

#include <iostream>
#include "globjects.h"
#include "exceptions.h"

namespace GLEngine
{
	class InvalidRowStrideException : GLException
	{	
	public:
		char* Message;
	
		void LogExceptionMessage();
		
		InvalidRowStrideException() : Message("The row size doesn't match trhe stride.")
		{
		}

		InvalidRowStrideException(char* message) : Message(message)
		{
		}												
	};

	class InvalidRowLengthException : GLException
	{	
	public:
		char* Message;
		
		void LogExceptionMessage();
		
		InvalidRowLengthException() : Message("The row size can't divide the matrix array into rows of equal sizes, accessing some elements may lead to segmentation fault.")		
		{
		}

		InvalidRowLengthException(char* message) : Message(message)
		{
		}												
	};

	class InvalidRowMetricsException : GLException
	{	
	public:
		char* Message;
		
		void LogExceptionMessage();
		
		InvalidRowMetricsException() : Message("The row length or stride are invalid.")		
		{
		}

		InvalidRowMetricsException(char* message) : Message(message)
		{
		}												
	};

	// void InvalidRowMetricsException::LogExceptionMessage()
	// {
	// 	Debug->Log(this->Message);
	// }

	// void InvalidRowLengthException::LogExceptionMessage()
	// {
	// 	Debug->Log(this->Message);
	// }

	// void InvalidRowStrideException::LogExceptionMessage()
	// {
	// 	Debug->Log(this->Message);
	// }

	template<typename T> 
	struct Matrix
	{
		std::vector<T> VectorArray;
		int Stride; 

		Matrix() : VectorArray(std::vector<T>()), Stride(0)
		{
		}

		Matrix(T* array, int size, int stride) : VectorArray(std::vector<T>()), Stride(stride)
		{
			Debug->Log(size);
			Debug->Log(stride);
			Debug->Log(size % stride);

			try
			{
				if ((size % stride) != 0)
					throw new InvalidRowLengthException();
			}
			catch (InvalidRowLengthException& e)
			{
				e.LogExceptionMessage();
			}

			for (int x = 0; x < size; x++)
				this->VectorArray.push_back(array[x]); 
		}
 
 
		Matrix(std::vector<T> vectorArray, int stride) : VectorArray(vectorArray), Stride(stride)
		{
			try
			{
				if ((vectorArray.size() % stride) != 0)
					throw new InvalidRowLengthException();
			}
			catch (InvalidRowLengthException& e)
			{
				e.LogExceptionMessage();
			}
		}
	}; 

	class MatrixOperations	//	contains static functions to work with matrix arrays
	{
	public:
		//	Checks if the provided row array's elements matches a the elements of other rows in the matrix array
		template<typename T>
		static bool IsRow(std::vector<T> rowVectorArray, Matrix<T> matrix)	
		{
			try
			{
				if (rowVectorArray.size() != matrix.Stride)
					throw new InvalidRowLengthException();
			}
			catch (GLException& e)
			{	
				e.LogExceptionMessage(); 

				return false;			
			}

			for (int x = 0; x < (matrix.VectorArray.size() / matrix.Stride); x++)
				if (rowVectorArray.at(0) == matrix.VectorArray.at((matrix.Stride * 3)) && rowVectorArray.at(1) == matrix.VectorArray.at((matrix.Stride * 3) + 1) && rowVectorArray.at(2) == matrix.VectorArray.at((matrix.Stride * 3) + 2))
					return true;

			return false; 
		}

		template<typename T>
		static int AtMatrixRowIndex(std::vector<T> rowVectorArray, Matrix<T> matrix, std::vector<unsigned int> indices)	//	returns the index stored in the provided index array to which the provided row is matched
		{
			int size, indexArraySize = indices.size();
			try
			{
				if ((matrix.VectorArray.size() % rowVectorArray.size()) != 0)	
					throw new InvalidRowLengthException();
			}
			catch (InvalidRowLengthException& e) 
			{
				e.LogExceptionMessage(); 
				
				return -1;
			}
			
			unsigned int indexTemp; 

			for (int y = 0; y < indexArraySize; y++)
				try
				{	
					if (rowVectorArray.at(0) == matrix.VectorArray.at((indexTemp = (matrix.Stride * indices.at(y)))) && rowVectorArray.at(1) == matrix.VectorArray.at((matrix.Stride * indices.at(y)) + 1) && rowVectorArray.at(2) == matrix.VectorArray.at((matrix.Stride * indices.at(y)) + 2))
						{
						Debug->Log(indexTemp); 

						return y; 
					}
				}
				catch (std::out_of_range& e)
				{
					Debug->Log("Index out of range.");

					return -1; 
				}
		}

		template<typename T>
		static bool IsDuplicate(std::vector<T> rowVectorArray, Matrix<T> matrix, std::vector<T> indices)
		{
			return (MatrixOperations::AtMatrixRowIndex<T>(rowVectorArray, matrix, indices) != -1);
		}
	};
}; 

#endif
