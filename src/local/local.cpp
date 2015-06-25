#include <iostream>
#include <forward_list>
#include <algorithm>

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
int localSearch(Node graph[], int n, int nodesUsedInSolution);
int localSearch2(Node graph[], int n, int nodesUsedInSolution);
bool belongsTo(forward_list<int> adj, int x);

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
	// int nodesUsedInSolution = greedyHeapConstructive(graph, n);
	
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

int localSearch2(Node graph[], int n, int nodesUsedInSolution) {

    int currentNodes = nodesUsedInSolution;

    for (int i = 0; i < n; ++i) {

    	// find index of two nodes not in S.
        if (graph[i].added == true || graph[i].degree == 1) continue;

        int j;
        for (j = i + 1; j < n; j++) { // search for a second node
        	if (graph[j].added == true || graph[j].degree == 1) continue;

	        if (j == n) break; // no pair found

	        // cout << "i: " << i << " j: " << j << endl;

	        // check if S with these 2 nodes and without adj nodes is a 'better' cover.
	        currentNodes = currentNodes + 2;
	        bool reachable;

			for (auto it = graph[i].adj.begin(); it != graph[i].adj.end(); ++it) {

				reachable = false; // flag that indicates if all removed nodes are reachable.

				int adjNode = *it;

				if (graph[*it].added == true) currentNodes--; 

				for (auto it2 = graph[adjNode].adj.begin(); it2 != graph[adjNode].adj.end(); ++it2) {
					if (adjNode == *it2) continue;
					if ((graph[*it2].added == true || *it2 == j) && !belongsTo(graph[j].adj, *it2)) { // if the adj to the adj is added, can remove safely.
						reachable = true;
						break;
					}
				}
				if (reachable == false) break;
			}

			if (reachable == true) {

				for (auto it = graph[j].adj.begin(); it != graph[j].adj.end(); ++it) {

					reachable = false; // flag that indicates if all removed nodes are reachable.

					int adjNode = *it;
					
					if (graph[*it].added == true && !belongsTo(graph[i].adj, *it)) currentNodes--;

					for (auto it2 = graph[adjNode].adj.begin(); it2 != graph[adjNode].adj.end(); ++it2) {
						if (adjNode == *it2) continue;
						if ((graph[*it2].added == true || *it2 == i) && !belongsTo(graph[i].adj, *it2)) { // if the adj to the adj is added, can remove safely.
							reachable = true;
							break;
						}
					}
					if (reachable == false) break;
				}
			}

			if (reachable == true && currentNodes < nodesUsedInSolution) { // build graph once we know we can improve it.
				// cout << "currentNodes: " << currentNodes << " nodesUsedInSolution: " << nodesUsedInSolution << endl;
				graph[i].added = true;
				graph[j].added = true;
				for (auto it = graph[i].adj.begin(); it != graph[i].adj.end(); ++it) {
					graph[*it].added = false;
				}
				for (auto it = graph[j].adj.begin(); it != graph[j].adj.end(); ++it) {
					graph[*it].added = false;
				}
				nodesUsedInSolution = currentNodes;
				i = 0; // s <- s'
			} else {
				currentNodes = nodesUsedInSolution;
			}
	    }
	}

    return nodesUsedInSolution;
}

bool belongsTo(forward_list<int> adj, int x) {
	for (auto it = adj.begin(); it != adj.end(); ++it) {
		if (*it == x) return true;
	}
	return false;
}