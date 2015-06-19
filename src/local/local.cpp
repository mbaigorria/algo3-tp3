#include <iostream>
#include <forward_list>

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

	Node graph1[n]; // graph1 container

	int nodesUsedInSolution = 0;

    int u, v;
	for (int i = 1; i <= m; ++i) { // (u,v) edges
		cin >> u >> v;
		u--; // nodes are counted from 0 in array.
		v--;
		graph1[u].adj.push_front(v);
		graph1[v].adj.push_front(u);
		graph1[u].score++;
		graph1[v].score++;
	}

    /* This script can be improved by:
	 *    1. Using some sort of 'dynamic heap'.
	 *    2. Not iterating degree 0 nodes.
	 *    3. Using a list instead of an array, not to iterate
	 		 through nodes that are not necessary.
	*/

    // greedy algorithm:---------------------------------------------------------------------------------------------

	for (int i = 0; i < n; ++i) {

		int greatest = 0;
		unsigned int score = 0;
		bool flag = false;

		// search for max score.
		for (int j = 0; j < n; ++j) {
			if (graph1[j].reachable == true) continue;
			if (graph1[j].score >= score) { // can be improved here!
				greatest = j;
				score    = graph1[j].score;
				flag = true;
			}
		}

		if (!flag) break; // no more nodes to search.

		graph1[greatest].added = true;
		graph1[greatest].reachable = true;

		// update adyacent nodes of reachable nodes scores.
		for (auto it = graph1[greatest].adj.begin(); it != graph1[greatest].adj.end(); ++it) {
			int adjNode = *it;
			graph1[adjNode].reachable = true;
			for (auto it2 = graph1[adjNode].adj.begin(); it2 != graph1[adjNode].adj.end(); ++it2) {
				graph1[*it2].score--;
			}
		}

		nodesUsedInSolution++;
	}

	//end of greedy algorithm--------------------------------------------------------------------------------------

	Node graph2[n]; // graph2 container

	for (int i = 0; i < n; ++i) graph2[i] = graph1[i];

    int nodesUsedInLocalSolution = nodesUsedInSolution;
    int currentNodes = nodesUsedInLocalSolution;

    for (int i = 0; i < n; ++i) {
        if( graph1[i].added == false ) continue;


        graph2[i].added = false;
        currentNodes--;
        bool flagAdj = false; //if there at least one adjacent node used to the current node in the partial solution.

        for (auto it = graph2[i].adj.begin(); it != graph2[i].adj.end(); ++it) {
            int adjNode = *it;
            graph2[adjNode].added = true;
            currentNodes++;

            for (auto it2 = graph2[adjNode].adj.begin(); it2 != graph2[adjNode].adj.end(); ++it2) {
                if(graph2[*it2].added == true){
                    graph2[adjNode].added = false;
                    currentNodes--;
                    break;
                }
            }
            if (graph2[adjNode].added == true) flagAdj = true;
        }

        if (currentNodes > nodesUsedInLocalSolution || flagAdj == false) {
            graph2[i].added = true;
            currentNodes = nodesUsedInLocalSolution;
            for (auto it = graph2[i].adj.begin(); it != graph2[i].adj.end(); ++it) graph2[*it].added = false;
        }else
            nodesUsedInLocalSolution = currentNodes;
    }

    //end of local algorithm--------------------------------------------------------------------------------------

    // display solution
    if(nodesUsedInLocalSolution <= nodesUsedInSolution){

        cout << nodesUsedInLocalSolution;
        for (int i = 0; i < n; ++i) {
            if (graph2[i].added == true) cout << " " << i + 1;
        }
        cout << endl;

    }else{

        cout << nodesUsedInSolution;
        for (int i = 0; i < n; ++i) {
            if (graph1[i].added == true) cout << " " << i + 1;
        }
        cout << endl;

    }

 	return 0;
}
