
namespace utils {
	namespace {		// Anonymous namespace to ensure that the next template functions are not exported

		/**
		* Selects an arbitrary pivot element.
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
		int partition(T* input, size_t leftIdx, size_t rightIdx) {
			size_t l = leftIdx;
			size_t r = rightIdx;
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
		void quickSort(T* input, size_t leftIdx, size_t rightIdx) {
			int r;
			if(leftIdx < rightIdx) {
				r = partition(input, leftIdx, rightIdx);
				quickSort(input, leftIdx, r);
				quickSort(input, r + 1, rightIdx);
			}
		}
	}


	/**
	 * Uses quicksort to sort the Array
	 */
	template<typename T>
	void quickSort(T* input, size_t size) {
		quickSort(input, 0, size - 1);
	}
}


