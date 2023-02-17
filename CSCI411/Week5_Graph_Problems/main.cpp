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

class Graph{
public:
	Graph();
	Graph(const Graph &ag);

    void printGraph();

	void bfs(int s);
	void dfs();
	void dfsVisit(int u, int &t);
	void printParents();
	void printDistance();

	//need to write these functions:

	bool isFlower();
	void topSort();//builds topOrder vector<int>
	void topSortVisit(int u, int &t);
	void printTopOrder();

	void buildTranspose();
	void printTranspose();

	void buildSCC();
	void dfsSCCVisit(int u, int &t, int ID);
	void printSCC();

private:
	vector< vector<int> > Adj; //adjacency lists of the graph 
	vector< int > distance; //for BFS
	vector<int> parents; //for BFS and DFS
	vector<char> colors; //for DFS
	vector<TimeStamp> stamps; //for DFS
    int size;//total vertices in the graph
	vector<int> topOrder;
	vector<vector<int> > transpose;
	vector<int> cc;//stores IDs of SCCs for each vertex
	vector<vector<int> > scc;//strongly connected components of the original graph
	int sccSize;//size of scc graph

};

#endif

Graph::Graph(){
	cin >> size;
	int m;
	cin >> m;
	int u, v;
	Adj.resize(size);
	distance.resize(size);
	parents.resize(size);
	colors.resize(size);
	stamps.resize(size);

	for(int i = 0; i < m; i++){
		cin >> u >> v;
		Adj[u].push_back(v);
	}//for

	for(int i = 0; i < size; i++){
		distance[i] = INT_MAX;
		parents[i] = i;
		colors[i] = 'W';
		
	}
}//default

bool Graph::isFlower(){
	buildTranspose();
	int s = 0;
	int count = 0;
	int total = 0;
	for(int i = 0; i < size; i++){
		if(Adj[i].size() > 1 && transpose[i].size() > 1){
			if(Adj[i].size() == transpose[i].size()){
				s=i;
			}
			count++;
		}else if(Adj[i].size() > 1 || transpose[i].size() > 1){
			count++;
		}else if(Adj[i].size() != 1 || transpose[i].size() != 1){
			return false;
		}else if(Adj[i].size() == 1 && transpose[i].size() == 1){
			total++;
		}
		if (count > 1){
			return false;
		}
		if (total == size){
			s = 0;
		}
		if(total > size - 1){
			return false;
		}
		bfs(s);
		for(int i = 0; i < size; i++){
			if (distance[i] == INT_MAX){
				return false;
			}
		}
		return true;
	}
}

void Graph::bfs(int s){
	for(int i = 0; i < size; i++){
		distance[i] = INT_MAX;
		parents[i] = i;
	}//for
	distance[s] = 0;
	queue<int> aq;
	aq.push(s);
	while(!aq.empty()){
		int u = aq.front();
        aq.pop();
        
		for(int i = 0; i < (int)Adj[u].size(); i++){
			int v = Adj[u][i];
			if(distance[v] == INT_MAX){
				distance[v] = distance[u] + 1;
				parents[v] = u;
				aq.push(v);
			}
		}//for
	}//while
	
}//bfs

void Graph::dfs(){
	for(int i = 0; i < size; i++){
		parents[i] = i;
		colors[i] = 'W';
	}	
	int t = 0;

	for(int i = 0; i < size; i++){
		if(colors[i] == 'W'){
			dfsVisit(i, t);
		}//if
	}//for

}//dfs

void Graph::dfsVisit(int u, int &t){
	colors[u] = 'G';
	stamps[u].d = t;
	t++;
	
	for(int i = 0; i < (int)Adj[u].size(); i++){
			int v = Adj[u][i];
			if(colors[v] == 'W'){
				parents[v] = u;
				colors[v] = 'G';
				dfsVisit(v, t);
			}
	}//for
	colors[u] = 'B';
	stamps[u].f = t;
	t++;
}//dfsVisit	

void Graph::printGraph(){

        //format: 
        //vertex: list of neighbors
        for(int u = 0; u < size; u++){
            cout << u << ": ";
            for(int j = 0; j < (int)Adj[u].size(); j++){
                int v = Adj[u][j]; //v is a neighbor
                cout << v << " ";
            }
            cout << endl;
        }
}

void Graph::printParents(){
	for(int i = 0; i < size; i++){
		cout << parents[i] << " " ;
	}
	cout << endl;
}

void Graph::printDistance(){
	for(int i = 0; i < size; i++){
		cout << distance[i] << " " ;
	}
	cout << endl;
}

void Graph::topSort(){
	for(int i = 0; i < size; i++){
		parents[i] = i;
		colors[i] = 'W';
	}	
	int t = 0;

	//the main loop of DFS
	for(int i = 0; i < size; i++){
		if(colors[i] == 'W'){
			topSortVisit(i, t);//rename dfsVisit
		}//if
	}//for

	std::reverse(topOrder.begin(), topOrder.end());//reverse topOrder

}//dfs

void Graph::topSortVisit(int u, int &t){
	colors[u] = 'G';
	stamps[u].d = t;
	t++;
	
	for(int i = 0; i < (int)Adj[u].size(); i++){
			int v = Adj[u][i];
			if(colors[v] == 'W'){
				parents[v] = u;
				colors[v] = 'G';
				topSortVisit(v, t);//rename dfsVisit with topSortVisit
			}
	}//for
	colors[u] = 'B';
	stamps[u].f = t;
	t++;
	topOrder.push_back(u);
}//topSortVisit

void Graph::printTopOrder(){
	for(int i = 0; i < size; i++)
		cout << topOrder[i] << " ";
	cout << endl;
}

void Graph::buildTranspose(){
	transpose.resize(size);

	for(int u = 0; u < size; u++){
		for(int j = 0; j < (int)Adj[u].size(); j++){
			int neighbor = Adj[u][j];//edge goes from u to "neighbor"
			transpose[neighbor].push_back(u);
		}
	}
}

void Graph::printTranspose(){
        //format: 
        //vertex: list of neighbors
        for(int u = 0; u < size; u++){
            cout << u << ": ";
            for(int j = 0; j < (int)transpose[u].size(); j++){
                int v = transpose[u][j]; //v is a neighbor
                cout << v << " ";
            }
            cout << endl;
        }
}

void Graph::buildSCC(){//kind of dfs
	topSort();//topOrder will have vertices sorted 
	buildTranspose();
	cc.resize(size);//so that we could access cc[u]
	for(int i = 0; i < size; i++){
		parents[i] = i;
		colors[i] = 'W';
	}
	int ID = 0;//current ID of an SCC
	int time = 0;
	for(int i = 0; i < size; i++){ //main for loop
		if(colors[topOrder[i]] == 'W'){
			dfsSCCVisit(topOrder[i], time, ID);
			ID++;
		}
	}//for
	//ID has the total number of SCCs in G
	sccSize = ID;
	scc.resize(sccSize);//SCC graph whose vertices correspond to SCCs of G
	//for each edge (u, v) of G, we need to add edge (cc[u], cc[v])
	//if cc[u] not equal to cc[v]
	for(int u = 0; u < size; u++){
		for(int j = 0; j < (int)Adj[u].size(); j++){
			int v = Adj[u][j];
			if(cc[u] != cc[v]){
				scc[ cc[u] ].push_back( cc[v] );//add edge (cc[u], cc[v])
			}
		}
	}
}

void Graph::dfsSCCVisit(int u, int &t, int ID){
	colors[u] = 'G';
	stamps[u].d = t;
	t++;
	cc[u] = ID;//SCC to which u belongs
	for(int i = 0; i < (int)transpose[u].size(); i++){
			int v = transpose[u][i];
			if(colors[v] == 'W'){
				parents[v] = u;
				colors[v] = 'G';
				dfsSCCVisit(v, t, ID);
			}
	}//for
	colors[u] = 'B';
	stamps[u].f = t;
	t++;
}//dfsSCCVisit	

void Graph::printSCC(){


        //format: 
        //vertex: list of neighbors
        for(int u = 0; u < sccSize; u++){
            cout << u << ": ";
            for(int j = 0; j < (int)scc[u].size(); j++){
                int v = scc[u][j]; //v is a neighbor
                cout << v << " ";
            }
            cout << endl;
        }

}
/************* End of Directed Graph **************/


int main(){

	Graph ag;//directed graph

	string command;
	while(cin >> command){
		if(command == "bfs"){
			int s;
			cin >> s;
			cout << "Running BFS on " << s << endl;
			ag.bfs(s);
			cout << "Parents array: " << endl;
			ag.printParents();
			cout << "Distance array: " << endl;
			ag.printDistance();
		}else if(command == "dfs"){
			cout << "Running DFS" << endl;
			ag.dfs();
			cout << "Parents array: " << endl;
			ag.printParents();
		}else if(command == "printGraph"){
			cout << "Adjacency lists of the graph are:" << endl;
			ag.printGraph();
		}
		else if(command == "topSort"){
			cout << "Running topological sort." << endl;
			ag.topSort();
			cout << "Vertices in topological order:" << endl;
			ag.printTopOrder();
		}
		else if(command == "buildTranspose"){
			cout << "Building transpose." << endl;
			ag.buildTranspose();
			cout << "Transpose:" << endl;
			ag.printTranspose();
		}
		else if(command == "buildSCC"){
			cout << "Building SCC graph." << endl;
			ag.buildSCC();
			cout << "SCC graph:" << endl;
			ag.printSCC();
		}else if(command == "isFlower"){//Training 7 beginning
			bool res = ag.isFlower();
			if(res)
				cout << "The graph is a flower-graph." << endl;
			else
				cout << "The graph is NOT a flower-graph." << endl;
		}
		else{
			cerr << "ERROR: the command " << command << " is not found." << endl;
		}
	}//while
  return 0;
}
