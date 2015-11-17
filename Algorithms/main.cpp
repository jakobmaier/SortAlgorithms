
#include <iostream>
#include <vector>
#include <algorithm>
#include "PerformanceMonitor.h"
#include "QuickSort.h"
#include "Algorithms.h"

const bool readNumbersFromFile = true;// false;


int main() {
	std::srand(static_cast<unsigned int>(time(nullptr)));
	//checkNthElementAlgo();

	const char* inputFilePath = { "Resources/numbers.txt" };
	PerformanceMonitor monitor;

	std::vector<int> originalNumbers;

	if (readNumbersFromFile) {
		originalNumbers.reserve(2000000);
		std::cout << "Reading file..." << std::endl;
		if (utils::readFile(inputFilePath, originalNumbers) != utils::STATUS_SUCCESS) {
			utils::exitWithError();
		}
		std::cout << "Processing " << originalNumbers.size() << " integers" << std::endl;
		// Preprocessing for testing purposes:
		std::cout << "Shuffeling values..." << std::endl;
		std::random_shuffle(originalNumbers.begin(), originalNumbers.end());
	}
	else {
		originalNumbers = utils::generateRandomNumbers(1000000, -10000, +10000);
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
	double stdSort[3] = { 0 };
	double quickSort[3] = { 0 };
	double stdNthElementSort[3] = { 0 };
	double medianOfMedianRes[3] = { 0 };
	double stdNthElement[3] = { 0 };
	double nthElement[3] = { 0 };

	std::vector<int> numbers;



	// ========================================================================================================================================================================


	// stdSort: unsorted array
	std::cout << "Running std::sort on unsorted array..." << std::endl;
	numbers = originalNumbers;
	monitor.start();
	std::sort(numbers.begin(), numbers.end());
	stdSort[0] = monitor.stop();
	utils::checkSortResult(numbers);
	// stdSort: sorted array
	std::cout << "Running std::sort on sorted array..." << std::endl;
	numbers = sortedNumbers;
	monitor.start();
	std::sort(numbers.begin(), numbers.end());
	stdSort[1] = monitor.stop();
	utils::checkSortResult(numbers);
	// stdSort: reverse sorted array
	std::cout << "Running std::sort on reverse sorted array..." << std::endl;
	numbers = reversedNumbers;
	monitor.start();
	std::sort(numbers.begin(), numbers.end());
	stdSort[2] = monitor.stop();
	utils::checkSortResult(numbers);


	// ========================================================================================================================================================================


	// quickSort: unsorted array
	std::cout << "Running quickSort on unsorted array..." << std::endl;
	numbers = originalNumbers;
	monitor.start();
	utils::quickSort(&(numbers[0]), numbers.size());
	quickSort[0] = monitor.stop();
	utils::checkSortResult(numbers);
	// quickSort: sorted array
	std::cout << "Running quickSort on sorted array..." << std::endl;
	numbers = sortedNumbers;
	monitor.start();
	utils::quickSort(&(numbers[0]), numbers.size());
	quickSort[1] = monitor.stop();
	utils::checkSortResult(numbers);
	// quickSort: reverse sorted array
	std::cout << "Running quickSort on reverse sorted array..." << std::endl;
	numbers = reversedNumbers;
	monitor.start();
	utils::quickSort(&(numbers[0]), numbers.size());
	quickSort[2] = monitor.stop();
	utils::checkSortResult(numbers);


	// ========================================================================================================================================================================


	// sort with std::nth_element
	// std::nth_element: unsorted array
	std::cout << "Running std::nth_element sort on unsorted array..." << std::endl;
	numbers = originalNumbers;
	monitor.start();
	simon::std_nthElementSort(numbers, 0, numbers.size() / 2, numbers.size());
	stdNthElementSort[0] = monitor.stop();
	utils::checkSortResult(numbers);
	// std::nth_element: sorted array
	std::cout << "Running std::nth_element sort on sorted array..." << std::endl;
	numbers = sortedNumbers;
	monitor.start();
	simon::std_nthElementSort(numbers, 0, numbers.size() / 2, numbers.size());
	stdNthElementSort[1] = monitor.stop();
	utils::checkSortResult(numbers);
	// std::nth_element: reverse sorted array
	std::cout << "Running std::nth_element sort on reverse sorted array..." << std::endl;
	numbers = reversedNumbers;
	monitor.start();
	simon::std_nthElementSort(numbers, 0, numbers.size() / 2, numbers.size());
	stdNthElementSort[2] = monitor.stop();
	utils::checkSortResult(numbers);


	// ========================================================================================================================================================================


	// median of medians: unsorted array
	std::cout << "Running median of medians on unsorted array..." << std::endl;
	numbers = originalNumbers;
	monitor.start();
	int median = simon::findMedianOfMedians(numbers, 0, numbers.size());
	std::cout << "Median: " << median << std::endl;
	medianOfMedianRes[0] = monitor.stop();
	// median of medians: sorted array
	std::cout << "Running median of medians on sorted array..." << std::endl;
	numbers = sortedNumbers;
	monitor.start();
	median = simon::findMedianOfMedians(numbers, 0, numbers.size());
	std::cout << "Median: " << median << std::endl;
	medianOfMedianRes[1] = monitor.stop();
	// median of medians: reverse sorted array
	std::cout << "Running median of medians on reverse sorted array..." << std::endl;
	numbers = reversedNumbers;
	monitor.start();
	median = simon::findMedianOfMedians(numbers, 0, numbers.size());
	std::cout << "Median: " << median << std::endl;
	medianOfMedianRes[2] = monitor.stop();


	// ========================================================================================================================================================================


	const int sampleCount = 100;		// number of nth-element calls
	int n = originalNumbers.size() / 2;


	// std::nth_element: unsorted array
	std::cout << "Running std::nth_element on unsorted array..." << std::endl;
	numbers = originalNumbers;
	for (int i = 0; i < sampleCount; ++i) {
		//int n = utils::randRange(1, numbers.size());
		monitor.start();
		std::nth_element(numbers.begin(), numbers.begin() + n, numbers.end());
		stdNthElement[0] += monitor.stop();
		median = numbers[n];
	}
	std::cout << "Median: " << median << std::endl;

	// std::nth_element: sorted array
	std::cout << "Running std::nth_element on sorted array..." << std::endl;
	numbers = sortedNumbers;
	for (int i = 0; i < sampleCount; ++i) {
		//int n = utils::randRange(1, numbers.size());
		monitor.start();
		std::nth_element(numbers.begin(), numbers.begin() + n, numbers.end());
		stdNthElement[1] += monitor.stop();
		median = numbers[n];
	}
	std::cout << "Median: " << median << std::endl;
	// std::nth_element: reverse sorted array
	std::cout << "Running std::nth_element on reverse sorted array..." << std::endl;
	numbers = reversedNumbers;
	for (int i = 0; i < sampleCount; ++i) {
		//int n = utils::randRange(1, numbers.size());
		monitor.start();
		std::nth_element(numbers.begin(), numbers.begin() + n, numbers.end());
		stdNthElement[2] += monitor.stop();
		median = numbers[n];
	}
	std::cout << "Median: " << median << std::endl;


	// ========================================================================================================================================================================


	// nthElement: unsorted array
	std::cout << "Running nthElement on unsorted array..." << std::endl;
	numbers = originalNumbers;
	for (int i = 0; i < sampleCount; ++i) {
		//int n = utils::randRange(1, numbers.size());
		monitor.start();
		median = utils::nthElement(&(numbers[0]), numbers.size(), n);
		nthElement[0] += monitor.stop();
	}
	std::cout << "Median: " << median << std::endl;
	// nthElement: sorted array
	std::cout << "Running nthElement on sorted array..." << std::endl;
	numbers = sortedNumbers;
	for (int i = 0; i < sampleCount; ++i) {
		//int n = utils::randRange(1, numbers.size());
		monitor.start();
		median = utils::nthElement(&(numbers[0]), numbers.size(), n);
		nthElement[1] += monitor.stop();
	}
	std::cout << "Median: " << median << std::endl;
	// nthElement: reverse sorted array
	std::reverse(numbers.begin(), numbers.end());
	std::cout << "Running nthElement on reverse sorted array..." << std::endl;
	numbers = reversedNumbers;
	for (int i = 0; i < sampleCount; ++i) {
		//int n = utils::randRange(1, numbers.size());
		monitor.start();
		median = utils::nthElement(&(numbers[0]), numbers.size(), n);
		nthElement[2] += monitor.stop();
	}
	std::cout << "Median: " << median << std::endl;



	// ========================================================================================================================================================================



	std::cout << std::endl << numbers.size() << " elements have been used." << std::endl
		<< "                     unsorted array       sorted array     reverse sorted array" << std::endl
		<< "--------------------------------------------------------------------------------" << std::endl
		<< "      std::sort       " << PerformanceMonitor::millisToString(stdSort[0]) << "        " << PerformanceMonitor::millisToString(stdSort[1]) << "         " << PerformanceMonitor::millisToString(stdSort[2]) << std::endl
		<< "      quicksort       " << PerformanceMonitor::millisToString(quickSort[0]) << "        " << PerformanceMonitor::millisToString(quickSort[1]) << "         " << PerformanceMonitor::millisToString(quickSort[2]) << std::endl
		<< " std::nthElementSort  " << PerformanceMonitor::millisToString(stdNthElementSort[0]) << "        " << PerformanceMonitor::millisToString(stdNthElementSort[1]) << "         " << PerformanceMonitor::millisToString(stdNthElementSort[2]) << std::endl
		<< "  median of medians   " << PerformanceMonitor::millisToString(medianOfMedianRes[0]) << "        " << PerformanceMonitor::millisToString(medianOfMedianRes[1]) << "         " << PerformanceMonitor::millisToString(medianOfMedianRes[2]) << std::endl
		<< "   std::nthElement    " << PerformanceMonitor::millisToString(stdNthElement[0]) << "        " << PerformanceMonitor::millisToString(stdNthElement[1]) << "         " << PerformanceMonitor::millisToString(stdNthElement[2]) << std::endl
		<< "     nth-element      " << PerformanceMonitor::millisToString(nthElement[0]) << "        " << PerformanceMonitor::millisToString(nthElement[1]) << "         " << PerformanceMonitor::millisToString(nthElement[2]) << std::endl;
	std::cout << "Note: nth element has been executed " << sampleCount << " times per measurement." << std::endl;
	int mid = numbers.size() / 2;
	std::cout << "Median: Middle element has value " << sortedNumbers[mid] << std::endl;

	std::cin.ignore();
}
