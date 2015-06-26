#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <list>
#include "local.h"

using namespace std;

bool isReachable(Node graph[], int u);

/* Local search by adding 1 node
 * @param graph[] Array of nodes.
 * @param n Size of graph.
 * @param nodesUsedInSolution Size of current solution
 */
int localSearch(Node graph[], int n, int nodesUsedInSolution) {

    int currentNodes = nodesUsedInSolution;
    list<int> removed;

    for (int i = 0; i < n; ++i) {
        if (graph[i].added == true || graph[i].degree == 1) continue; // search for a node not in S.

        graph[i].added = true;
        currentNodes++;

        bool reachable = true;

		for (auto it = graph[i].adj.begin(); it != graph[i].adj.end(); ++it) { // iterate adj

			if (graph[*it].added == false) continue; // doesnt affect adj nodes.

			removed.push_front(*it);
			graph[*it].added = false; 
			currentNodes--;
			
			for (auto it2 = graph[*it].adj.begin(); it2 != graph[*it].adj.end(); ++it2) { // iterate adj to adj
				if (!isReachable(graph, *it2)) {
					reachable = false;
					goto stop;
				}
			}

		}

		stop:

		if (reachable == true && currentNodes < nodesUsedInSolution) { // build graph once we know we can improve it.
			nodesUsedInSolution = currentNodes;
			i = 0; // s <- s'
		} else {
			graph[i].added = false;
			while (removed.size() > 0) { // restore graph
				graph[removed.front()].added = true;
				removed.pop_front();
			}
			currentNodes = nodesUsedInSolution;
		}
    }

    return nodesUsedInSolution;
}

bool isReachable(Node graph[], int u) {
	for (auto it = graph[u].adj.begin(); it != graph[u].adj.end(); ++it) {
		if (graph[*it].added) {
			return true;
		}
	}
	return false;
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