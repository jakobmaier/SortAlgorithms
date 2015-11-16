
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "PerformanceMonitor.h"
#include "QuickSort.h"

const bool readNumbersFromFile = false;

using namespace std;

enum Status {
	STATUS_SUCCESS,
	STATUS_ERROR
};

// Reads a file of integers
Status readFile(const char* path, std::vector<int>& numbers) {
	std::ifstream inputStream;
	inputStream.open(path);
	if (!inputStream.is_open()) {
		std::cerr << "Input file could not be opened: '" << path << "'" << std::endl;
		return STATUS_ERROR;
	}

	int number;
	while (inputStream >> number) {
		numbers.push_back(number);
	}

	inputStream.close();
	return STATUS_SUCCESS;
}


void exitWithError() {
	std::cerr << "Execution failed. Exiting..." << std::endl;
	std::cin.ignore();
	exit(1);
}

// Checks if a given vector is actually sorted
void checkSortResult(const std::vector<int>& numbers) {
	std::cout << "Checking result..." << std::endl;
	int invalid = utils::isSorted(&(numbers[0]), numbers.size());
	if (invalid == -1) {
		std::cerr << "Sort succeeded" << std::endl;
	}
	else {
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
	for (int i = 0; i < count; ++i) {
		numbers.push_back(utils::randRange(min, max));
	}
	return numbers;
}





// nth element

// This algorithm uses the std::nth_element function to rearrange the elements around the pivot
// the partitions are always broken in half and passed recursevly down the line until the range is smaller than 3 
// this algorithm is probably finished earlyer but we cannot control how much this algorith sorts itself. So we have to use what it guarantees
// and that is that the pivot element has the right value and no element left of the pivot is greater and no element to the right is greater.
void std_nthElementSort(std::vector<int>& input, int left, int pivot, int right)
{
	assert(pivot >= left && pivot <= right);
	//std::cout << left << "|" << pivot << "|" << right << std::endl;
	std::nth_element(input.begin() + left, input.begin() + pivot, input.begin() + right);	// do the "sort"

	int newRange = pivot - left;
	if (newRange <= 3){ return; } // stop condition

	int pivotOffset = newRange / 2;

	// call left side
	std_nthElementSort(input, left, left + pivotOffset, pivot);

	// call right side
	std_nthElementSort(input, pivot + 1, pivot + pivotOffset, right);
}


// * * * * *
// * * * * *
// * * * * *
// * * * * *
// * * * * *
int findMedianOfMedians(std::vector<int>& input, int left, int right)
{
	std::vector<int> medianVecs;
	for (int rangeCounter = left; rangeCounter < right;)
	{
		std::vector<int> fiveElem;
		for (int i = 0; i < 5 && rangeCounter < right; ++i) /// probably switch to sort directly in the inputvector this could help to reduce memory allocs.. 
		{
			//std::cout << input[left + rangeCounter] << " ";
			fiveElem.push_back(input[rangeCounter]);
			++rangeCounter;
		}
		std::sort(fiveElem.begin(), fiveElem.end());
		//std::cout << std::endl << "m: " << fiveElem[static_cast<int> (fiveElem.size() / 2)] << std::endl;
		medianVecs.push_back(fiveElem[static_cast<int> (fiveElem.size() / 2)]);
	}

	//std::cout << "---------------------------" << std::endl;
	if (medianVecs.size() <= 5)
	{
		std::sort(medianVecs.begin(), medianVecs.end());
		return medianVecs[(medianVecs.size()-1) / 2];
	}
	else
	{
		return findMedianOfMedians(medianVecs, 0, medianVecs.size());
	}
}


//The Dselect Algo
// break A into groups of 5 sort each group with what ever wanted
// C = the n/5 "middle elements"
// p = Select (C, n/5 , n/10) recursevly computes median of C
// Partition A around p
// if j = i return p
// if j < i return select ( first part of A, j-1 , i)
// else if j  > i return select ( second part of A , n-j , i-j)
void DselectAlgo(std::vector<int>& input, int left, int right)
{
	
	//std::cout << "----------------------------" << std::endl;
	//std::cout << "LR: " << left << "|" << right << std::endl << std::endl;
	std::vector<int>::iterator bound;

	int median = findMedianOfMedians(input, left, right);

	//int indexPivot = partition_around_pivot_a(input, left, right, median);

	bound =  std::partition(input.begin() + left, input.begin() + right, [median](int val){return val < median; });

	//for (int i = 0; i < input.size(); ++i)
	//{
	//	std::cout << input[i] << ", ";
	//}
	//std::cout << std::endl;

	//int boundIdx = left + indexPivot;
	int boundIdx = bound - input.begin();

	//std::cout << "m: " << median;
	//std::cout << " idx " << boundIdx << std::endl;

	// check if work is done
	if ((boundIdx - left) > 1) // left side
	{
		//std::cout << "left " << std::endl;
		//std::cout << "----------------------------" << std::endl << std::endl;
		DselectAlgo(input, left, boundIdx);
	}
	if ((right - boundIdx) > 1) // right side
	{
		//std::cout << "right " << std::endl;
		//std::cout << "----------------------------" << std::endl << std::endl;
		DselectAlgo(input, boundIdx, right);
	}

}




int main() {

	//std::vector<int> testVec;
	//for (int i = 0; i < 100; ++i){
	//	testVec.push_back(i);
	//}
	//std::random_shuffle(testVec.begin(), testVec.end());


	//	std::nth_element(testVec.begin(), testVec.begin() + testVec.size() / 2, testVec.end());
	//int media = testVec[testVec.size() / 2];
	//std::cout << "Median: " << media << std::endl;

	srand(static_cast<unsigned int>(time(nullptr)));
	//checkNthElementAlgo();

	const char* inputFilePath = { "Resources/numbers.txt" };
	PerformanceMonitor monitor;

	std::vector<int> originalNumbers;

	if (readNumbersFromFile) {
		originalNumbers.reserve(2000000);
		std::cout << "Reading file..." << std::endl;
		if (readFile(inputFilePath, originalNumbers) != STATUS_SUCCESS) {
			exitWithError();
		}
		std::cout << "Processing " << originalNumbers.size() << " integers" << std::endl;
		// Preprocessing for testing purposes:
		std::cout << "Shuffeling values..." << std::endl;
		std::random_shuffle(originalNumbers.begin(), originalNumbers.end());
	}
	else {
		originalNumbers = generateRandomNumbers(100000, -10000, +10000);
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
	double medianOfMedianRes[3] = { 0 };
	double stdNthElement[3] = { 0 };
	double stdNthElementSort[3] = { 0 };
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





	// median of medians: unsorted array
	std::cout << "Running median of medians on unsorted array..." << std::endl;
	numbers = originalNumbers;
	monitor.start();
	int median = findMedianOfMedians(numbers, 0, numbers.size());
	std::cout << "Median: " << median << std::endl;
	medianOfMedianRes[0] = monitor.stop();
	// median of medians: sorted array
	std::cout << "Running median of medians on sorted array..." << std::endl;
	numbers = sortedNumbers;
	monitor.start();
	median = findMedianOfMedians(numbers, 0, numbers.size());
	std::cout << "Median: " << median << std::endl;
	medianOfMedianRes[1] = monitor.stop();
	// median of medians: reverse sorted array
	std::cout << "Running median of medians on reverse sorted array..." << std::endl;
	numbers = reversedNumbers;
	monitor.start();
	median = findMedianOfMedians(numbers, 0, numbers.size());
	std::cout << "Median: " << median << std::endl;
	medianOfMedianRes[2] = monitor.stop();



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


	// sort with std::nth_element
	// std::nth_element: unsorted array
	std::cout << "Running std::nth_element sort on unsorted array..." << std::endl;
	numbers = originalNumbers;
	monitor.start();
	std_nthElementSort(numbers, 0, numbers.size() / 2, numbers.size());
	stdNthElementSort[0] = monitor.stop();
	checkSortResult(numbers);
	// std::nth_element: sorted array
	std::cout << "Running std::nth_element sort on sorted array..." << std::endl;
	numbers = sortedNumbers;
	monitor.start();
	std_nthElementSort(numbers, 0, numbers.size() / 2, numbers.size());
	stdNthElementSort[1] = monitor.stop();
	checkSortResult(numbers);
	// std::nth_element: reverse sorted array
	std::cout << "Running std::nth_element sort on reverse sorted array..." << std::endl;
	numbers = reversedNumbers;
	monitor.start();
	std_nthElementSort(numbers, 0, numbers.size() / 2, numbers.size());
	stdNthElementSort[2] = monitor.stop();
	checkSortResult(numbers);



	// std::nth_element: unsorted array
	std::cout << "Running std::nth_element on unsorted array..." << std::endl;
	numbers = originalNumbers;
	monitor.start();
	std::nth_element(numbers.begin(), numbers.begin() + numbers.size() / 2, numbers.end());
	median = numbers[numbers.size() / 2];
	std::cout << "Median: " << median << std::endl;
	stdNthElement[0] = monitor.stop();
	// std::nth_element: sorted array
	std::cout << "Running std::nth_element on sorted array..." << std::endl;
	numbers = sortedNumbers;
	monitor.start();
	std::nth_element(numbers.begin(), numbers.begin() + numbers.size() / 2, numbers.end());
	median = numbers[numbers.size() / 2];
	std::cout << "Median: " << median << std::endl;
	stdNthElement[1] = monitor.stop();
	// std::nth_element: reverse sorted array
	std::cout << "Running std::nth_element on reverse sorted array..." << std::endl;
	numbers = reversedNumbers;
	monitor.start();
	std::nth_element(numbers.begin(), numbers.begin() + numbers.size() / 2, numbers.end());
	median = numbers[numbers.size() / 2];
	std::cout << "Median: " << median << std::endl;
	stdNthElement[2] = monitor.stop();


	const int sampleCount = 100;		// number of nth-element calls
	int n = originalNumbers.size() / 2;

	///*
	// The target of the homework is to compare median-finding algorithms.
	// In order to compare the nth-element algorithm to the sort-algorithms, they have to search for the exact same number every time: the median.
	// Note that the search-array is rearranged after each nthElement-call and not reset during the samples. So the last function call doesn't operate on a fully random/reversed array anymore.
	// */


	// nthElement: unsorted array
	std::cout << "Running nthElement on unsorted array..." << std::endl;
	numbers = originalNumbers;
	for (int i = 0; i < sampleCount; ++i) {
		//int n = utils::randRange(1, numbers.size());
		monitor.start();
		median = utils::nthElement(&(numbers[0]), numbers.size(), n);
		nthElement[0] += monitor.stop();
	}
	std::cout << "Median: " << numbers[median] << std::endl;
	// nthElement: sorted array
	std::cout << "Running nthElement on sorted array..." << std::endl;
	numbers = sortedNumbers;
	for (int i = 0; i < sampleCount; ++i) {
		//int n = utils::randRange(1, numbers.size());
		monitor.start();
		median = utils::nthElement(&(numbers[0]), numbers.size(), n);
		nthElement[1] += monitor.stop();
	}
	std::cout << "Median: " << numbers[median] << std::endl;
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
	std::cout << "Median: " << numbers[median] << std::endl;


	std::cout << std::endl << numbers.size() << " elements have been used." << std::endl
		<< "             unsorted array       sorted array     reverse sorted array" << std::endl
		<< "-------------------------------------------------------------------------" << std::endl
		<< "  std::sort   " << PerformanceMonitor::millisToString(stdSort[0]) << "        " << PerformanceMonitor::millisToString(stdSort[1]) << "         " << PerformanceMonitor::millisToString(stdSort[2]) << std::endl
		<< "  quicksort   " << PerformanceMonitor::millisToString(quickSort[0]) << "        " << PerformanceMonitor::millisToString(quickSort[1]) << "         " << PerformanceMonitor::millisToString(quickSort[2]) << std::endl
		<< "median of medians " << PerformanceMonitor::millisToString(medianOfMedianRes[0]) << "        " << PerformanceMonitor::millisToString(medianOfMedianRes[1]) << "         " << PerformanceMonitor::millisToString(medianOfMedianRes[2]) << std::endl
		<< " std::nthElem " << PerformanceMonitor::millisToString(stdNthElement[0]) << "        " << PerformanceMonitor::millisToString(stdNthElement[1]) << "         " << PerformanceMonitor::millisToString(stdNthElement[2]) << std::endl
		<< " nth-element  " << PerformanceMonitor::millisToString(nthElement[0]) << "        " << PerformanceMonitor::millisToString(nthElement[1]) << "         " << PerformanceMonitor::millisToString(nthElement[2]) << std::endl;

	//std::cout << "Note: nth element has been executed " << sampleCount << " times per measurement." << std::endl;
	int mid = numbers.size() / 2;
	std::cout << "Median: Middle element has value " << sortedNumbers[mid] << std::endl;

	std::cin.ignore();
}
