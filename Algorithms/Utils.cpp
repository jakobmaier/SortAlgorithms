#include "Utils.h"
#include <random>
#include <assert.h>
#include <immintrin.h>
#include <iostream>


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

}