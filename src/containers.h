#ifndef DATA_STRUCTURES_H_
#define DATA_STRUCTURES_H_

#include <forward_list>

using namespace std;

struct Node {
	int degree;
	int score;
	bool added;
	bool reachable;
	forward_list<int> adj;

	Node() {
		degree = 0;
		score = 0;
		added = false;
		reachable = false;
	}
};

struct _Pair {
	int degree;	
	int id;

	_Pair(int _degree, int _id) {
		degree = _degree;
		id = _id;
	}

	bool operator <(const _Pair& x) {
		return this->degree < x.degree;
	}
};

#endif