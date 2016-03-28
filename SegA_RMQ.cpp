#include "SegA_RMQ.hpp"
#include <algorithm>

SegA_RMQ::SegA_RMQ(DT* input, DTidx n) : RMQ(input) {
	this->n = n;
}

void SegA_RMQ::preprocessAlg() {
	/* segment tree is made above the input array*/

	// height of segment tree
	DTidx height = Util::log2fast(n - 1) + 1; // nearest power of 2
	DTidx leafs = 1 << height; // 2^height

	// allocate segment array 
	seg_array_size = leafs - 1 ; // (2^height) -1 = #all_nodes - #leafs
	seg_array = new DTidx[seg_array_size];
	
	DTidx level_size = leafs / 2;
	DTidx offset = seg_array_size / 2; // from beginning of seg_array

	// fill the lowest level of segment tree from input array
	for (DTidx i = 0; i < level_size; i++) {
		DTidx left = 2 * i;
		DTidx right = left + 1;
		if (right < n) {
			seg_array[offset + i] = input[left] <= input[right] ? left : right;
		}
	}

	DTidx multiplier = 2;

	// fill higher levels of segment tree
	for (DTidx i = 0; i < height - 1; i++) {
		level_size /= 2;
		offset -= level_size;
		multiplier *= 2;
		// for each level of tree
		for (DTidx j = 0; j < level_size; j++) {
			// if we are in existing part of tree
			if (((multiplier * (j + 1)) - 1) < n) {
				// compute children
				DTidx segment = offset + level_size + (2 * j); 	
				DTidx left = seg_array[segment];
				DTidx right = seg_array[segment + 1];
				// fill array
				seg_array[offset + j] = input[left] <= input[right] ? left : right;
			}
		}
	}
}

DTidx SegA_RMQ::queryAlg(DTidx i, DTidx j) {
	return this->arrayQuery(i, j, 0, seg_array_size - 1, 0);
}

DTidx SegA_RMQ::arrayQuery(DTidx i, DTidx j, DTidx a, DTidx b, DTidx level) {
	// <a, b> is a subset of <i, j>
	if ((i <= a) && (b <= j)) {
		return this->seg_array[level];
	}

	DTidx m = (a + b) / 2;
	DTidx left = (2 * level) + 1;
	DTidx right = left + 1;	

	// <a, k> overlaps with <i, j>, recurse on left child
	bool goLeft = (a <= j) && (i <= m);
	DTidx left_min = (goLeft && (left < seg_array_size)) ? arrayQuery(i, j, a, m, left) : m;

	// <k + 1, b> overlaps with <i, j>, recurse on right child
	bool goRight = ((m + 1) <= j) && (i <= b);
	DTidx right_min = (goRight && (right < seg_array_size)) ? arrayQuery(i, j, m + 1, b, right) : m + 1;

	if (goLeft && goRight) {
		return (input[left_min] <= input[right_min]) ? left_min : right_min;
	}
	return goLeft ? left_min : right_min;
}

void SegA_RMQ::deleteMem() {
	delete[] seg_array;
}

SegA_RMQ::~SegA_RMQ() {
	if (preprocess_complete) deleteMem();
}
