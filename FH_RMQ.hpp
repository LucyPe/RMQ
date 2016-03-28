#ifndef _FH_RMQ_hpp_
#define _FH_RMQ_hpp_

#include "RMQ.hpp"
#include "Big_RMQ.hpp"
#include <iostream>

using namespace std;

/**
* Implementation of the <O(n), O(1)> method for RMQ
*/
class FH_RMQ : public RMQ {
public:
	/**
	* Standard Constructor
	* input is the array to be prepared for RMQ
	* n is the size of the array
	*/
	FH_RMQ(DT* input, DTidx n);
	~FH_RMQ();

	virtual void deleteMem();

protected:
	// input array size
	DTidx n;
	// index array for the out-of-block queries (contains indices of block-minima)
	DTidx *index;

	// block size
	DTidx block_size;
	// number of blocks (always n/bs)
	DTidx block_count;
	// type of blocks
	DTidx *type;

	// number of different queries per block = bs*(bs+1)/2
	DTidx qpb;
	// precomputed in-block queries
	DTidx** Prec;

	// nlogn_1-Algo for out-of-block-block-queries:
	Big_RMQ* RMQ_ST;

	// preproccessing implementation
	virtual void preprocessAlg();
	// query implementation
	virtual DTidx queryAlg(DTidx i, DTidx j);

	// return block of entry i:
	inline DTidx block(DTidx i) { return i / block_size; }
};

#endif