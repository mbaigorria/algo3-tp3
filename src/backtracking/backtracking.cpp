#include <iostream>
#include <forward_list>
#include "../containers.h"

using namespace std;

void backtracking(int current, int& n, int coveredNodes, int usedNodes, Node graph[], bool localSolution[], int& nodesUsedInSolution);

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

	int nodesUsedInSolution = n; // worst case scenario is n, that way I avoid setting all the array as true.

	backtracking(0, n, initialNodes, initialNodes, graph, localSolution, nodesUsedInSolution);
	
	// display solution
	cout << nodesUsedInSolution;
	for (int i = 0; i < n; ++i) {
		if (localSolution[i] == true) cout << " " << i + 1;
	}
	cout << endl;

	return 0;
}

void backtracking(int current, int& n, int coveredNodes, int usedNodes, Node graph[], bool localSolution[], int& nodesUsedInSolution) {

	if (current == n) return; // no nodes left to add.
	if (graph[current].reachable == true) return backtracking(current + 1, n, coveredNodes, usedNodes, graph, localSolution, nodesUsedInSolution);
	// if (usedNodes + 1 == nodesUsedInSolution) return; // cant beat current solution

	int pushed = 0;
	forward_list<int> added; // save changes to graph to then restore
	graph[current].added = true;

	for (auto it = graph[current].adj.begin(); it != graph[current].adj.end(); ++it) {
		int adjNode = *it;
		if (graph[adjNode].reachable == false) { // node reaches these new vertices
			graph[adjNode].reachable = true;
			added.push_front(adjNode);
			++pushed;
		}
	}

	int tempCoveredNodes = coveredNodes + pushed + 1;
	if (tempCoveredNodes == n) { // coverage found
		for (int i = 0; i < n; ++i) {
			localSolution[i] = graph[i].added;
		}
		nodesUsedInSolution = ++usedNodes;
	} else {
		backtracking(current + 1, n, tempCoveredNodes, usedNodes + 1, graph, localSolution, nodesUsedInSolution); // adding current element to coverage
	}

	// restore graph state
	graph[current].added = false;
	for (auto it = added.begin(); it != added.end(); ++it) {
		graph[*it].reachable = false;
	}

	backtracking(current + 1, n, coveredNodes, usedNodes, graph, localSolution, nodesUsedInSolution); // skip current node

}