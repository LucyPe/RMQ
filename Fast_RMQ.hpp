#ifndef _Fast_RMQ_hpp_
#define _Fast_RMQ_hpp_

#include "RMQ.hpp"
#include "Big_RMQ.hpp"

using namespace std;

/**
* Implementation of the <O(n), O(log(n))> method for RMQ
*/
class Fast_RMQ : public RMQ {
public:
	/**
	* Standard Constructor
	* input is the array to be prepared for RMQ
	* n is the size of the array
	*/
	Fast_RMQ(DT* input, DTidx n);
	~Fast_RMQ();

	virtual void deleteMem();

protected:
	// input array size
	DTidx n;
	// block size
	DTidx block_size;
	// number of blocks (always n/bs)
	DTidx block_count;

	// index array for the out-of-block queries (contains indices of block-minima)
	DTidx* block_mins;

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
