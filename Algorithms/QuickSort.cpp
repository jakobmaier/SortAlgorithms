#include <stack>
#include "Utils.h"
#include <assert.h>


namespace utils {


	namespace {		// Anonymous namespace to ensure that the next template functions are not exported
		enum PivotStrategy {
			RANDOMIZED_PIVOT,
			PIVOT_LEFT,
			PIVOT_RIGHT,
			PIVOT_MIDDLE
		};
		const PivotStrategy pivotStrategy = RANDOMIZED_PIVOT;


		/**
		* Selects a randomized pivot element.
		* Ensures that all numbers on the left are smaller than the pivot and the numbers on the right are bigger.
		* Returns the position of the selected pivot.
		* Ensures that the number of swap operations is minimal, but has a higher number of comparisons.
		*
		*	Algorithm:
		*    Two pointers coming from left and right towards the middle, until they meet.
		*    1) The right pointer is decreased until the value is smaller than the pivot.
		*			Then, the left pointer is overwritten by the right's value.
		*			The original value is stored in the pivot variable. The value now appears twice in the Array.
		*    2) The left pointer is increased until the value is bigger than the pivot.
		*          The right pointer is overwritten by the left's value. Now, the old value doesn't appear twice anymore, instead the new value does
		*    This is repeated until left and right mets. At this time, the last value that has been put somewhere else appears twice.
		*    This value is then replaced by the pivot's value.
		*
		*/
		template<typename T>
		int partition(T* input, int leftIdx, int rightIdx) {
			switch(pivotStrategy) {
				case PIVOT_LEFT: break;
				case RANDOMIZED_PIVOT: {
						int pivotIdx = randRange(leftIdx, rightIdx);
						std::swap(input[leftIdx], input[pivotIdx]);
					} break;
				case PIVOT_RIGHT:
					std::swap(input[leftIdx], input[rightIdx]);
					break;
				case PIVOT_MIDDLE: {
						int pivotIdx = leftIdx + ((rightIdx - leftIdx) / 2);
						std::swap(input[leftIdx], input[pivotIdx]);
					} break;
			}			

			int l = leftIdx;
			int r = rightIdx;
			int pivot = input[l];

			while(l < r) {
				while(input[r] >= pivot && l < r) {
					--r;
				}
				if(l < r) {
					input[l++] = input[r];
				}

				while(input[l] <= pivot && l < r) {
					++l;
				}
				if(l < r) {
					input[r--] = input[l];
				}
			}
			input[l] = pivot;
			return l;
		}


		template<typename T>
		void quickSort(T* input, int leftIdx, int rightIdx) {
			int split;
			if(leftIdx < rightIdx) {
				split = partition(input, leftIdx, rightIdx);
				quickSort(input, leftIdx, split - 1);
				quickSort(input, split + 1, rightIdx);
			}
		}

		template<typename T>
		int nthElement(T* input, int leftIdx, int rightIdx, int n) {
			int split;
			int leftElementCount;

			if(leftIdx >= rightIdx) {
				return input[leftIdx];
			}

			split = partition(input, leftIdx, rightIdx);
			leftElementCount = split - leftIdx + 1;		// number of elements on the left side, incl. split element
			if(leftElementCount == n) {
				return input[split-1];
			}

			if(n < leftElementCount) {					// search on left side
				return nthElement(input, leftIdx, split - 1, n);
			}
			return nthElement(input, split + 1, rightIdx, n - leftElementCount);	// search on right side
		}
	}


	/**
	 * Uses quicksort to recursively sort the Array
	 */
	template<typename T>
	void quickSort(T* input, int size) {
		assert(size >= 0);
		quickSort(input, 0, size - 1);
	}

	/**
	* Uses quicksort to sort the Array non recusrivly
	* This method is much more inefficient than the recusrive solution.
	*/
	template<typename T>
	void quickSortNonRecursive(T* input, int size) {
		assert(size >= 0);

		std::stack<std::pair<int, int>> borders;
		borders.push(std::pair<int, int>(0, size - 1));

		std::pair<int, int> b;
		int split;
		while(!borders.empty()) {
			b = borders.top();
			borders.pop();
			if(b.first >= b.second) {
				continue;
			}
			split = partition(input, b.first, b.second);
			borders.push(std::pair<int, int>(b.first, split - 1));
			borders.push(std::pair<int, int>(split + 1, b.second));
		}
	}

	/**
	 * Returns the index of the nth element. n is 1-based.
	 * Parts of the input data will be reordered.
	 * If the input size is zero, -1 will be returned.
	 */
	template<typename T>
	int nthElement(T* input, int size, int n) {
		assert(size >= 0);
		assert(n > 0);
		assert(n <= size);
		if(size == 0) {
			return -1;
		}
		return nthElement(input, 0, size - 1, n);
	}
}


