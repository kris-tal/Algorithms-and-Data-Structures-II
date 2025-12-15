//czyli chcemy turbo matchingiem znalezc max skojarzenie
//jesli |M| = n to TAK wpp NIE
#include <iostream>
#include <vector>

bool augment(int v, std::vector<std::vector<int>> &graph, std::vector<int> &match, std::vector<bool> &visited) {
    visited[v] = true;
    for(auto u : graph[v]) {
        if(match[u] == -1) {        //jesli mamy sasiada nie w skojarzeniu to sie z nim matchujemy
            match[u] = v;
            match[v] = u;
            return true;
        }
    }
    for(auto u : graph[v]) {
        if(!visited[match[u]] && augment(match[u],graph, match, visited)) {    //xorujemy sie rekrencyjnie
            match[u] = v;
            match[v] = u;
            return true;
        }
    }
    return false;
}

bool turbo_matching(int n, std::vector<std::vector<int>> &graph) {
    std::vector<int> match(2 * n, -1);
    std::vector<bool> visited(2 * n);

    bool found = true;
    while(found) {
        for(int i = 0; i < 2 * n; i++) visited[i] = false;
        found = false;
        for(int u = 0; u < n; u++) {
            if(match[u] == -1 && augment(u, graph, match, visited)) found = true;
        }
    }

    for(int i = 0; i < n; i++) {
        if(match[i] == -1) return false;
    }
    return true;
}

int main() {
    int q;
    std::cin >> q;

    while(q--) {
        int n, m;
        std::cin >> n >> m;
        
        std::vector<std::vector<int>> graph(2 * n);

        for(int i = 0; i < m; i++) {
            int a, b;
            std::cin >> a >> b;
            if(a == b) continue;
            a--; b--;
            graph[a].push_back(n + b);
            graph[n + b].push_back(a);
        }

        if(turbo_matching(n, graph)) std::cout << "TAK\n";
        else std::cout << "NIE\n";
    }

    return 0;
}