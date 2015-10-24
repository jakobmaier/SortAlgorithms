
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "PerformanceMonitor.h"
#include "SortUtils.h"

enum Status {
	STATUS_SUCCESS,
	STATUS_ERROR
};

// Reads a file of integers
Status readFile(const char* path, std::vector<int>& numbers)
{
	std::ifstream inputStream;
	inputStream.open(path);
	if(!inputStream.is_open()) {
		std::cerr << "Input file could not be opened: '" << path << "'" << std::endl;
		return STATUS_ERROR;
	}

	
	int number;
	while(inputStream >> number) {
		numbers.push_back(number);
	}

	inputStream.close();
	
	return STATUS_SUCCESS;
}

void exitWithError()
{
	std::cerr << "Execution failed" << std::endl;
	std::cin.ignore();
	exit(1);
}



int main(int argc, char* argv[])
{
	const char* inputFilePath = { "Resources/numbers.txt" };
	PerformanceMonitor monitor;
	
	std::vector<int> numbers;
	numbers.reserve(1000000);
	std::cout << "Reading file..." << std::endl;
	if(readFile(inputFilePath, numbers) != STATUS_SUCCESS) {
		exitWithError();
	}
	std::cout << "Processing " << numbers.size() << " integers" << std::endl;
	std::random_shuffle(numbers.begin(), numbers.end());
		
	monitor.start();
	//std::sort(numbers.begin(), numbers.end());
	utils::quickSort(&(numbers[0]), numbers.size());
	monitor.stop();
	
	//Check:
	std::cout << "Checking result..." << std::endl;
	int lastNum = numbers[0];
	for(int num : numbers) {
		//std::cout << num << std::endl;
		if(num < lastNum) {
			std::cerr << "Sort failed "<<num << std::endl;
			exitWithError();
		}
		lastNum = num;
	}
	std::cerr << "Sort succeeded" << std::endl;

	std::cout << "Execution time: " << monitor.toString() << std::endl;
	std::cin.ignore();
}
