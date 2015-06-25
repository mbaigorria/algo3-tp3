#include <iostream>
#include <algorithm>
#include "../data_structures.h"

using namespace std;

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