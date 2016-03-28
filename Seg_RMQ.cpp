#include "Seg_RMQ.hpp"
#include <algorithm>

Seg_RMQ::Seg_RMQ(DT* input, DTidx n) : RMQ(input) {
	this->n = n;
}

void Seg_RMQ::preprocessAlg() {
	root = createNode(0, n - 1);
}

DTidx Seg_RMQ::queryAlg(DTidx i, DTidx j) {
	return getNode(i, j, 0, n - 1, root);
}

Seg_RMQ::Node* Seg_RMQ::createNode(DTidx i, DTidx j) {
	if (i == j) {
		return NULL;
	}

	Node* node = new Node();
	
	// create children
	DTidx m = (i + j) / 2;
	node->left = this->createNode(i, m);
	node->right = this->createNode(m + 1, j);

	// compute min
	DTidx left_min = (node->left != NULL) ? node->left->min : m;
	DTidx right_min = (node->right != NULL) ? node->right->min : m + 1;
	node->min = (input[left_min] <= input[right_min]) ? left_min : right_min;

	return node;
}

DTidx Seg_RMQ::getNode(DTidx i, DTidx j, DTidx a, DTidx b, Node* node) {
	// <a, b> is a subset of <i, j>
	if ((i <= a) && (b <= j)) {
		return node->min;
	}

	DTidx m = (a + b) / 2;

	// <a, k> overlaps with <i, j>, recurse on left child
	bool goLeft = (a <= j) && (i <= m);
	DTidx left_min = (goLeft && (node->left != NULL)) ? getNode(i, j, a, m, node->left) : m;

	// <k + 1, b> overlaps with <i, j>, recurse on right child
	bool goRight = ((m + 1) <= j) && (i <= b);
	DTidx right_min = (goRight && (node->right != NULL)) ? getNode(i, j, m + 1, b, node->right) : m + 1;


	if (goLeft && goRight) {
		return (input[left_min] <= input[right_min]) ? left_min : right_min;
	}
	return goLeft ? left_min : right_min;
}

void Seg_RMQ::deleteNode(Node* node) {
	if (node->left != NULL) {
		deleteNode(node->left);
	}
	if (node->right != NULL) {
		deleteNode(node->right);
	}
	delete node;
}

void Seg_RMQ::deleteMem() { 
	deleteNode(root);
}

Seg_RMQ::~Seg_RMQ() {
	if (preprocess_complete) deleteNode(root);
}