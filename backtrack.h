#include "header.h"
#pragma once
//pragma is used to avoid ambiguity

vector<int> backtrack (vector <int> assigned, Graph const& graph) {
	//simple backtrack algo
	vector<int> ans;
	bool assign = false;
	bool return_ans = true;

	for(int i=0; i<NODES; i++) {
		if (assigned[i] == -1) {
			return_ans = false;
			break;
		}	
	}

	if(return_ans)
		return assigned;
	
	int node = max_cvar (graph, assigned);

	for(int i=0; i<COLOR; i++){
		int value = min_cval (graph, node, i, assigned);
		if(value == -1) {
			ans.push_back(-1);
			return ans;
		}		

		if(check_consistency(graph, node, value, assigned)) {
			assigned[node] = value;
			assign = true;

			vector<int>result = backtrack(assigned, graph);
			if(result.size() != 1)
				return result;
		}
		if(assign)
			assigned[node] = -1;
	}
	ans.push_back(-1);
	return ans;
}

vector<int> backtrack_fc (vector <int> assigned, Graph const& graph) {
	//forward checking
	vector <int> temp;
	vector<int> ans;
	bool assign = false;
	bool add_infer = false;
	bool return_ans = true;
	temp.clear();

	for(int i=0; i<NODES; i++) {
		if (assigned[i] == -1) {
			return_ans = false;
			break;
		}	
	}
	if(return_ans)
		return assigned;
	
	int node = max_cvar (graph, assigned);

	for(int i=0; i<COLOR; i++){
		int value = min_cval (graph, node, i, assigned);
		if(value == -1) {
			ans.push_back(-1);
			return ans;
		}		

		if(check_consistency(graph, node, value, assigned)) {
			assigned[node] = value;
			assign = true;

			bool inference = infer(graph, node, value);
			
			if(inference) {
				for(int j=0; j<NODES; j++)
					temp.push_back(color[j][value]);

				add_inference(graph, node, value);
				add_infer = true;
				vector<int>result = backtrack_fc(assigned, graph);

				if(result.size() != 1)
					return result;
			}
		}
		if(assign)
			assigned[node] = -1;
		if (add_infer)
			revert_inference(graph, value, temp);
	}
	ans.push_back(-1);
	return ans;
}

vector<int> backtrack_mac (vector <int> assigned, Graph const& graph) {
	//arc consistency
	vector <vector <int> > temp (NODES, vector<int>(COLOR));
	vector <int> temp2 (COLOR);
	vector<int> ans;
	bool assign = false;
	bool add_infer = false;
	bool return_ans = true;
	temp.clear();

	for(int i=0; i<NODES; i++) {
		if (assigned[i] == -1) {
			return_ans = false;
			break;
		}	
	}
	if(return_ans)
		return assigned;

	int node = max_cvar (graph, assigned);

	for(int i=0; i<COLOR; i++){
		int value = min_cval (graph, node, i, assigned);
		if(value == -1) {
			ans.push_back(-1);
			return ans;
		}		

		if(check_consistency(graph, node, value, assigned)) {
			assigned[node] = value;
			assign = true;

			bool inference = AC3(graph, assigned, node);
			
			if(inference) {
				for(int j=0; j<NODES; j++) {
					for (int k=0; k<COLOR; k++)
						temp2.push_back(color[j][k]);
					temp.push_back(temp2);
				}
				add_infer = true;
				vector<int>result = backtrack_mac(assigned, graph);

				if(result.size() != 1)
					return result;
			}
		}
		if(assign)
			assigned[node] = -1;
		if (add_infer)
			revert_inference_mac(graph, temp);
	}
	ans.push_back(-1);
	return ans;
}
