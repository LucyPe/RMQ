#include "Fast2_RMQ.hpp"
#include <algorithm>

Fast2_RMQ::Fast2_RMQ(DT* input, DTidx n) : RMQ(input) {
	this->n = n;
}

void Fast2_RMQ::preprocessAlg() {
	block_size = (DTidx)ceil(Util::log2fast(n) / 2);
	block_count = block(n - 1) + 1;

	// allocate and fill arrays for out-of-block-queries;
	block_mins = new DTidx[block_count];
	block_prefix_mins = new DTidx[n];
	block_suffix_mins = new DTidx[n];

	// compute min for blocks
	for (DTidx b = 0; b < block_count; b++) {
		DTidx start = (DTidx)(b * block_size);
		DTidx end = std::min(start + block_size, n - 1);
		block_mins[b] = findMin(start, end);

		// compute block of prefix mins
		block_prefix_mins[start] = start;
		DT min = input[start];
		for (DTidx i = start + 1; i < end; i++) {
			if (input[i] < min) {
				min = input[i];
				block_prefix_mins[i] = i;
			}
			else {
				block_prefix_mins[i] = block_prefix_mins[i - 1];
			}
		}

		// compute block of suffix mins
		block_suffix_mins[end - 1] = end - 1;
		min = input[end - 1];
		for (DTidx j = 2; j <= block_size; j++) {
			DTidx i = end - j;
			if (input[i] < min) {
				min = input[i];
				block_suffix_mins[i] = i;
			}
			else {
				block_suffix_mins[i] = block_suffix_mins[i + 1];
			}
		}
	}
	RMQ_ST = new Big_RMQ(input, block_mins, block_count);
}

DTidx Fast2_RMQ::queryAlg(DTidx i, DTidx j) {
	DTidx block_i = block(i);       // block where i is
	DTidx block_j = block(j);       // j's block

	// in same block
	if (block_j == block_i) {
		return findMin(i, j);
	}

	DTidx i_end = (block_i + 1) * this->block_size;
	DTidx j_start = block_j * this->block_size;
	
	// first block
	DTidx min = block_suffix_mins[i];

	// out-of-block query
	if ((block_i + 1) < block_j) {
		DTidx tmp = block_mins[RMQ_ST->query(block_i + 1, block_j - 1)];
		if (this->input[tmp] < this->input[min]) {
			min = tmp;
		}
	}

	// last block
	if (input[block_prefix_mins[j]] < input[min]) {
		return block_prefix_mins[j];
	}

	return min;
}

void Fast2_RMQ::deleteMem() {
	RMQ_ST->deleteMem();
	delete[] block_mins;
	delete[] block_prefix_mins;
	delete[] block_suffix_mins;
}

Fast2_RMQ::~Fast2_RMQ() {
	if (preprocess_complete) deleteMem();
}