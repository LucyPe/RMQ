#ifndef _RMQ_hpp_
#define _RMQ_hpp_

#include <math.h>
#include <ctime>

typedef int DT;                 // use long for 64bit-version (but take care of fast log!)
typedef unsigned int DTidx;     // for indexing in arrays

class Util {
public:
	static const DT minus_infinity = -2147483647 - 1;
	// the following stuff is for fast base 2 logarithms: (currently only implemented for 32 bit numbers)
	static const char LogTable256[256];
	static DTidx log2fast(DTidx);

	static const DTidx Catalan[17][17];
};

/* Abstract class for RMQ-queries.
* Proprocessing is done in constructor. */
class RMQ {
public:
	RMQ(DT* input);
	RMQ() {}

	// returns index of RMQ[i,j]
	DTidx query(DTidx, DTidx);
	DTidx* query(DTidx* queries, DTidx size);

	void preprocess(int preprocessIterationCount = 1, int queryIterationCount = 1);

	inline double getPreprocessingTime() const { return (double)this->preprocessingTime / CLOCKS_PER_SEC; }
	inline double getQueryTime() const { return (double)this->queryTime / CLOCKS_PER_SEC; }

	virtual void deleteMem() = 0;
	virtual ~RMQ() {};
protected:
	// a copy of the input array
	DT* input;

	// trivial O(n) find min
	DTidx findMin(DTidx i, DTidx j);

	clock_t preprocessingTime = 0;
	clock_t queryTime = 0;

	int preprocessIterationCount = 10;
	int queryIterationCount = 10;

	bool preprocess_complete = false;

	// special implementation for each algorithm
	virtual void preprocessAlg() = 0;
	virtual DTidx queryAlg(DTidx i, DTidx j) = 0;
};
#endif