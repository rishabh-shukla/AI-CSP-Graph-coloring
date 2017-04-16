//contains all the headers used as well as misc functions
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <numeric>
#include <climits>
#include <algorithm>
#include <queue>
#include <typeinfo>
#include <string>
#pragma once 
using namespace std;

//pragma is used to avoid ambiguity
//number of nodes in the graph
#define NODES 7
//number of colors allowed
#define COLOR 3
//maximum number of steps allowed in local search
#define MAX 100

typedef unsigned long long timestamp_t;

vector <vector <int> > color (NODES, vector<int>(COLOR, 1));

static timestamp_t get_timestamp () {
	//to calculate the time in running an algorithm
  struct timeval now;
  gettimeofday (&now, NULL);
  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

class Edge {
	// data structure for the edges in the graph
    public:
        int source, destination;
        Edge (int i, int j){
            source = i;
            destination = j;
    }
};

class Graph {
	//adjacency list representation for the graph
    public:
        vector<int> adjacency_list [NODES];
        Graph(vector<Edge> edges) {
            for (unsigned i = 0; i < edges.size(); i++) {
                int source = edges[i].source;
                int destination = edges[i].destination;
                
                adjacency_list[source].push_back(destination);
                adjacency_list[destination].push_back(source);

            }
        }
};

void print_graph(Graph const& graph) {
	//function to print a graph
    for (int i = 0; i < NODES; i++)  {
        cout << i << " : ";

        for (int j=0; j<graph.adjacency_list[i].size();j++)
            cout << graph.adjacency_list[i][j] << " ";
        cout << endl;
    }
}

int max_cvar (Graph const& graph, vector<int> assigned){
	//Maximum Constained Variable
	/* 
	While selecting an unassigned variable to assign a colour, 
	we should choose the variable with minimum number of legal values 
	i.e. variable with minimum values in its domain. Also known as 
	Maximum Constraining Variable, this function picks a variable that
	is most likely to cause a failure soon, thereby pruning the search
	tree. 
	*/

	int max_value = 0;
	int min_color = INT_MAX;
	int allowed = 0;
	int value = 0;
	vector<int> ans;

	for(int i=0; i<NODES; i++){
		value = graph.adjacency_list[i].size();
		allowed = accumulate(color[i].begin(), color[i].end(), 0);

		if(allowed < min_color && assigned[i] == -1)
			min_color = allowed;

		if(value > max_value && assigned[i] == -1)
			max_value = value;
	}

	for(int i=0; i<NODES; i++){
		allowed = accumulate(color[i].begin(), color[i].end(), 0);
		if(allowed == min_color && assigned[i] == -1)
			ans.push_back(i);
	}

	if(ans.size() == 1)
		return ans[0];

	for(int i=0; i<NODES; i++){
		if(graph.adjacency_list[i].size() == max_value && assigned[i] == -1)
			return i;
	}
}

bool func_sort(const pair<int, int> &a, const pair<int, int> &b) {
	return a.second > b.second;
}

int min_cval (Graph const& graph, int node, int turn, vector<int> assigned) {
	//Least constrainig value
	/* 
	Once a variable has been selected, the algorithm must decide
	on the order in which to examine its values. This heuristic 
	prefers the value that rules out the fewest choices for the 
	neighbouring variables in the constraint graph. It tries to
	leave the maximum flexibility for subsequent variable assignments.   
	*/
	int max_value = INT_MIN;
	int sum = 0;
	vector <pair <int, int> > values;

	for(int i=0; i<COLOR; i++) {
		if(accumulate(color[node].begin(), color[node].end(), 0) == 0)
			return -1;
	}

	for(int i=0; i<COLOR; i++) {
		for(int j=0; j<graph.adjacency_list[node].size(); j++) {
			int next = graph.adjacency_list[node][j];

			if(assigned[next] == -1)
				sum += accumulate(color[next].begin(), color[next].end(), 0);

			if(color[next][i] == 1 && assigned[next] == -1)
				sum = sum - 1;
		}
	values.push_back(make_pair(i, sum));
	sum = 0;
	}

	sort(values.begin(), values.end(), func_sort);

	if(turn < values.size())
		return values[turn].first;

	return -1;
}

bool check_consistency(Graph const& graph, int node, int value, vector<int> assigned) {
	//checks arc consistency for the neighboring nodes
	for(int i=0; i<graph.adjacency_list[node].size(); i++) {
		int next = graph.adjacency_list[node][i];
		if(assigned[next] == value)
			return false;
	}
	return true;
}

bool infer (Graph const& graph, int node, int value) {
	//takes inference from neighboring nodes
	for (int i=0; i<graph.adjacency_list[node].size(); i++) {
		int next = graph.adjacency_list[node][i];
		color[next][value] = 0;
		if(accumulate(color[next].begin(), color[next].end(), 0) == 0)
			return false;
	}
	return true;
}

void add_inference (Graph const& graph, int node, int value) {
	for (int i=0; i<graph.adjacency_list[node].size(); i++) {
		int next = graph.adjacency_list[node][i];
		color[next][value] = 0;
	}
}

void revert_inference (Graph const& graph, int value, vector<int> temp) {
	for (int i=0; i<NODES; i++) {
		color[i][value] = temp[i];
	}
}

void revert_inference_mac (Graph const& graph, vector<vector<int> > temp) {
	for (int i=0; i<NODES; i++) {
		for (int j=0; j<COLOR; j++) {
			color[i][j] = temp[i][j];
		}
	}
}

bool revise (Graph const& graph, int xi, int xj) {
	//function used in AC3
	bool revised = false;
	bool remove = true;

	for(int i=0; i<COLOR ; i++) {
		remove = true;

		if(color[xi][i] == 1) {
			for (int j=0; j<COLOR; j++) {

				if (color[xj][j] == 1 && i!=j) {
					remove = false;
					break;
				}
			}
			if(remove) {
				color[xi][i] = 0;
				revised = true;
				}
		}
	}
	return revised;
}

bool AC3(Graph const& graph, vector<int> assigned, int node) {
	//Arc Consistency 3 algo
	queue <pair <int, int> > ac3_queue;
	pair <int, int> popped;

	for(int i=0; i<graph.adjacency_list[node].size(); i++) {
		int next = graph.adjacency_list[node][i];
		if(assigned[next] == -1)
			ac3_queue.push(make_pair(next, node));
	}	

	while(!ac3_queue.empty()) {
		popped  = ac3_queue.front();
		ac3_queue.pop();
		int xi = popped.first;
		int xj = popped.second;

		if(revise(graph, xi, xj)) {
			if(accumulate (color[xi].begin(), color[xi].end(), 0) == 0)
				return false;
			for(int i=0; i<graph.adjacency_list[xi].size(); i++) {
				int xk = graph.adjacency_list[xi][i];
				if(assigned[xk] == -1 && xk != xj)
					ac3_queue.push(make_pair(xk, xi));
			}
		}
	}
	return true;
}