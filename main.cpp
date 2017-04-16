#include "backtrack.h"
#include "local_search.h"

vector<int> CSP_SEARCH(Graph const& graph, int choice) {

	//calling function 
	vector<int> assigned(NODES, -1);
	if (choice == 1)
		return backtrack (assigned, graph);

	else if (choice == 2)
		return backtrack_fc (assigned, graph);
    
	else 
		return backtrack_mac (assigned, graph);
}

Graph make_graph () {
	//creating a sample graph based on map of Australia
    vector<Edge> edges;
    edges.push_back(Edge(0,1));
    edges.push_back(Edge(0,2));
    edges.push_back(Edge(0,3));
    edges.push_back(Edge(0,4));
    edges.push_back(Edge(0,5));
    edges.push_back(Edge(1,2));
    edges.push_back(Edge(2,3));
    edges.push_back(Edge(3,4));
    edges.push_back(Edge(4,5));
    return Graph(edges);
}

int main() {
	vector<int> assignment;
    
    cout<<"\n\t\t\t\tCSP PROBLEM\n";
    cout<<"\t\t\t\t***********";

    timestamp_t t0, t1;
    int choice = 0;
    srand (time(NULL));
    Graph graph (make_graph());
    cout << endl;

    cout<< "Graph : \n\n";
    print_graph(graph);

   		//simple backtracking
        t0 = get_timestamp();
        assignment = CSP_SEARCH(graph, 0);
        t1 = get_timestamp();
        cout << endl << "\t \t \t Backtrack Search : " << (t1-t0)/1000.0L << " msec" << endl;  
        for(int i=0;i<assignment.size();i++)
            cout<<assignment[i]<<" ";     
      
      	//backtracking with forward check
        t0 = get_timestamp();
        assignment = CSP_SEARCH(graph, 1);
        t1 = get_timestamp();
        cout << endl <<  "\t \t \t Forward Checking :  "  << (t1-t0)/1000.0L << " msec " << endl;
                    for(int i=0;i<assignment.size();i++)
            cout<<assignment[i]<<" ";    

        //backtrack with mac
        t0 = get_timestamp();
        assignment = CSP_SEARCH(graph, 2);
        t1 = get_timestamp();
        cout << endl <<  "\t \t \t Arc Consistency :  "  << (t1-t0)/1000.0L << " msec " << endl;
             for(int i=0;i<assignment.size();i++)
            cout<<assignment[i]<<" ";    

        //local search
        t0 = get_timestamp();
        assignment = local_search (graph);
        t1 = get_timestamp();
        cout << (t1-t0)/1000.0L << " msec " << endl;
         for(int i=0;i<assignment.size();i++)
            cout<<assignment[i]<<" ";    
    
    	//maximum number of steps allowed in local search = 100
        cout << "\t\n \t \t *If steps==100 then \n\t\t  No solution*" << endl << endl;
        
    return 0;
}