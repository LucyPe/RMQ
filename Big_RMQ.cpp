#include "Big_RMQ.hpp"

Big_RMQ::Big_RMQ(DT* input, DTidx n) : RMQ(input) {
	this->index_size = n;
	this->index = new DTidx[n];
	for (DTidx i = 0; i < n; i++) {
		this->index[i] = i;
	}
}

Big_RMQ::Big_RMQ(DT* input, DTidx* index, DTidx index_size) : RMQ(input) {
	this->index = index;
	this->index_size = index_size;
	this->preprocessAlg();
}

void Big_RMQ::preprocessAlg() {
	depth = Util::log2fast(index_size); // table depth

	// allocate space for table:
	M = new DTidx*[depth];
	for (DTidx i = 0; i < depth; i++)
		M[i] = new DTidx[index_size];

	// fill table:
	for (DTidx i = 0; i < index_size - 1; i++) // fill first row
		M[0][i] = input[index[i]] <= input[index[i + 1]] ? i : i + 1;
	if (depth > 0) M[0][index_size - 1] = index_size - 1;          // fill overhang in first row

	DTidx dist = 1; // always 2^j
	for (DTidx j = 1; j < depth; j++) {
		dist *= 2;
		for (DTidx i = 0; i < index_size - dist; i++) // fill jth row
			M[j][i] = input[index[M[j - 1][i]]] <= input[index[M[j - 1][i + dist]]] ? M[j - 1][i] : M[j - 1][i + dist];
		for (DTidx i = index_size - dist; i < index_size; i++) M[j][i] = M[j - 1][i]; // overhang
	}
}

DTidx Big_RMQ::queryAlg(DTidx i, DTidx j) {
	if (j-i == 0) return j;
	if (j-i == 1) return M[0][i];
	DTidx k = Util::log2fast(j-i);
	DTidx twotothek = 1 << k; // 2^k
	return input[index[M[k - 1][i]]] <= input[index[M[k - 1][j + 1 - twotothek]]] ? M[k - 1][i] : M[k - 1][j + 1 - twotothek];
}

void Big_RMQ::deleteMem() {
	for (DTidx i = 0; i < depth; i++)
		delete[] M[i];
	delete[] M;
}

Big_RMQ::~Big_RMQ() {
	delete[] index;
	if (preprocess_complete) deleteMem();
}