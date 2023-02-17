#include<climits>
#include<iostream>
#include<string>
#include<vector>
#include<queue>
using namespace std;

class Graph{
public:
	Graph();//initializes Adjacency lists
	void bfs(int s);
	void findDiameter();
	void findDiameterHelper();

private:
	int size;
	vector< vector<int> >  Adj;
	vector<int> parents;
	vector<int> distance;
};

Graph::Graph(){
	int total = 0;
	cin >> total;
	if(total == 0){
		cerr << "ERROR: graph is empty." << endl;
	    return;
    }
	size = total ; 
	Adj.resize(size);
	parents.resize(size);
	distance.resize(size);

	int cur_vertex = 0;
	int child = 0;

	while( cin >> cur_vertex ){
		cin >> child;
		Adj[cur_vertex].push_back(child);
		Adj[child].push_back(cur_vertex);

	}//while
}//constructor

void Graph::bfs(int s){
	for(int i = 0; i < size; i++){
		parents[i] = i;
		distance[i] = INT_MAX;
	}
	distance[s] = 0;
	queue<int> Q;
	Q.push(s);
	
	while( !Q.empty() ){
		int current = Q.front();
		Q.pop();
		int adj_size = (int)Adj[current].size();
		for(int i = 0; i < adj_size; i++){
            int child = Adj[current].at(i);
			if(distance[child] == INT_MAX){
				Q.push(child);
				distance[child] = distance[current] + 1;
				parents[child] = current;
			}//if
           
		}//for
		
	}//while

	//distance contains the shortest distances in the number of edges from s to each vertex in the graph

}//bfs

void Graph::findDiameter(){
	//Use BFS on u.
	//Find longest path, node v.
	//Use BFS on l

}
void Graph::findDiameterHelper(){


}

int main(){
	Graph agraph;//reads graph from an input file and initializes Adjacency lists Adj

	//ADD YOUR CODE below:
	//you may add any functions to class Graph
	
	return 0;
}//main()
