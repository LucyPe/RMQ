#ifndef _Triv_RMQ_hpp_
#define _Triv_RMQ_hpp_

#include "RMQ.hpp"
#include <stdlib.h>

using namespace std;

/**
* Implementation of the <O(1), O(n)> trivial method for RMQ
*/
class Triv_RMQ : public RMQ {
public:
	/**
	* Standard Constructor
	* input is the array to be prepared for RMQ
	*/
	Triv_RMQ(DT* input) : RMQ(input) {}

	virtual void deleteMem() {};

protected:
	// preproccessing implementation
	virtual void preprocessAlg() {}
	// query implementation
	virtual DTidx queryAlg(DTidx i, DTidx j) { return findMin(i, j); }
};

#endif