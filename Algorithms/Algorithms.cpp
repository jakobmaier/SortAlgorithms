#include "Algorithms.h"
#include <algorithm>
#include <assert.h>
#include <vector>

namespace simon
{
	
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




	//The Dselect Algo  -> deterministic select algorithm - Median of medians
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

		bound = std::partition(input.begin() + left, input.begin() + right, [median](int val){return val < median; });

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


						
					 //  |   |   |   |    |
					 //  v   v   v   v    v
	// * * * * *     //  0   2   45  77  100   // 0    45   77   2    100           //  *    *   *    2   100
	// * * * * *     //  2   66  51  78  101   // 2    51   78   66   101			//  *    *   *   66   101
	// * * * * *     //  6   90  52  79  102   // 6    52   79   90   102 <<----	// 6    52 |79|  90   102 
	// * * * * *     //  89  91  65  90  200   // 89   65   90   91   200			// 89   65   *    *    *
	// * * * * *     //  200 100 98  99  250   // 200  98   99   100  250			// 200  98   *    *    *
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
			return medianVecs[(medianVecs.size() - 1) / 2];
		}
		else
		{
			return findMedianOfMedians(medianVecs, 0, medianVecs.size());
		}
	}
}
