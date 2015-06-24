#include <iostream>
#include <forward_list>
#include <algorithm>

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

void displaySolution(Node graph[], int n, int nodesUsedInSolution);
int greedyConstructive(Node graph[], int n);
int greedyHeapConstructive(Node graph[], int n);

int main() {

	int n, m; // n: vertices, m: edges
	cin >> n >> m;

	Node graph[n]; // graph container
	
	int u, v;
	for (int i = 1; i <= m; ++i) { // (u,v) edges
		cin >> u >> v;
		u--; // nodes are counted from 0 in array.
		v--;
		graph[u].adj.push_front(v);
		graph[v].adj.push_front(u);

		graph[u].degree++;
		graph[v].degree++;
		graph[u].score++;
		graph[v].score++;
	}

	int initialNodes = 0;
	for (int i = 0; i < n; ++i) { // add d(v)=0 nodes to cover.
		if (graph[i].degree == 0) {
			graph[i].added = true;
			graph[i].reachable = true;
			initialNodes++;
		}
	}

	int nodesUsedInSolution = greedyConstructive(graph, n);
	// int nodesUsedInSolution = greedyHeapConstructive(graph, n);

	displaySolution(graph, n, nodesUsedInSolution + initialNodes);

	return 0;
}

void displaySolution(Node graph[], int n, int nodesUsedInSolution) {
	cout << nodesUsedInSolution;
	for (int i = 0; i < n; ++i) {
		if (graph[i].added == true) cout << " " << i + 1;
	}
	cout << endl;
}

int greedyHeapConstructive(Node graph[], int n) {

	vector<_Pair> heap;
	int nodesUsed = 0;

	for (int i = 0; i < n; i++) {
		if (graph[i].added == false)
			heap.push_back(_Pair(graph[i].score, i));
	}
	make_heap(heap.begin(), heap.end());

	for (int i = 0; i < n; i++) {
		_Pair p = heap.front();
		pop_heap(heap.begin(), heap.end());
		heap.pop_back();

		if (graph[p.id].reachable == true) continue;

		graph[p.id].added = true;
		nodesUsed++;

		for (auto it = graph[p.id].adj.begin(); it != graph[p.id].adj.end(); ++it) {
			int adjNode = *it;
			graph[adjNode].reachable = true;
		}
	}

	return nodesUsed;
}

/** 
* This function can be improved by:
* 1. Using some sort of 'dynamic heap'.
* 2. Not iterating degree 0 nodes.
* 3. Using a list instead of an array, not to iterate
*    through nodes that are not necessary.
*/
int greedyConstructive(Node graph[], int n) {

	int nodesUsedInSolution = 0;

	for (int i = 0; i < n; ++i) {

		int greatest = 0;
		int score = 0;
		bool flag = false;

		// search for max score.
		for (int j = 0; j < n; ++j) {
			if (graph[j].reachable == true) continue;
			if (graph[j].score >= score) { // can be improved here!
				greatest = j;
				score    = graph[j].score;
				flag = true;
			}
		}

		if (!flag) break; // no more nodes to search.

		graph[greatest].added = true;
		graph[greatest].reachable = true;

		// update adyacent nodes of reachable nodes' scores.
		for (auto it = graph[greatest].adj.begin(); it != graph[greatest].adj.end(); ++it) {
			int adjNode = *it;
			graph[adjNode].reachable = true;
			for (auto it2 = graph[adjNode].adj.begin(); it2 != graph[adjNode].adj.end(); ++it2) {
				graph[*it2].score--;
			}
		}

		nodesUsedInSolution++;
	}

	return nodesUsedInSolution;
}