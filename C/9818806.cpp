#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>

struct edge {
    int dest;
    int id;
};

struct qe {
    int vert;
    int val;
};

int back(int id) {
    return id % 2 == 0 ? id + 1 : id - 1;
}

void bfs(int start, int s, int n, std::vector<int> &h, std::vector<std::vector<edge>> &network, std::vector<int> &capacity, std::vector<bool> &visited) {
    std::queue<qe> q;
    q.push({start, h[start] - 1});
    visited[start] = true;

    while(!q.empty()) {
        auto [v, val] = q.front();
        q.pop();

        h[v] = val + 1;

        for(auto u : network[v]) {
            if(!visited[u.dest] && u.dest != s && capacity[back(u.id)]) {
                visited[u.dest] = true;
                q.push({u.dest, h[v]});       //tu moze byc zle
            }
        }
    }
}

void global_relabel(int s, int t, int n, std::vector<int> &h, std::vector<std::vector<edge>> &network, std::vector<int> &capacity) {
    std::vector<bool> visited(n, false);
    for(auto &i : h) i = 2 * n + 1;
    h[t] = 0;
    h[s] = n;
    bfs(t, s, n, h, network, capacity, visited);
    bfs(s, s, n, h, network, capacity, visited);
}

void push(int v, edge e, int s, int t, std::vector<std::vector<edge>> &network, std::vector<int> &capacity, std::vector<long long> &preflow, std::vector<int> &last, std::queue<int>&q) {     //ten bool mowi mi czy wyczerpalismy to czy nie
    //musze pamietac i o kraw w jedna i w druga strone
    long long flow = std::min(preflow[v], (long long)capacity[e.id]);
    if(flow > 0 && preflow[e.dest] == 0 && e.dest != s && e.dest != t) q.push(e.dest); 

    preflow[v] -= flow;
    preflow[e.dest] += flow;
    capacity[e.id] -= flow;
    capacity[back(e.id)] += flow;
}

void relabel(int v, std::vector<int> &h, std::vector<std::vector<edge>> &network, std::vector<int> &capacity) {
    int min_h = INT_MAX;
    for(auto u : network[v]) {
        if(capacity[u.id] > 0) min_h = std::min(min_h, h[u.dest]);
    }
    if(min_h != INT_MAX) h[v] = min_h + 1;
}

void discharge(int v, int s, int t, int n, std::vector<int> &h, std::vector<std::vector<edge>> &network, std::vector<int> &capacity, std::vector<int> &last, std::vector<long long> &preflow, std::queue<int>&q) {
    while(preflow[v] > 0) {
        int u = last[v];
        if(u < (int)network[v].size()) {
            edge e = network[v][u];
            if(capacity[e.id] > 0 && h[e.dest] == h[v] - 1) push(v, e, s, t, network, capacity, preflow, last, q);
            else last[v]++;
        }
        else {
            relabel(v, h, network, capacity);
            last[v] = 0;
        }
    }
}

long long push_relabel(int s, int t, int n, int m, std::vector<std::vector<edge>> &network, std::vector<int> &capacity) {
    std::vector<int> last(n, 0);
    std::vector<long long> preflow(n, 0);
    long long operation_counter = 0;
    std::vector<int> h(n);
    std::queue<int> q;
    
    global_relabel(s, t, n, h, network, capacity);

    // najpierw pushujemy do wszystkich sasiadow tyle ile sie da
    // i wrzucamy je od razu na kolejke 
    
    preflow[s] = LLONG_MAX;
    for(auto e : network[s]) push(s, e, s, t, network, capacity, preflow, last, q);

    // i robimy discharge kazdemu
    while(!q.empty()) {
        if(operation_counter == n + m) global_relabel(s, t, n, h, network, capacity);
        int v = q.front();
        q.pop();
        discharge(v, s , t,  n, h, network, capacity, last, preflow, q);
        operation_counter++;
    }

    return preflow[t];
}

int main() {
    //sprawdz czy long longi
    int q;
    std::cin >> q;

    while(q--) {
        int n, m, s, t;
        std::cin >> n >> m >> s >> t;
        s--; t--;

        std::vector<std::vector<edge>> network(n);
        std::vector<int> capacity(2 * m);

        int id_counter = 0;
        for(int i = 0; i < m; i++) {
            int u, v, cap;
            std::cin >> u >> v >> cap;
            u--; v--;
            network[u].push_back({v, id_counter * 2});
            capacity[id_counter * 2] = cap;
            network[v].push_back({u, id_counter * 2 + 1});
            capacity[id_counter * 2 + 1] = 0;
            id_counter++;
        }

        std::cout << push_relabel(s, t, n, m, network, capacity) << "\n";
    }

    return 0;
}