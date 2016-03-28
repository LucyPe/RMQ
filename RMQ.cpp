#include "RMQ.hpp"

RMQ::RMQ(DT* input) {
	this->input = input;
}

DTidx RMQ::findMin(DTidx i, DTidx j) {
	DTidx min = i;
	for (DTidx k = i + 1; k <= j; k++) {
		if (input[k] < input[min]) {
			min = k;
		}
	}
	return min;
}

void RMQ::preprocess(int preprocessIterationCount, int queryIterationCount) {
	this->preprocessIterationCount = preprocessIterationCount;
	this->queryIterationCount = queryIterationCount;

	preprocessingTime = 0;
	for (int i = 0; i < preprocessIterationCount; i++) {
		clock_t start_click = clock();
		preprocessAlg();
		preprocessingTime += (clock_t)ceil((double)(clock() - start_click));
		if (i != (preprocessIterationCount - 1)) {
			this->deleteMem();
		}
	}
	preprocessingTime /= preprocessIterationCount;
	preprocess_complete = true;
}

DTidx* RMQ::query(DTidx* queries, DTidx size) {
	DTidx* results = new DTidx[size]();

	clock_t start_click = clock();

	DTidx r = 0;
	for (DTidx q = 0; q < 2*size; q +=2) {
		DTidx i = queries[q];
		DTidx j = queries[q+1];

		for (int k = 0; k < queryIterationCount; k++) {
			results[r] = queryAlg(i, j);
		}
		r++;
	}

	queryTime = (clock_t)ceil((double)(clock() - start_click) / queryIterationCount);

	return results;
}

DTidx RMQ::query(DTidx i, DTidx j) {
	DTidx result;
	clock_t start_click = clock();
	
	result = this->queryAlg(i, j);

	queryTime = (clock_t)ceil((double)(clock() - start_click) / queryIterationCount);

	return result;
}

const DTidx Util::Catalan[17][17] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
	{ 0, 0, 2, 5, 9, 14, 20, 27, 35, 44, 54, 65, 77, 90, 104, 119, 135 },
	{ 0, 0, 0, 5, 14, 28, 48, 75, 110, 154, 208, 273, 350, 440, 544, 663, 798 },
	{ 0, 0, 0, 0, 14, 42, 90, 165, 275, 429, 637, 910, 1260, 1700, 2244, 2907, 3705 },
	{ 0, 0, 0, 0, 0, 42, 132, 297, 572, 1001, 1638, 2548, 3808, 5508, 7752, 10659, 14364 },
	{ 0, 0, 0, 0, 0, 0, 132, 429, 1001, 2002, 3640, 6188, 9996, 15504, 23256, 33915, 48279 },
	{ 0, 0, 0, 0, 0, 0, 0, 429, 1430, 3432, 7072, 13260, 23256, 38760, 62016, 95931, 144210 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 1430, 4862, 11934, 25194, 48450, 87210, 149226, 245157, 389367 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 4862, 16796, 41990, 90440, 177650, 326876, 572033, 961400 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16796, 58786, 149226, 326876, 653752, 1225785, 2187185 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 58786, 208012, 534888, 1188640, 2414425, 4601610 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 208012, 742900, 1931540, 4345965, 8947575 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 742900, 2674440, 7020405, 15967980 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2674440, 9694845, 25662825 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9694845, 35357670 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35357670 }
};

const char Util::LogTable256[256] =
{
	0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

DTidx Util::log2fast(DTidx v) {
	DTidx c = 0;          // c will be lg(v)
	register DTidx t, tt; // temporaries

	if (tt = v >> 16)
		c = (t = v >> 24) ? 24 + LogTable256[t] : 16 + LogTable256[tt & 0xFF];
	else
		c = (t = v >> 8) ? 8 + LogTable256[t] : LogTable256[v];
	return c;
}