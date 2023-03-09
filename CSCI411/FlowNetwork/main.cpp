#include<climits>
#include<algorithm>
#include<queue>
#include<math.h>
#include<vector>
#include<iostream>
#include<string>
#include<utility>
using namespace std;

class Graph {
    public:
        Graph();

        bool bfs(int s, int e);

        void updateFlow();

        void printCapacity();
        void printResidual();
        void printAdj();


    private:
        int size, numOfAgents, numOfBuyers, numOfShips, currAgent, shipsToSell, 
        end, buyersAvaliable, currBuyer, shipsAvaliable, currShip, flow, maxFlow;

        vector<int> distance;
        vector<int> parents;
        vector<int> currentPathC;

        vector<vector<int>> adj;
        vector<vector<int>> capacity;
        vector<vector<int>> residual;
        

};

Graph::Graph() {   
    cin >> numOfAgents >> numOfBuyers >> numOfShips;
    size = 2*numOfAgents + numOfBuyers + numOfShips + 2;
    end = size - 1;
    adj.resize(size);capacity.resize(size); residual.resize(size);currentPathC.resize(size); 
    distance.resize(size); parents.resize(size);
    for(int i = 0; i < size; i++){
        capacity[i].resize(size);
        residual[i].resize(size);
    }
    for(int i = 2*numOfAgents + 1; i <= 2*numOfAgents + numOfBuyers; i++){
        adj[0].push_back(i);
        capacity[0][i] = 1;
    }
    for(int i = 2*numOfAgents + numOfBuyers + 1; i <= 2*numOfAgents + numOfBuyers + numOfShips; i++){
        adj[i].push_back(end);
        capacity[i][end] = 1;
    }
    for(int i = 0; i < numOfAgents; i++){
        cin >> currAgent;
        int agentSecond = currAgent + numOfAgents;
        adj[currAgent].push_back(agentSecond);  
        cin >> shipsToSell;
        capacity[currAgent][agentSecond] = shipsToSell;
        cin >> buyersAvaliable;
        for(int j = 0; j < buyersAvaliable; j++){
            cin >> currBuyer;
            adj[currBuyer].push_back(currAgent);
            capacity[currBuyer][currAgent] = 1;
        }
        cin >> shipsAvaliable;
        for(int j = 0; j < shipsAvaliable; j++){
            cin >> currShip;
            adj[agentSecond].push_back(currShip);
            capacity[agentSecond][currShip] = 1;
        }
    }
    residual = capacity;
    
}

bool Graph::bfs(int start, int end) {
    for(int i = 0; i < size; i++){
        distance[i] = INT_MAX;
        parents[i] = -1;
    }
    distance[start] = 0;
    queue<int> Q;
    Q.push(start);
    while(!Q.empty()){
        int currNode = Q.front();
        Q.pop();
        for(int i = 0; i < (int)adj[currNode].size(); i++){
            int neighbor = adj[currNode][i];
            if(residual[currNode][neighbor] > 0){
                if(distance[neighbor] == INT_MAX){
                    Q.push(neighbor);
                    distance[neighbor] = distance[currNode] + 1;
                    parents[neighbor] = currNode;
                }
                
            }
        }
    }
    if(distance[end] == INT_MAX){
        return false;
    }
    return true;
}

void Graph::updateFlow (){
    maxFlow = numOfBuyers;
    while(bfs(0, end)){
        vector<int> path;
        for(int i = end; i >= 0; i = parents[i]){
            path.push_back(i);
        }
        reverse(path.begin(), path.end());
        int minCapacity = INT_MAX;
        for(int i = 0; i < (int)path.size() - 1; i++){
            if(residual[path[i]][path[i+1]] < minCapacity){
                minCapacity = residual[path[i]][path[i+1]];
            }
        }
        for(int i = 5; i > 0; i--){
            residual[path[i]][path[i-1]] += minCapacity;
            residual[path[i-1]][path[i]] -= minCapacity;
        }
        cout <<"Agent " << path[2] << " sold spaceship " << path[4] << " to buyer " 
        << path[1] << "." << endl;
        flow++;
    } 
    cout << flow << ", " << maxFlow << endl;
    if(flow < maxFlow){

    }
}

void Graph::printAdj(){
    for(int i = 0; i < size; i++){
        cout << i << ": ";
        for(int j = 0; j < (int)adj[i].size(); j++){
            cout << adj[i][j] << " ";
        }
        cout << "\n";
    }
}

void Graph::printResidual() {
    for(int i = 0; i < size; i++){
        cout << i << ": ";
        for(int j = 0; j < (int)residual[i].size(); j++){
            cout << residual[i][j] << " ";
        }
        cout << "\n";
    }
}

void Graph::printCapacity (){
    for(int i = 0; i < size; i++){
        cout << i << ": ";
        for(int j = 0; j < (int)capacity[i].size(); j++){
            cout << capacity[i][j] << " ";
        }
        cout << "\n";
    }
}
int main () {
    Graph G;
    G.updateFlow();
}