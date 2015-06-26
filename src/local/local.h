#ifndef LOCAL_H_
#define LOCAL_H_

#include "../data_structures.h"
#include "../greedy/greedy.h"

using namespace std;

int localSearch(Node graph[], int n, int nodesUsedInSolution);
int localSearch2(Node graph[], int n, int nodesUsedInSolution);
bool belongsTo(forward_list<int> adj, int x);

#endif