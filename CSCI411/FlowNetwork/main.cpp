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

        void bfs(int s, int e);

        void updateFlow();
        void buildResidual();
        void resetResidual();
        bool createPath();

        void printCapacity();
        void printResidual();
        void printResidualAdj();
        void printAdj();
        void printFlow();
        void printPath();

        int getSize();


    private:
        int size, numOfAgents, numOfBuyers, numOfShips, currAgent, shipsToSell, 
        end, buyersAvaliable, currBuyer, shipsAvaliable, currShip, A, B, S;

        vector<int> distance;
        vector<int> parents;
        vector<int> path;

        vector<vector<int>> adj;
        vector<vector<int>> capacity;
        vector<vector<int>> flow;
        vector<vector<int>> residual;
        vector<vector<int>> residualAdj;
        vector<vector<int>> reset;
        

};

Graph::Graph() {   
    cin >> numOfAgents >> numOfBuyers >> numOfShips;
    size = 2*numOfAgents + numOfBuyers + numOfShips + 2;
    end = size - 1;
    adj.resize(size);flow.resize(size);capacity.resize(size); residual.resize(size);
    distance.resize(size); parents.resize(size); residualAdj.resize(size);reset.resize(size);
    for(int i = 0; i < size; i++){
        reset[i].resize(size);
        fill(reset[i].begin(), reset[i].end(), -1);
        residual[i].resize(size);
        capacity[i].resize(size);
        fill(capacity[i].begin(), capacity[i].end(), -1);
        flow[i].resize(size);
        fill(flow[i].begin(), flow[i].end(), -1);
    }
    for(int i = 2*numOfAgents + 1; i <= 2*numOfAgents + numOfBuyers; i++){
        adj[0].push_back(i);
        flow[0][i] = 0;
        capacity[0][i] = 1;
    }
    for(int i = 2*numOfAgents + numOfBuyers + 1; i <= 2*numOfAgents + numOfBuyers + numOfShips; i++){
        adj[i].push_back(end);
        flow[i][end] = 0;
        capacity[i][end] = 1;
    }
    for(int i = 0; i < numOfAgents; i++){
        cin >> currAgent;
        int agentSecond = currAgent + numOfAgents;
        adj[currAgent].push_back(agentSecond);  
        cin >> shipsToSell;
        flow[currAgent][agentSecond] = 0;
        capacity[currAgent][agentSecond] = shipsToSell;
        cin >> buyersAvaliable;
        for(int j = 0; j < buyersAvaliable; j++){
            cin >> currBuyer;
            adj[currBuyer].push_back(currAgent);
            flow[currBuyer][currAgent] = 0;
            capacity[currBuyer][currAgent] = 1;
        }
        cin >> shipsAvaliable;
        for(int j = 0; j < shipsAvaliable; j++){
            cin >> currShip;
            adj[agentSecond].push_back(currShip);
            flow[agentSecond][currShip] = 0;
            capacity[agentSecond][currShip] = 1;
        }
    }  
}

void Graph::buildResidual(){
    resetResidual();
    for(int i = 0; i < size; i++){
        for(int j = 0; j < (int)adj[i].size(); j++){
            int v = adj[i][j];
            if(flow[i][v] < capacity[i][v]){
                residualAdj[i].push_back(v);
                residualAdj[v].push_back(i);
                residual[i][v] = capacity[i][v] - flow[i][v];
                residual[v][i] = flow[i][v];
            }else if(flow[i][v] == capacity[i][v]){
                residualAdj[v].push_back(i);
                residual[v][i] = capacity[i][v];
                residual[i][v] = 0;
            }
        }
    }
}

void Graph::resetResidual(){
    for(int i = 0; i < size; i++){
        
        residualAdj.clear();
        residualAdj.resize(0);
    }
    residual = reset;
}

bool Graph::createPath(){
    path.clear();  
    if(distance[end] == INT_MAX){
        return false;
    }
    for(int i = end; parents[i] != i; i = parents[i]){
        path.push_back(i);
    } 
    path.push_back(0);
    reverse(path.begin(), path.end());
    // for(int i = 0; i < (int)path.size(); i++){
    //     cout << path[i] << " ";
    // }
    // cout << endl;
    return true;
}

void Graph::bfs(int start, int end) {
    buildResidual();
    for(int i = 0; i < size; i++){
        distance[i] = INT_MAX;
        parents[i] = i;
    }
    distance[start] = 0;
    
    queue<int> Q;
    Q.push(start);

    while(!Q.empty()){
        int currNode = Q.front();
        Q.pop();
        for(int i = 0; i < (int)residualAdj[currNode].size(); i++){
            int neighbor = residualAdj[currNode][i];
            if(residual[currNode][neighbor] != 0 && distance[neighbor] == INT_MAX){
                Q.push(neighbor);
                distance[neighbor] = distance[currNode] + 1;
                parents[neighbor] = currNode;
                
            }
        }
    }
}

void Graph::updateFlow (){
    
    for(int i = 0; i < (int)path.size()-1; i++){
        if(flow[path[i]][path[i+1]] == -1){
            flow[path[i+1]][path[i]] -= 1;
        }else{
            flow[path[i]][path[i+1]] += 1;
        }
    }
} 

void Graph::printPath(){
    cout << "Agent " << path[2] << " sold spaceship " << path[4] << " to buyer " << path[1] << ".\n";
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

void Graph::printResidualAdj() {
    for(int i = 0; i < size; i++){
        cout << i << ": ";
        for(int j = 0; j < (int)residualAdj[i].size(); j++){
            cout << residualAdj[i][j] << " ";
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

int Graph::getSize(){
    return end;
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

void Graph::printFlow(){
    for(int i = 0; i < size; i++){
        cout << i << ": ";
        for(int j = 0; j < (int)flow[i].size(); j++){
            cout << flow[i][j] << " ";
        }
        cout << "\n";
    }
}
int main () {
    Graph G;
    bool isPath;
    do{
        G.bfs(0, G.getSize());
        
        isPath = G.createPath();
        
        // G.printFlow();
        
        // G.printFlow();
        if(isPath) {
            G.updateFlow();
            G.printPath();
        }
        
    }while(isPath);
}