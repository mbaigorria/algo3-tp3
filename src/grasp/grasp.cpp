#include <iostream>
#include <forward_list>
#include <algorithm>
#include <stdlib.h>

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
int greedyHeapConstructiveRandomized(Node graph[], int n, int k);
int localSearch(Node graph[], int n, int nodesUsedInSolution);
int graspMIDS(Node graph[], int n, int j, int k, bool localSolution[]);

int main() {

	int n, m; // n: vertices, m: edges
	cin >> n >> m;

	Node graph[n]; // graph container
	bool localSolution[n];

	int u, v;
	for (int i = 1; i <= m; ++i) { // (u,v) edges
		cin >> u >> v;
		u--; // nodes are counted from 0 in array.
		v--;
		graph[u].adj.push_front(v);
		graph[v].adj.push_front(u);
		
		graph[u].degree++;
		graph[v].degree++;
	}

	int initialNodes = 0;
	for (int i = 0; i < n; ++i) { // add d(v)=0 nodes to cover.
		if (graph[i].degree == 0) {
			graph[i].added = true;
			graph[i].reachable = true;
			localSolution[i] = true;
			initialNodes++;
		}
	}

	int nodesUsedInSolution = graspMIDS(graph, n, 3, 3, localSolution);

	// display solution
	cout << nodesUsedInSolution;
	for (int i = 0; i < n; ++i) {
		if (localSolution[i] == true) cout << " " << i + 1;
	}
	cout << endl;

	return 0;
}

/* GRASP Heuristic
 * Minimum Independent Dominating Set
 * @param j Amount of attempts to improve solution.
 * @param k Parameter used for greedy heuristic.
 */
int graspMIDS(Node graph[], int n, int j, int k, bool localSolution[]) {
	int currentBest = n + 1;
	while (j > 0) {
		int nodesUsed = greedyHeapConstructiveRandomized(graph, n, k);
		nodesUsed = localSearch(graph, n, nodesUsed);

		if (nodesUsed < currentBest) { // save local solution
			for (int i = 0; i < n; ++i) {
				localSolution[i] = graph[i].added;
			}
			currentBest = nodesUsed;
		}

		j--;
	}
	return currentBest;
}

int greedyHeapConstructiveRandomized(Node graph[], int n, int k) {

	if (k == 0) {
		return 0;
	}

	vector<_Pair> currentPicks;
	vector<_Pair> heap;
	int nodesUsed = 0;

	for (int i = 0; i < n; i++) {
		if (graph[i].degree == 1) {
			graph[i].added = true;
			nodesUsed++;
		} else {
			graph[i].added = false;
			graph[i].reachable = false;
 			heap.push_back(_Pair(graph[i].score, i));
		}
	}
	make_heap(heap.begin(), heap.end());

	int i = 0;
	while (i < k && i < (int) heap.size()) {
		_Pair p = heap.front();
		currentPicks.push_back(p);
		pop_heap(heap.begin(), heap.end());
		heap.pop_back();
		i++;
	}

	while (currentPicks.size() > 0) {
		int id = rand() % currentPicks.size();
		// cout << "picked id " << id << endl;
		_Pair p = currentPicks.at(id);
		currentPicks.erase(currentPicks.begin() + id);

		// cout << "node id " << p.id << endl;

		if (heap.size() > 0) {
			// cout << heap.size() << endl;
			_Pair p2 = heap.front();
			currentPicks.push_back(p2);
			pop_heap(heap.begin(), heap.end());
			heap.pop_back();
		}

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

int localSearch(Node graph[], int n, int nodesUsedInSolution) {

    int currentNodes = nodesUsedInSolution;

    for (int i = 0; i < n; ++i) {
        if (graph[i].added == true || graph[i].degree == 1) continue; // search for a node not in S.
        currentNodes++;

        bool reachable;

		for (auto it = graph[i].adj.begin(); it != graph[i].adj.end(); ++it) {

			if (graph[*it].added == false) continue; // we already know its reachable.

			reachable = false; // flag that indicates if all removed nodes are reachable.

			int adjNode = *it;
			currentNodes--;

			for (auto it2 = graph[adjNode].adj.begin(); it2 != graph[adjNode].adj.end(); ++it2) {
				if (adjNode == *it2) continue;
				if (graph[*it2].added == true) { // if the adj to the adj is added, can remove safely.
					reachable = true;
					break;
				}
			}
			if (reachable == false) break;
		}

		if (reachable == true && currentNodes < nodesUsedInSolution) { // build graph once we know we can improve it.
			graph[i].added = true;
			for (auto it = graph[i].adj.begin(); it != graph[i].adj.end(); ++it) {
				graph[*it].added = false;
			}
			nodesUsedInSolution = currentNodes;
			i = 0; // s <- s'
		} else {
			currentNodes = nodesUsedInSolution;
		}
    }

    return nodesUsedInSolution;
}