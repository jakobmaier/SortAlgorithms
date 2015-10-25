#pragma once

namespace utils {
	/**
	 * Returns a randum integer in the range [min, max]
	 */
	int randRange(int min, int max);

	/**
	* Checks if the given Array is correctly sorted.
	* If yes, -1 is returned. Otherwise the first invalid array index is returned.
	*/
	template<typename T>
	int isSorted(const T* numbers, int size) {
		for(int i = 0; i < size; ++i) {
			if(numbers[i] < numbers[i - 1]) {
				return i;
			}
		}
		return -1;
	}

}