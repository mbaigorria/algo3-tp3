#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include "../containers.h"

using namespace std;

#define E_INVALID_PARAMETER 0

/* Gready Constructive Randomized Heuristic for MIDS
 * Using a heap, this function builds a MIDS by picking vertices
 * randomly from the top k vertices with the highest degree.
 * 
 * @param graph[] Array of nodes.
 * @param n Size of graph.
 * @param k Parameter that indicates from how many nodes to
 *			pick randomly 
 * @return Nodes used in solution set.
 */
int greedyHeapConstructiveRandomized(Node graph[], int n, int k) {

	if (k == 0) return E_INVALID_PARAMETER;

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
		_Pair p = currentPicks.at(id);
		currentPicks.erase(currentPicks.begin() + id);

		if (heap.size() > 0) {
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

/* Gready Constructive Randomized Heuristic for MIDS
 * Using a heap, this function builds a MIDS by picking vertices
 * randomly from the vertices that are k degrees away from the
 * available vertex with the highest degree.
 * 
 * @param graph[] Array of nodes.
 * @param n Size of graph.
 * @param k Parameter that indicates from how many nodes to
 *			pick randomly 
 * @return Nodes used in solution set.
 */
int greedyHeapConstructiveRandomized2(Node graph[], int n, int k) {

	if (k == 0) return E_INVALID_PARAMETER;

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
 			heap.push_back(_Pair(graph[i].degree, i));
		}
	}
	make_heap(heap.begin(), heap.end());

	int i = 0;
	int degree = heap.front().degree;
	while(heap.front().degree > degree - k && i < (int) heap.size()) {
		_Pair p = heap.front();
		currentPicks.push_back(p);
		pop_heap(heap.begin(), heap.end());
		heap.pop_back();
		i++;
	}

	while (currentPicks.size() > 0) {
		int id = rand() % currentPicks.size();
		_Pair p = currentPicks.at(id);
		currentPicks.erase(currentPicks.begin() + id);

		degree = currentPicks.at(0).degree; // update degree

		if (heap.size() > 0 && heap.front().degree > degree - k) {
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

/* Greedy Constructive Heuristic for MIDS
 * Using a heap, this function builds a MIDS by picking
 * the highest degree vertex repeatedly.
 * 
 * @param graph[] Array of nodes.
 * @param n Size of graph.
 * @return Nodes used in solution set.
 */

int greedyHeapConstructive(Node graph[], int n) {

	vector<_Pair> heap;
	int nodesUsed = 0;

	for (int i = 0; i < n; i++) {
		if (graph[i].degree == 0) {
			graph[i].added = true;
			graph[i].reachable = true;
			nodesUsed++;
		} else {
			heap.push_back(_Pair(graph[i].score, i));
		}
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

/* Greedy Constructive Heuristic for MIDS
 * This function builds a MIDS by picking vertices by score.
 * The score is defined as the number of effective reachable
 * vertices given the vertices that have already been picked.
 * 
 * @param graph[] Array of nodes.
 * @param n Size of graph.
 * @return Nodes used in solution set.
 */
int greedyConstructive(Node graph[], int n) {

	int nodesUsed = 0;

	for (int i = 0; i < n; i++) {
		if (graph[i].degree == 0) {
			graph[i].added = true;
			graph[i].reachable = true;
			nodesUsed++;
		}
	}

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

		nodesUsed++;
	}

	return nodesUsed;
}