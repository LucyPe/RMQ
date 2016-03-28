#include "Fast_RMQ.hpp"
#include <algorithm>

Fast_RMQ::Fast_RMQ(DT* input, DTidx n) : RMQ(input) {
	this->n = n;
}

void Fast_RMQ::preprocessAlg() {
	block_size = (DTidx)ceil(Util::log2fast(n) / 2);
	block_count = block(n - 1) + 1;

	// allocate and fill arrays for out-of-block-queries;
	block_mins = new DTidx[block_count];

	// compute min for blocks
	for (DTidx b = 0; b < block_count; b++) {
		DTidx start = (DTidx)(b * block_size);
		DTidx end = std::min(start + block_size, n);
		block_mins[b] = RMQ::findMin(start, end -1);
	}

	RMQ_ST = new Big_RMQ(input, block_mins, block_count);
}

DTidx Fast_RMQ::queryAlg(DTidx i, DTidx j) {
	DTidx block_i = block(i);       // block where i is
	DTidx block_j = block(j);       // j's block

	// in same block
	if (block_j == block_i) {
		return RMQ::findMin(i, j);
	}

	DTidx i_end = (block_i + 1) * block_size;
	DTidx j_start = block_j * block_size;
	
	// first block
	DTidx min = RMQ::findMin(i, i_end-1);

	// out-of-block query
	if ((block_i + 1) < block_j) {
		DTidx tmp = block_mins[RMQ_ST->query(block_i + 1, block_j - 1)];
		if (input[tmp] < input[min]) {
			min = tmp;
		}
	}

	// last block
	DTidx tmp = RMQ::findMin(j_start, j);
	if (input[tmp] < input[min]) {
		return tmp;
	}

	return min;
}

void Fast_RMQ::deleteMem() {
	RMQ_ST->deleteMem();
	delete[] block_mins;
}

Fast_RMQ::~Fast_RMQ() {
	if (preprocess_complete) deleteMem();
}
