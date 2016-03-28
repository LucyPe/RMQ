#ifndef _SegA_RMQ_hpp_
#define _SegA_RMQ_hpp_

#include "RMQ.hpp"

using namespace std;

/**
* Implementation of the <O(n), O(log(n))> method for RMQ
*/
class SegA_RMQ : public RMQ {
public:
	/**
	* Standard Constructor
	* input is the array to be prepared for RMQ
	* n is the size of the array
	*/
	SegA_RMQ(DT* input, DTidx n);
	~SegA_RMQ();

	virtual void deleteMem();

protected:
	// input array size
	DTidx n;

	// array for segment tree
	DTidx* seg_array;
	DTidx seg_array_size;

	// preproccessing implementation
	virtual void preprocessAlg();
	// query implementation
	virtual DTidx queryAlg(DTidx i, DTidx j);

	// recursive metod for finding min in interval <i,j>
	DTidx arrayQuery(DTidx i, DTidx j, DTidx a, DTidx b, DTidx level);
};
#endif