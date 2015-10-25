
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "PerformanceMonitor.h"
#include "QuickSort.h"


const bool readNumbersFromFile = false;


enum Status {
	STATUS_SUCCESS,
	STATUS_ERROR
};

// Reads a file of integers
Status readFile(const char* path, std::vector<int>& numbers) {
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


void exitWithError() {
	std::cerr << "Execution failed" << std::endl;
	std::cin.ignore();
	exit(1);
}

// Checks if a given vector is actually sorted
void checkSortResult(const std::vector<int>& numbers) {
	std::cout << "Checking result..." << std::endl;
	int invalid = utils::isSorted(&(numbers[0]), numbers.size());
	if(invalid == -1) {
		std::cerr << "Sort succeeded" << std::endl;
	} else {
		std::cerr << "Sort failed at index " << invalid << std::endl;
	}
}

//// Checks the nthElement algorithm for its correctnes
//void checkNthElementAlgo() {
//	const int count = 2000;
//	std::cout << "Checking nthElement algorithm with " << count << " numbers..." << std::endl;
//
//	std::vector<int> numbers;
//	for(int i = 0; i < count; ++i) {
//		numbers.push_back(i);
//		if(i % 10 == 0) { // duplicate numbers
//			numbers.push_back(i);
//		}
//	}
//	assert(utils::isSorted(&(numbers[0]), numbers.size()) == -1);
//
//	std::vector<int> shuffled = numbers;
//	std::random_shuffle(shuffled.begin(), shuffled.end());
//
//	for(unsigned int i = 0; i < numbers.size(); ++i) {
//		shuffled = numbers;
//		int idx = utils::nthElement(&(shuffled[0]), shuffled.size(), i+1);
//		int val = shuffled[idx];
//		
//		//std::cout << i << "th element is at index " << idx << " and has value " << val << std::endl;
//		if(val != numbers[i]) {
//			std::cerr << "nth-element failed for n = " << i << std::endl;
//			return;
//		}
//		std::random_shuffle(shuffled.begin(), shuffled.end());
//	}
//	std::cerr << "nth-element is working correctly" << std::endl;
//}

/**
 * Generates a list of random numbers for testing
 */
std::vector<int> generateRandomNumbers(int count, int min, int max) {
	std::cout << "Generating " << count << " random numbers between " << min << " and " << max << "..." << std::endl;
	std::vector<int> numbers;
	numbers.reserve(count);
	for(int i = 0; i < count; ++i) {
		numbers.push_back(utils::randRange(min, max));
	}
	return numbers;
}

int main() {
	srand(static_cast<unsigned int>(time(nullptr)));
	//checkNthElementAlgo();

	const char* inputFilePath = { "Resources/numbers.txt" };
	PerformanceMonitor monitor;

	std::vector<int> originalNumbers;

	if(readNumbersFromFile) {
		originalNumbers.reserve(2000000);
		std::cout << "Reading file..." << std::endl;
		if(readFile(inputFilePath, originalNumbers) != STATUS_SUCCESS) {
			exitWithError();
		}
		std::cout << "Processing " << originalNumbers.size() << " integers" << std::endl;
		// Preprocessing for testing purposes:
		std::cout << "Shuffeling values..." << std::endl;
		std::random_shuffle(originalNumbers.begin(), originalNumbers.end());
	} else {
		originalNumbers = generateRandomNumbers(1000000, -10000, +10000);
	}

	// The buffers are needed later for better test case comparisons
	std::cout << "Preparing buffers..." << std::endl;
	std::vector<int> sortedNumbers;
	std::vector<int> reversedNumbers;
	sortedNumbers = originalNumbers;
	std::sort(sortedNumbers.begin(), sortedNumbers.end());
	reversedNumbers = sortedNumbers;
	std::reverse(reversedNumbers.begin(), reversedNumbers.end());
	
	assert(utils::isSorted(&(originalNumbers[0]), originalNumbers.size()) >= 0);
	assert(utils::isSorted(&(sortedNumbers[0]), sortedNumbers.size()) == -1);
	assert(utils::isReverseSorted(&(reversedNumbers[0]), reversedNumbers.size()) == -1);
	
	// Holder for time measurements:
	double stdSort[3] = {0};
	double quickSort[3] = { 0 };	
	double stdNthElement[3] = { 0 };
	double nthElement[3] = { 0 };

	std::vector<int> numbers;
	
	// stdSort: unsorted array
	std::cout << "Running std::sort on unsorted array..." << std::endl;
	numbers = originalNumbers;
	monitor.start();
	std::sort(numbers.begin(), numbers.end());
	stdSort[0] = monitor.stop();
	checkSortResult(numbers);
	// stdSort: sorted array
	std::cout << "Running std::sort on sorted array..." << std::endl;
	numbers = sortedNumbers;
	monitor.start();
	std::sort(numbers.begin(), numbers.end());
	stdSort[1] = monitor.stop();
	checkSortResult(numbers);
	// stdSort: reverse sorted array
	std::cout << "Running std::sort on reverse sorted array..." << std::endl;
	numbers = reversedNumbers;
	monitor.start();
	std::sort(numbers.begin(), numbers.end());
	stdSort[2] = monitor.stop();
	checkSortResult(numbers);
	

	// quickSort: unsorted array
	std::cout << "Running quickSort on unsorted array..." << std::endl;
	numbers = originalNumbers;
	monitor.start();
	utils::quickSort(&(numbers[0]), numbers.size());
	quickSort[0] = monitor.stop();
	checkSortResult(numbers);
	// quickSort: sorted array
	std::cout << "Running quickSort on sorted array..." << std::endl;
	numbers = sortedNumbers;
	monitor.start();
	utils::quickSort(&(numbers[0]), numbers.size());
	quickSort[1] = monitor.stop();
	checkSortResult(numbers);
	// quickSort: reverse sorted array
	std::cout << "Running quickSort on reverse sorted array..." << std::endl;
	numbers = reversedNumbers;
	monitor.start();
	utils::quickSort(&(numbers[0]), numbers.size());
	quickSort[2] = monitor.stop();
	checkSortResult(numbers);

	const int sampleCount = 100;		// number of nth-element calls
	int n = originalNumbers.size() / 2;		

	/*
	 The target of the homework is to compare median-finding algorithms. 
	 In order to compare the nth-element algorithm to the sort-algorithms, they have to search for the exact same number every time: the median.
	 Note that the search-array is rearranged after each nthElement-call and not reset during the samples. So the last function call doesn't operate on a fully random/reversed array anymore.
	*/



	// TODO: std::nth_element
	// numbers = originalNumbers;
	// ...

	
	// nthElement: unsorted array
	std::cout << "Running nthElement on unsorted array..." << std::endl;
	numbers = originalNumbers;
	for(int i = 0; i < sampleCount; ++i) {
		//int n = utils::randRange(1, numbers.size());
		monitor.start();
		utils::nthElement(&(numbers[0]), numbers.size(), n);
		nthElement[0] += monitor.stop();
	}
	// nthElement: sorted array
	std::cout << "Running nthElement on sorted array..." << std::endl;
	numbers = sortedNumbers;
	for(int i = 0; i < sampleCount; ++i) {
		//int n = utils::randRange(1, numbers.size());
		monitor.start();
		utils::nthElement(&(numbers[0]), numbers.size(), n);
		nthElement[1] += monitor.stop();
	}
	// nthElement: reverse sorted array
	std::reverse(numbers.begin(), numbers.end());
	std::cout << "Running nthElement on reverse sorted array..." << std::endl;
	numbers = reversedNumbers;
	for(int i = 0; i < sampleCount; ++i) {
		//int n = utils::randRange(1, numbers.size());
		monitor.start();
		utils::nthElement(&(numbers[0]), numbers.size(), n);
		nthElement[2] += monitor.stop();
	}


	std::cout << std::endl << numbers.size() << " elements have been used." << std::endl
		<< "             unsorted array       sorted array     reverse sorted array" << std::endl
		<< "-------------------------------------------------------------------------" << std::endl
		<< "  std::sort   " << PerformanceMonitor::millisToString(  stdSort[0] ) << "        " << PerformanceMonitor::millisToString(  stdSort[1] ) << "         " << PerformanceMonitor::millisToString(  stdSort[2] ) << std::endl
		<< "  quicksort   " << PerformanceMonitor::millisToString( quickSort[0]) << "        " << PerformanceMonitor::millisToString( quickSort[1]) << "         " << PerformanceMonitor::millisToString( quickSort[2]) << std::endl
		<< " nth-element  " << PerformanceMonitor::millisToString(nthElement[0]) << "        " << PerformanceMonitor::millisToString(nthElement[1]) << "         " << PerformanceMonitor::millisToString(nthElement[2]) << std::endl;

	std::cout << "Note: nth element has been executed " << sampleCount << " times per measurement." << std::endl;
	int mid = numbers.size() / 2 + 5;
	std::cout << "Median: Middle element has value " << sortedNumbers[mid] << std::endl;

	std::cin.ignore();
}
