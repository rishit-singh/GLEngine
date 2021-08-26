#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <iterator>
#include <vector>
#include <string.h>


#define MAX_STRING_LENGTH 1000

static std::vector<std::string_view> QuickSplit(std::string_view& str, char splitChar)
{
	std::vector<std::string_view> splitStrings = std::vector<std::string_view>();\

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
			}

			if ((end + 1) == str.end())
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
		

int main()
{
	std::string_view str = "22/23";

	PrintVectorArray<std::string_view>(QuickSplit(str, '/'), 2);

	return 0;
}
