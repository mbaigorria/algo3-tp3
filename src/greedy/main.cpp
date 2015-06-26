#include <iostream>
#include "greedy.h"

using namespace std;

void displaySolution(Node graph[], int n, int nodesUsedInSolution);

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

	// int nodesUsedInSolution = greedyConstructive(graph, n);
	// int nodesUsedInSolution = greedyHeapConstructive(graph, n);
	int nodesUsedInSolution = greedyHeapConstructiveRandomized(graph, n, 3);

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