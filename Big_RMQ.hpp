#ifndef _Big_RMQ_hpp_
#define _Big_RMQ_hpp_

#include "RMQ.hpp"

using namespace std;

/**
* Implementation of the <O(n log n), O(1)> method for RMQ
* as described in Bender and Farach's paper, section 3
*/
class Big_RMQ : public RMQ {
public:
	Big_RMQ(DT* input, DTidx n);
	/**
	* Standard Constructor.
	* input[index[0]],...,input[index[n-1]] is the array to be prepared for RMQ
	* "index_size" is the size of the index array "index".
	*/
	Big_RMQ(DT* input, DTidx* index, DTidx index_size);
	~Big_RMQ();

	virtual void deleteMem();

protected:
	// index array for input array:
	DTidx* index;

	// size of index array
	DTidx index_size;

	// depth of table
	DTidx depth;

	// the precomputed table:
	DTidx** M;

	// preproccessing implementation
	virtual void preprocessAlg();
	// query implementation
	virtual DTidx queryAlg(DTidx i, DTidx j);
};

#endif