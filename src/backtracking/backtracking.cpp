#include <iostream>
#include <forward_list>

using namespace std;

struct Node {
	forward_list<int> adj;
	unsigned int degree;
	bool added;
	bool reachable;

	Node() {
		degree = 0;
		added = false;
		reachable = false;
	}
};

void backtracking(int current, int n, int covered, int usedNodes, Node graph[], bool localSolution[], int& nodesUsed);

int main() {

	int n, m; // n: vertices, m: edges
	cin >> n >> m;

	Node graph[n]; // declare graph container
	
	bool localSolution[n];
	int nodesUsed = n;

	int u, v;
	for (int i = 1; i <= m; ++i) { // (u,v) edges
		cin >> u >> v;
		u--;
		v--;
		graph[u].adj.push_front(v);
		graph[v].adj.push_front(u);
		graph[u].degree++;
		graph[v].degree++;
	}

	cout << graph[0].degree;

	backtracking(0, n, 0, 0, graph, localSolution, nodesUsed);

	return 0;
}

void backtracking(int current, int n, int covered, int usedNodes, Node graph[], bool localSolution[], int& nodesUsed) {
	if (graph[current].reachable == true) return backtracking((current++), n, covered, usedNodes, graph, localSolution, nodesUsed);
	if (current == n) return;
	if (nodesUsed == usedNodes + 1) return;

	int pushed = 0;
	forward_list<int> added;
	graph[current].added = true;
	for (auto it = graph[current].adj.begin(); it != graph[current].adj.end(); ++it) {
		int adjNode = *it;
		if (graph[adjNode].reachable == false) {
			graph[adjNode].reachable = true;
			added.push_front(adjNode);
			pushed++;
		}
	}

	if ((covered + pushed + 1) == n) { // load local solution
		for (int i = 0; i < n; ++i) {
			localSolution[i] = graph[i].added;
		}
		nodesUsed = usedNodes++;
	} else {
		backtracking((current++), n, (covered + pushed + 1), (usedNodes + 1), graph, localSolution, nodesUsed); // adding current element to coverage
	}

	graph[current].added = false;
	// restore reachable nodes
	for (auto it = added.begin(); it != added.end(); ++it) {
		graph[*it].reachable = false;
	}

	backtracking((current++), n, covered, usedNodes, graph, localSolution, nodesUsed);

}