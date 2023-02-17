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
                Graph(const vector<vector<int> > &another);

                void printGraph();

                void bfs(int s);
                void bfsTranspose(int s);
                void dfs();
                void dfsVisit(int u, int &t);
                void printParents();
                void printDistance();

                void topSort();//builds topOrder vector<int>
                void topSortVisit(int u, int &t);	
                void printTopOrder();
                void buildTranspose();
                void printTranspose();
                void buildSCC();
                void printSCC();
                int dfsTranspose();
                void dfsVisitTranspose(int u, int &t, int &curID);

				//Test 1: need to write this function (and/or any other functions)
				bool biMutexSink();

        private:
                vector< vector<int> > Adj; //adjacency lists of the graph 
                vector< int > distance; //for BFS
                vector<int> parents; //for BFS and DFS
                vector<char> colors; //for DFS
                vector<TimeStamp> stamps; //for DFS
                int size;//total vertices in the graph

                vector<int> topOrder;
                vector<vector<int> > transpose;
                vector<vector<int> > SCC;//scc graph
                vector<int> SCC_IDs;
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

Graph::Graph(const vector<vector<int> > &another){
		size = another.size();
        Adj.resize(size);
        distance.resize(size);
        parents.resize(size);
        colors.resize(size);
        stamps.resize(size);

		for(int i = 0; i < size; i++){
			Adj[i] = another[i];
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

void Graph::bfsTranspose(int s){
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

                for(int i = 0; i < (int)transpose[u].size(); i++){
                        int v = transpose[u][i];
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

int Graph::dfsTranspose(){
        for(int i = 0; i < size; i++){
                parents[i] = i;
                colors[i] = 'W';
        }
        int t = 0;
        int curID = 0;
        for(int i = 0; i < size; i++){
                int ind = topOrder[i];
                if(colors[ind] == 'W'){
                        dfsVisitTranspose(ind, t, curID);
                        curID++;
                }//if
        }//for
        return curID;
}//dfs
void Graph::dfsVisitTranspose(int u, int &t, int &curID){
        colors[u] = 'G';
        stamps[u].d = t;
        t++;
        SCC_IDs[u] = curID;
        for(int i = 0; i < (int)transpose[u].size(); i++){
                int v = transpose[u][i];
                if(colors[v] == 'W'){
                        parents[v] = u;
                        colors[v] = 'G';
                        dfsVisitTranspose(v, t, curID);
                }
        }//for
        colors[u] = 'B';
        stamps[u].f = t;
        t++;
}//dfsVisit 

void Graph::buildSCC(){
        topSort();
        buildTranspose();

        SCC.resize(size);
        for(int i = 0; i < size; i++){
                SCC[i].clear();
                SCC[i].resize(0);
        }

        SCC_IDs.resize(size);

        int totalSCC = dfsTranspose();
        SCC.resize(totalSCC);
        for(int u = 0; u < size; u++){
                for(int j = 0; j < (int)Adj[u].size(); j++){
                        int v = Adj[u][j];
                        //we have edge (u, v); find SCCs to which u and v belong
                        int sccU = SCC_IDs[u];
                        int sccV = SCC_IDs[v];
                        if(sccU != sccV){
                                SCC[sccU].push_back(sccV);
                        }
                }
        }
}

void Graph::printSCC(){
        for(int u = 0; u < (int)SCC.size(); u++){
                cout << u << ": ";
                for(int j = 0; j < (int)SCC[u].size(); j++){
                        int v = SCC[u][j]; //v is a neighbor
                        cout << v << " ";
                }
                cout << endl;
        }

}


void Graph::topSort(){
        for(int i = 0; i < size; i++){
                parents[i] = i;
                colors[i] = 'W';
        }
        int t = 0;
        topOrder.clear();
        topOrder.resize(0);
        for(int i = 0; i < size; i++){
                if(colors[i] == 'W'){
                        topSortVisit(i, t);
                }//if
        }//for
        std::reverse(topOrder.begin(), topOrder.end());
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
                        topSortVisit(v, t);
                }
        }//for
        colors[u] = 'B';
        stamps[u].f = t;
        t++;
        topOrder.push_back(u);
}//dfsVisit 

void Graph::buildTranspose(){
        transpose.resize(size);
        for(int i = 0; i < size; i++){
                transpose[i].clear();
                transpose[i].resize(0);
        }//for

        for(int u = 0; u < size; u++){
                for(int j = 0; j < (int)Adj[u].size(); j++){
                        int v = Adj[u][j];//neighbor
                        transpose[v].push_back(u);
                }//for j
        }//for u
}
void Graph::printTranspose(){
        for(int u = 0; u < size; u++){
                cout << u << ": ";
                for(int j = 0; j < (int)transpose[u].size(); j++){
                        cout << transpose[u][j] << " ";
                }
                cout << endl;
        }
}

void Graph::printTopOrder(){
        for(int i = 0; i < size; i++)
                cout << topOrder[i] << " ";
        cout << endl;
}

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

/******************************Solution ***********/

bool Graph::biMutexSink(){
        int t1 = -1, t2 = -1, counter = 0;
        for(int i = 0; i < (int)Adj.size(); i++){
                int out = Adj[i].size();
                if(out == 0 && t1 == -1){
                        t1 = i;
                        counter++;

                }else if(out == 0 && t2 == -1){
                        t2 = i;
                        counter++;
                }else if(out == 0){
                        counter++;
                }
        }
        if(counter != 2){
                return false;
        }
        buildTranspose();
        bfs(t1);
        bfsTranspose(t1);
        vector<int> temp = distance;
        bfs(t2);
        bfsTranspose(t2);
        //cout << "t1: " << t1 << " t2: " << t2 << endl;
        for(int i = 0; i < size; i++){
                if(distance[i] < INT_MAX) {
                        if(temp[i] < INT_MAX){
                                return false;
                        }
                }
                else if(distance[i] == INT_MAX) {
                        if(temp[i] == INT_MAX){
                                return false;
                        }
                }
        }
        return true;

}


/************* End of Directed Graph **************/


int main(){

        Graph ag;//directed graph

        string command;
        while(cin >> command){
				if(command == "printGraph"){
                        cout << "Adjacency lists of the graph are:" << endl;
                        ag.printGraph();
                }
                else if(command == "biMutexSink"){
                        cout << "Running biMutexSink." << endl;
                        bool res = ag.biMutexSink();
                        if(res)
							cout << "The graph is bi-Mutex-Sink." << endl;
						else
							cout << "The graph is NOT bi-Mutex-Sink." << endl;
                 }                         
                else{
                        cerr << "ERROR: the command " << command << " is not found." << endl;
                }
        }//while
        return 0;
}
