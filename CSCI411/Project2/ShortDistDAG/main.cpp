#include<climits>
#include<algorithm>
#include<queue>
#include<math.h>
#include<vector>
#include<iostream>
#include<string>
#include<utility>
using namespace std;

#ifndef TIMESTAMP_H
#define TIMESTAMP_H
class TimeStamp{
public:
	TimeStamp(): d(0), f(0){};
	int d; //discovery time
	int f; //finish time

};

#endif

/********** Directed Graph ************/

#ifndef GRAPH_H
#define GRAPH_H

struct edge{
	int source;//edge is from this node to neighbor
	int neighbor; // adjacent node
	int w; //keeps auxiliary information
	edge(){
		source = 0;
		neighbor = 0;
		w = 0;
	};
	edge(int i, int j){
		source = i;//dummy value
		neighbor = j;
		w = 0;
	};
	edge(int from, int to, int aweight){
		source = from;//edge is from this node
		neighbor = to;//edge is to this node
		w = aweight;//weight of this edge
	};
};

class Graph{
public:
	Graph();

    void printGraph();

	//functions:
	void topSort();
	void topSort_dfsVisit(int u, int &t);
	void printTopOrder();

	//need to write this function
	void findShortPaths(int source);

private:
	vector< vector<edge> > Adj; //adjacency lists of the graph 
	vector< int > distance; //for BFS
	vector<int> parents; //for BFS and DFS
	vector<char> colors; //for DFS
	vector<TimeStamp> stamps; //for DFS
    int size;//total vertices in the graph
	vector<int> topOrder;
};

#endif

Graph::Graph(){
	cin >> size;
	int m;
	cin >> m;
	int u, v, w;
	Adj.resize(size);
	distance.resize(size);
	parents.resize(size);
	colors.resize(size);
	stamps.resize(size);

	for(int i = 0; i < m; i++){
		cin >> u >> v >> w;
		Adj[u].push_back(edge(u, v, w));
	}//for

}//default
void Graph::printGraph(){

        //format: 
        //vertex: list of neighbors
        for(int u = 0; u < size; u++){
            cout << u << ": ";
            for(int j = 0; j < (int)Adj[u].size(); j++){
                int v = Adj[u][j].neighbor; //v is a neighbor
				int w = Adj[u][j].w;
                cout << v << "/" << w << "\t";
            }
            cout << endl;
        }
}

//Topological Sort
void Graph::topSort(){
	if(topOrder.size() > 0){
		topOrder.clear();
		topOrder.resize(0);
	}
	for(int i = 0; i < size; i++){
		parents[i] = i;
		colors[i] = 'W';
	}	
	int t = 0;

	for(int i = 0; i < size; i++){
		if(colors[i] == 'W'){
			topSort_dfsVisit(i, t);
		}//if
	}//for

	//reverse the order
	int halfSize = size/2;
	int last = size -1;
	for(int i = 0; i < halfSize; i++){
		swap(topOrder[i], topOrder[last - i]);
	}
}//dfs

void Graph::topSort_dfsVisit(int u, int &t){
	colors[u] = 'G';
	stamps[u].d = t;
	t++;
	
	for(int i = 0; i < (int)Adj[u].size(); i++){
			int v = Adj[u][i].neighbor;
			if(colors[v] == 'W'){
				parents[v] = u;
				colors[v] = 'G';
				topSort_dfsVisit(v, t);
			}
	}//for
	colors[u] = 'B';
	stamps[u].f = t;
	topOrder.push_back(u);
	t++;
}//topSort_dfsVisit	


void Graph::printTopOrder(){
	for(int i = 0; i < (int)topOrder.size(); i++)
		cout << topOrder[i] << " " ;
	cout << endl;
}//printTopOrder


/************* End of Directed Graph **************/


int main(){

	Graph ag;//directed graph

	string command;
	while(cin >> command){
		if(command == "printGraph"){
			cout << "Graph is:" << endl;
			ag.printGraph();
		}
		else if(command == "findShortPaths"){
			int source;
			cin >> source;
			cout << "Shortest distances from " << source << " to all reachable nodes are:" << endl;
			ag.findShortPaths(source);
		}
		else{
			cerr << "ERROR: the command " << command << " is not found." << endl;
		}
	}//while
  return 0;
}
