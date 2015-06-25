#include <iostream>
#include <forward_list>

using namespace std;

struct Node {
	unsigned int degree;
	unsigned int score;
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

void displaySolution(Node graph[], int n, int nodesUsedInSolution);
int greedyConstructive(Node graph[], int n);
int localSearch(Node graph[], int n, int nodesUsedInSolution);

int main() {

	int n, m; // n: vertices, m: edges
	cin >> n >> m;

	Node graph[n]; // graph1 container

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

	nodesUsedInSolution = localSearch(graph, n, nodesUsedInSolution);

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

int greedyConstructive(Node graph[], int n) {

	int nodesUsedInSolution = 0;

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

	return nodesUsedInSolution;
}


int localSearch(Node graph[], int n, int nodesUsedInSolution) {

    int currentNodes = nodesUsedInSolution;

    for (int i = 0; i < n; ++i) {
        if (graph[i].added == true || graph[i].degree == 1) continue; // search for a node not in S.
        currentNodes++;

        bool reachable;

		for (auto it = graph[i].adj.begin(); it != graph[i].adj.end(); ++it) {
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