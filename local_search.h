#include "header.h"

bool is_answer (Graph const& graph, vector<int> current) {
	//to check if the color assigned is valid or not
	for(int i=0; i<NODES; i++) {
		for(int j=0; j<graph.adjacency_list[i].size(); j++) {
			int neighbour = graph.adjacency_list[i][j];
			if(current[neighbour] == current[i])
				return false;
		}
	}
	return true;
}

int find_conflicted_var (Graph const& graph, vector<int> current, int random) {
	//finding a random conflicted variable
	vector<int> conflicted;

	for(int i=0; i<NODES; i++) {
		for(int j=0; j<graph.adjacency_list[i].size(); j++) {
			int neighbour = graph.adjacency_list[i][j];
			if(current[neighbour] == current[i]) {
				conflicted.push_back(i);
				break;
			}
		}
	}
	random = random % (conflicted.size());
	return conflicted[random];
}

int conflict (Graph const& graph, vector<int> current, int node, int value) {
	//to return the number of conflicts
	current[node] = value;
	int conflicts = 0;

	for(int i=0; i<graph.adjacency_list[node].size(); i++) {
		int neighbour = graph.adjacency_list[node][i];
		if(current[neighbour] == current[node])
			conflicts++;
	}
	return conflicts;
}


vector<int> local_search(Graph graph) {
	//minimum conflict local search algo
	int max_steps = MAX;
	//max steps can be 100
	vector<int> current (NODES);
	int temp = 0;
	int node = 0;
	int value = 0;
	int min_value = INT_MAX;
	int number_conflicts = 0;

	//assigning a random assignment to the nodes
	for(int i=0; i<NODES; i++) 
		current[i] = (rand() % 100) % COLOR;

	//checking if assignment is valid
	for(int a=0; a<max_steps; a++) {
		if (is_answer(graph, current)) {
			cout << endl <<  "\t \t \t Local Search :  "  << a << " Steps  :  " ;
			return current;
		}
			
		temp = rand() % 100000;
		//finding a random variable 
		node = find_conflicted_var(graph, current, temp);

		//assiging the min conflict color to the node
		for(int j=0; j<COLOR; j++) {
			if(j!= current[node]) {
				number_conflicts = conflict (graph, current, node, j);
				
				if(number_conflicts < min_value) {
					min_value = number_conflicts;
					value = j;
				}
			}
		}
		current[node] = value;
		min_value = INT_MAX;
		value = 0;
	}
	cout << endl <<  "\t \t \t Local Search : "  << max_steps << " Steps  : " ;
	
	return current;
}
