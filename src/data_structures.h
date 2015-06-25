#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

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
	int score;	
	int id;

	_Pair(int _score, int _id) {
		score = _score;
		id = _id;
	}

	bool operator <(const _Pair& x) {
		return this->score < x.score;
	}
};

#endif