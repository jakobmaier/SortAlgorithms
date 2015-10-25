#include "Utils.h"
#include <random>
#include <assert.h>

namespace utils {
	int randRange(int min, int max) {
		assert(max >= min);
		double range = max - min + 1;
		int rnd = min + static_cast<int>(range*rand() / (RAND_MAX + 1.0));
		assert(rnd >= min && rnd <= max);
		return rnd;
	}
}