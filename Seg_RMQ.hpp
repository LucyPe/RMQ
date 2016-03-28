#ifndef _Seg_RMQ_hpp_
#define _Seg_RMQ_hpp_

#include "RMQ.hpp"

using namespace std;

/**
* Implementation of the <O(n), O(log(n))> method for RMQ
*/
class Seg_RMQ : public RMQ {
private:
	typedef struct Node {
		Node* left;
		Node* right;
		DTidx min;
	}
	Node;

public:
	Seg_RMQ(DT* input, DTidx n);
	~Seg_RMQ();

	virtual void deleteMem();

protected:
	// input array size
	DTidx n;
	
	// root of segment tree
	Node* root;

	// preproccessing implementation
	virtual void preprocessAlg();
	// query implementation
	virtual DTidx queryAlg(DTidx i, DTidx j);

	// recursive metod for creating segment tree
	Node* createNode(DTidx i, DTidx j);
	// recursive metod for finding min in interval <i,j>
	DTidx getNode(DTidx i, DTidx j, DTidx a, DTidx b, Node* node);
	// recursive metod for destroying segment tree
	void deleteNode(Node* node);
};
#endif