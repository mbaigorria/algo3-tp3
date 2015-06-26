#ifndef GREEDY_H_
#define GREEDY_H_

#include "../data_structures.h"

using namespace std;

int greedyConstructive(Node graph[], int n);
int greedyHeapConstructive(Node graph[], int n);
int greedyHeapConstructiveRandomized(Node graph[], int n, int k);
int greedyHeapConstructiveRandomized2(Node graph[], int n, int k);

#endif