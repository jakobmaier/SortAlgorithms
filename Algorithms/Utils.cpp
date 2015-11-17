#include "Utils.h"
#include <random>
#include <assert.h>
#include <immintrin.h>
#include <iostream>
#include <fstream>


namespace utils {

	int randRange(int min, int max) {
		assert(max >= min);
		double range = max - min + 1;
		int rnd = min + static_cast<int>(range*rand() / (RAND_MAX + 1.0)); // TODO this plus one has to be moved outside these brackets http://stackoverflow.com/questions/9878965/c-rand-between-0-and-1
		assert(rnd >= min && rnd <= max);
		return rnd;
	}


	// http://www.eternallyconfuzzled.com/arts/jsw_art_rand.aspx
	size_t randRangeRD(int min, int max)
	{
		assert(max > min && "Max isn't greater than min");
		size_t rand = 0;
		int result = _rdrand32_step(&rand);
		assert(result == 1 && "If result != 1 the hardware couldn't produce random values");
		rand %= (max - min +1) + min;
		//std::cout << "rdRand: " << rand << std::endl;
		assert(rand >= min && rand <= max && "result is smaller or greater than expected");
		return rand;
	}

	void exitWithError() {
		std::cerr << "Execution failed. Exiting..." << std::endl;
		std::cin.ignore();
		exit(1);
	}

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
}