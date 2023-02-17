#include<climits>
#include<algorithm>
#include<queue>
#include<math.h>
#include<vector>
#include<iostream>
#include<iomanip>
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


                void printGraph();
                void printGraphWeight();
                void printGraphResidual();
                void printGraphFlow();

                bool bfs(int s, int t);
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
                void flowNetwork();

        private:
                vector< vector<int> > Adj; 
                vector< vector<int> > AdjWeight;
                vector< vector<int> > AdjFlow;
                vector< vector<int> > Residual;
                vector< int > distance; //for BFS
                vector<int> parents; //for BFS and DFS
                vector<char> colors; //for DFS
                vector<TimeStamp> stamps; //for DFS
                int size;//total vertices in the graph
                int s,t;
                vector<int> topOrder;
                vector<vector<int> > transpose;
                vector<vector<int> > SCC;//scc graph
                vector<int> SCC_IDs;
};

#endif
Graph::Graph(){
        int K, N, M, A, B, b, D, d; 
        s = 0;
        cin >> K >> N >> M;
        size = 2*K + N + M + 2;

        Adj.resize(size); AdjWeight.resize(size); AdjFlow.resize(size); Residual.resize(size);
        for(int i = 0; i < size; i++){
                AdjFlow[i].resize(size);
                AdjWeight[i].resize(size);
                Residual[i].resize(size);
        }
        distance.resize(size); parents.resize(size); colors.resize(size); stamps.resize(size);
        t = size - 1;
        for(int i = 0; i < K; i++){
                cin >> A;
                cin >> AdjWeight[A][A + K];
                cin >> B;
                for(int j = 0; j < B; j++){
                        cin >> b;
                        AdjWeight[b][A] = 1;
                        AdjWeight[s][b] = 1;
                }
                cin >> D;
                for(int j = 0; j < D; j++){
                        cin >> d;
                        AdjWeight[A + K][d] = 1;
                        AdjWeight[d][t] = 1;
                }
                

                for(int j = 0; j < size; j++){
                        distance[i] = INT_MAX;
                        parents[i] = i;
                        colors[i] = 'W';

                }
        }
        for(int i = 0; i < size; i++){
                for(int j = 0; j < size; j++){
                        if(AdjWeight[i][j] > 0){
                                if(AdjFlow[i][j] < AdjWeight[i][j]){
                                        Residual[i][j] = AdjWeight[i][j] - AdjFlow[i][j];
                                        Residual[j][i] = AdjFlow[i][j];
                                }else if(AdjFlow[i][j] == AdjWeight[i][j]){
                                        Residual[j][i] = AdjFlow[i][j];
                                }

                        }
                }
        }      
}//default


bool Graph::bfs(int s, int t){
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

                for(int i = 0; i < (int)Residual[u].size(); i++){
                        if(AdjWeight[u][i] > 0){
                                if(Residual[u][i] > 0){
                                        if(distance[i] == INT_MAX){
                                                distance[i] = distance[u] + 1;
                                                parents[i] = u;
                                                aq.push(i);
                                        }
                                }
                                
                        }
                }//for
        }//while
        if(distance[t] == INT_MAX){
                return false;
        }else{
                return true;
        }

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

void Graph::printGraphWeight(){

        for(int u = 0; u < size; u++){
                cout << u << ": ";
                for(int j = 0; j < (int)AdjWeight[u].size(); j++){
                        int v = AdjWeight[u][j]; //v is a neighbor
                        cout << v << " ";
                }
                cout << endl;
        }
}

void Graph::printGraphResidual(){

        for(int u = 0; u < size; u++){
                cout << u << ": ";
                for(int j = 0; j < (int)Residual[u].size(); j++){
                        int v = Residual[u][j]; //v is a neighbor
                        cout << v << " ";
                }
                cout << endl;
        }
}

void Graph::printGraphFlow(){

        for(int u = 0; u < size; u++){
                cout << u << ": ";
                for(int j = 0; j < (int)AdjFlow[u].size(); j++){
                        int v = AdjFlow[u][j]; //v is a neighbor
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

void Graph::flowNetwork(){
        for(int i = 0; i < size; i++){
                for(int j = 0; j < size; j++){
                        AdjFlow[i][j] = 0;
                }
        }
        while(bfs(s,t)){
                vector<int> path;
                path.push_back(t);
                for(int i = t; parents[i] != s; i = parents[i]){
                        path.push_back(parents[i]);
                }
                path.push_back(s);
                reverse(path.begin(), path.end());
                int minCap = INT_MAX;
                for(int i = 0; i < (int)path.size()-1; i++){
                        if(AdjWeight[path[i]][path[i +1 ]] < minCap){
                                minCap = AdjWeight[path[i]][path[i+1]];
                        }
                }
                for(int i = 0; i < (int)path.size()-1; i++){
                        AdjFlow[path[i]][path[i + 1]] += minCap;
                        if(AdjFlow[path[i]][path[i + 1]] < AdjWeight[path[i]][path[i+1]]){
                                Residual[path[i]][path[i + 1]] -= minCap;
                                Residual[path[i + 1]][path[i]] += minCap;
                        }else if(AdjFlow[path[i]][path[i + 1]] == AdjWeight[path[i]][path[i+1]]){
                                Residual[path[i]][path[i + 1]] = 0;
                                Residual[path[i + 1]][path[i]] = AdjWeight[path[i]][path[i+1]];
                        }
                }
                cout << "Agent " << path[2] << " sold spaceship " << path[4] << " to buyer " << path[1] << ".\n";
        }
        
        
        
        
        
}

int main(){

        Graph G;//directed graph
        G.flowNetwork();
        return 0;
}