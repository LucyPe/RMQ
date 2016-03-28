#include "FH_RMQ.hpp"

FH_RMQ::FH_RMQ(DT* input, DTidx n) : RMQ(input) {
	this->n = n;
}

void FH_RMQ::preprocessAlg() {
	this->n = n;
	block_size = (DTidx)ceil(log2(n) / 4); // calculate block size
	block_count = block(n - 1) + 1;              // number of blocks
	qpb = block_size * (block_size + 1) / 2;          // # of different queries per block

	// allocate and fill arrays for out-of-block-queries;
	// combined with type-calculation for the blocks:
	index = new DTidx[block_count];
	type = new DTidx[block_count];
	Prec = new DTidx*[Util::Catalan[block_size][block_size]];
	for (DTidx i = 0; i < Util::Catalan[block_size][block_size]; i++) {
		Prec[i] = new DTidx[qpb];
		Prec[i][0] = block_size; // init with impossible value
	}
	DT* rp = new DT[block_size + 1];  // rp: rightmost path in Cart. tree

	DTidx min;   // position of minimum in current block
	DTidx z = 0;   // index in array a
	DTidx end;   // end of current block
	DTidx start; // start of current block
	DTidx q;     // position in Catalan triangle
	DTidx p;     // --------- " ----------------
	rp[0] = Util::minus_infinity; // stopper (minus infinity)

	for (DTidx i = 0; i < block_count; i++) { // step through blocks
		start = z;            // init start
		min = start;          // init minimum
		end = start + block_size;     // end of block
		if (end > n) end = n; // last block could be smaller than bs!
		q = block_size;       // init q
		p = block_size - 1;     // init p
		type[i] = 0;  // init type (will be increased!)
		rp[1] = input[z]; // init rightmost path

		while (++z < end) {   // step through current block:
			if (input[z] < input[min]) min = z;   // update minimum
			p--;
			while (rp[q - p - 1] > input[z]) {
				type[i] += Util::Catalan[p][q]; // update type
				q--;
			}
			rp[q - p] = input[z]; // add last element to rightmost path
		}
		index[i] = min; // store index of minimum

		// precompute in-block-queries for this block (if necessary):
		if (Prec[type[i]][0] == block_size) {
			p = 0; // now index in Prec[type[i]]
			for (DTidx j = start; j < end; j++) {
				min = j;
				for (DTidx k = j; k < end; k++) {
					if (input[k] < input[min]) min = k;
					Prec[type[i]][p++] = min - start;
				}
			}
		}
	}
	delete[] rp;

	RMQ_ST = new Big_RMQ(input, index, block_count); // construct sparse table on block-minima
}

DTidx FH_RMQ::queryAlg(DTidx i, DTidx j) {
	DTidx block_i = block(i);       // block where i is
	DTidx block_j = block(j);       // j's block
	DTidx i_pos = i - (block_i*block_size); // position of i in block_i
	DTidx j_pos = j - (block_j*block_size); // pos. of j in block_j
	DTidx min, min_i, min_j; 		// min: to be returned

	if (block_j == block_i)
		min = (block_i*block_size) + Prec[type[block_i]][i_pos*(block_size - 1) + j_pos - ((i_pos - 1)*i_pos / 2)];
	else {
		min_i = (block_i*block_size) + Prec[type[block_i]][(i_pos + 1)*(block_size - 1) - ((i_pos - 1)*i_pos / 2)];
		min_j = (block_j*block_size) + Prec[type[block_j]][j_pos];
		min = input[min_i] < input[min_j] ? min_i : min_j;
		if (block_j > block_i + 1) { // out-of-block query:
			min_i = index[RMQ_ST->query(block_i+1, block_j-1)];
			if (input[min_i] < input[min]) min = min_i;
		}
	}

	return min;
}

void FH_RMQ::deleteMem() {
	RMQ_ST->deleteMem();
	delete[] index; 
	delete[] type;	
	for (DTidx i = 0; i < Util::Catalan[block_size][block_size]; i++)
		delete[] Prec[i];
	delete[] Prec;
}

FH_RMQ::~FH_RMQ() {
	if (preprocess_complete) deleteMem();
}