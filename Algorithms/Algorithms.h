#pragma once
#include <vector>

namespace simon {
	
	void std_nthElementSort(std::vector<int>& input, int left, int pivot, int right);

	void DselectAlgo(std::vector<int>& input, int left, int right);

	int findMedianOfMedians(std::vector<int>& input, int left, int right);


}