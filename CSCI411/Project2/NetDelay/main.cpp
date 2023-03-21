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

#ifndef UGRAPH_H
#define UGRAPH_H

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



class Ugraph{
public:
	Ugraph(int n);
	int getSize(){ return size; };

	void addNode();//can only add the node with the next index = size
	void addEdge(int u, int v, int aweight);

	void bfs(int s);
	void dfs();
	void dfsVisit(int u, int &t);

	void printGraph();
	void printGraphW();//prints graph with weights
	void printParents();
	void printTime();

	int findNetDelay();

private:
	vector< vector<edge> > Adj; //adjacency lists of the graph 
	vector< int > distance; //for BFS
	vector<int> parents; //for BFS and DFS
	vector<char> colors; //for DFS
	vector<TimeStamp> stamps; //for DFS
    int size;
};

#endif

bool lessThan(const edge &x, const edge &y){ return (x.w < y.w) ? true : false; }

Ugraph::Ugraph(int n){
	size = n;
	
	Adj.resize(size);
	distance.resize(size);
	parents.resize(size);
	colors.resize(size);
	stamps.resize(size);

	for(int i = 0; i < size; i++){
		distance[i] = INT_MAX;
		parents[i] = i;
		colors[i] = 'W';
	}//for
}//default

void Ugraph::printGraph(){
	for(int u = 0; u < size; u++){
		int uSize = (int)Adj[u].size();
		for(int i = 0; i < uSize; i++)
			cout << Adj[u][i].neighbor << " " ;
		cout << endl;
	}
}
void Ugraph::printGraphW(){
	for(int u = 0; u < size; u++){
		int uSize = (int)Adj[u].size();
		cout << u << ": ";
		for(int i = 0; i < uSize; i++)
			cout << Adj[u][i].neighbor << "/" << Adj[u][i].w << "\t" ;
		cout << endl;
	}
}
void Ugraph::addNode(){
	size++;
	Adj.resize(size);
	distance.resize(size);
	parents.resize(size);
	colors.resize(size);
	stamps.resize(size);
	
}

void Ugraph::addEdge(int u, int v, int aweight){
	edge one(u, v, aweight);
	edge two(v, u, aweight);
	Adj[u].push_back(one);
	Adj[v].push_back(two);
}

void Ugraph::bfs(int s){
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
       // cout << u << " ";
		for(int i = 0; i < (int)Adj[u].size(); i++){
			int v = Adj[u][i].neighbor;
			if(distance[v] == INT_MAX){
				distance[v] = distance[u] + 1;
				parents[v] = u;
				aq.push(v);
			}
		}//for
	}//while
	//cout << endl;
}//bfs

void Ugraph::dfs(){
	for(int i = 0; i < size; i++){
		parents[i] = i;
		colors[i] = 'W';
	}	
	int t = 0;

	for(int i = 0; i < size; i++){
		if(colors[i] == 'W'){
			//color[i] = 'G';
			dfsVisit(i, t);
		}//if
	}//for
	//cout << endl;
}//dfs

void Ugraph::dfsVisit(int u, int &t){
	colors[u] = 'G';
	stamps[u].d = t;
	t++;
	//cout << u << " ";
	for(int i = 0; i < (int)Adj[u].size(); i++){
			int v = Adj[u][i].neighbor;
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


int Ugraph::findNetDelay(){
	vector<vector<int>> D_Max(size, vector<int> (2, 0));
	vector<vector<int>> Backtrack(size, vector<int> (2, 0));
	for(int i = 0; i < size; i++){
		Backtrack[i][0] = i;
		Backtrack[i][1] = i;
	}
	dfs();
	int child = INT_MAX;
	for(int i = 1; i < size; i++){
		if((int)Adj[i].size() == 1){
			if(i < child){
				child = i;
			}
			
		}
	}
	int parent = parents[child];
	
	while(child != 0){
		printf("P:%d, C:%d", parent, child);
		for(int i = 1; i < (int)Adj[parent].size();i++){
			child = Adj[parent][i]
			if()
		}
	}
	
}

void Ugraph::printParents(){
	for(int i = 0; i < size; i++){
		cout << parents[i] << " ";
	}
	cout << "\n";
}

void Ugraph::printTime(){
	for(int i = 0; i < size; i++){
		cout << i << ": " << "(" << stamps[i].d << ", " << stamps[i].f << ") ";
	}
	cout << "\n";
}

int main(){

	int N;
	cin >> N;
	Ugraph ag(N);

	string command;
	while(cin >> command){
		if(command == "addEdge"){
			int u, v, w;
			cin >> u >> v >> w;
			ag.addEdge(u, v, w);
		}else if(command == "printGraph"){
			cout << "Graph is:" << endl;
			ag.printGraph();
		}else if(command == "printGraphW"){
			cout << "Graph is:" << endl;
			ag.printGraphW();
		}else if(command == "findNetDelay"){
		cout << "\nThe longest path is:" << endl;
			int length = ag.findNetDelay();
			cout << "The length of the longest path is " << length << "." << endl;
		}else{
			cerr << "ERROR: the command " << command << " is not found." << endl;
		}
	}//while
  return 0;
}

