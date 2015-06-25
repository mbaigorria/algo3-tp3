#include <iostream>
#include <forward_list>
#include <algorithm>
#include <stdlib.h>
#include "../greedy/greedy.h"
#include "../local/local.h"

using namespace std;

void displaySolution(Node graph[], int n, int nodesUsedInSolution);
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


void displaySolution(Node graph[], int n, int nodesUsedInSolution) {
	cout << nodesUsedInSolution;
	for (int i = 0; i < n; ++i) {
		if (graph[i].added == true) cout << " " << i + 1;
	}
	cout << endl;
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