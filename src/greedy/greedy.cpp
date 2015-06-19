#include <iostream>
#include <forward_list>
#include <algorithm>

using namespace std;

struct Node {
	forward_list<int> adj;
	unsigned int score;
	bool added;
	bool reachable;

	Node() {
		score = 0;
		added = false;
		reachable = false;
	}

};

int main() {

	int n, m; // n: vertices, m: edges
	cin >> n >> m;

	Node graph[n]; // graph container
	int nodesUsedInSolution = 0;
	
	int u, v;
	for (int i = 1; i <= m; ++i) { // (u,v) edges
		cin >> u >> v;
		u--; // nodes are counted from 0 in array.
		v--;
		graph[u].adj.push_front(v);
		graph[v].adj.push_front(u);
		graph[u].score++;
		graph[v].score++;
	}

	/** This script can be improved by:
	 *    1. Using some sort of 'dynamic heap'.
	 *    2. Not iterating degree 0 nodes.
	 *    3. Using a list instead of an array, not to iterate
	 *		 through nodes that are not necessary.
	 */

	for (int i = 0; i < n; ++i) {

		int greatest = 0;
		unsigned int score = 0;
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

		// update adyacent nodes of reachable nodes scores.
		for (auto it = graph[greatest].adj.begin(); it != graph[greatest].adj.end(); ++it) {
			int adjNode = *it;
			graph[adjNode].reachable = true;
			for (auto it2 = graph[adjNode].adj.begin(); it2 != graph[adjNode].adj.end(); ++it2) {
				graph[*it2].score--;
			}
		}

		nodesUsedInSolution++;
	}

	// display solution
	cout << nodesUsedInSolution;
	for (int i = 0; i < n; ++i) {
		if (graph[i].added == true) cout << " " << i + 1;
	}
	cout << endl;

	return 0;
}