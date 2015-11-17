#pragma once
#include <iostream>
#include <vector>

namespace utils {
	enum Status {
		STATUS_SUCCESS,
		STATUS_ERROR
	};

	/**
	 * Returns a random integer in the range [min, max]
	 */
	int randRange(int min, int max);


	/**
	* Returns a random integer in the range [min, max]
	*/
	size_t randRangeRD(int min, int max);


	/**
	* Checks if the given Array is correctly sorted.
	* If yes, -1 is returned. Otherwise the first invalid array index is returned.
	*/
	template<typename T>
	int isSorted(const T* numbers, int size) {
		for (int i = 1; i < size; ++i) {
			if (numbers[i] < numbers[i - 1]) {
				return i;
			}
		}
		return -1;
	}

	/**
	* Checks if the given Array is correctly reversed sorted.
	* If yes, -1 is returned. Otherwise the first invalid array index is returned.
	*/
	template<typename T>
	int isReverseSorted(const T* numbers, int size) {
		for (int i = 1; i < size; ++i) {
			if (numbers[i] > numbers[i - 1]) {
				return i;
			}
		}
		return -1;
	}

	void exitWithError();

	std::vector<int> generateRandomNumbers(int count, int min, int max);

	Status readFile(const char* path, std::vector<int>& numbers);

	void checkSortResult(const std::vector<int>& numbers);

}