#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

vector<vector<int>> graph;

void parallel_bfs(int start) {
    int num_nodes = graph.size();
    vector<bool> visited(num_nodes, false);
    queue<int> q;

    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int level_size = q.size();
        
        #pragma omp parallel for
        for (int i = 0; i < level_size; ++i) {
            int current_node;
            #pragma omp critical
            {
                current_node = q.front();
                q.pop();
                cout << current_node << " ";
            }

            // Explore neighbors of the current node
            #pragma omp parallel for
            for (int j = 0; j < graph[current_node].size(); ++j) {
                int neighbor = graph[current_node][j];
                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            q.push(neighbor);
                            visited[neighbor] = true;
                        }
                    }
                }
            }
        }
    }
}

void parallel_dfs(int start){
    int n = graph.size();
    vector<bool> visited(n, false);
    stack<int> s;
    s.push(start);
    while(!s.empty()){
    int current_node=s.top();
    s.pop();

    if(!visited[current_node])
    {
        visited[current_node]=true;
        cout<< current_node<< " ";

            #pragma omp parallel for 
            for(int j=0 ; j<graph[current_node].size(); ++j){
                int neighbor = graph[current_node][j];
                if(!visited[neighbor])
                {
                    #pragma omp critical
                    {
                        if(!visited[neighbor])
                        {
                            s.push(neighbor);
                        }        
                    }
                }
            }
    }

}

}
int main() {
    // Initialize the graph (adjacency list representation)
    graph = {
        {1, 2},     // Node 0 is connected to nodes 1 and 2
        {0, 3, 4},  // Node 1 is connected to nodes 0, 3, and 4
        {0, 5},     // Node 2 is connected to nodes 0 and 5
        {1},        // Node 3 is connected to node 1
        {1},        // Node 4 is connected to node 1
        {2}         // Node 5 is connected to node 2
    };
    // Perform DFS traversal starting from node 0
    cout << "DFS traversal: ";
    parallel_dfs(0);
    cout << endl;
    cout << "BFS traversal: ";
    parallel_bfs(0);
    cout << endl;

    return 0;
}