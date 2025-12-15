// listy albo macierz sasiedztwa 
// jesli lista sasiedztwa to trzymamy listy krawedzi wychodzacych z wierzch
// i w kazdej kraw trzymamy gdzie ta krawedz prowadzi (destination) i id krawedzi przeciwnej
// czyli mozemy miec tablice indeksowana id i trzymamy to w dwoch tablicach w drugiej faktyczne wartosci
// (edmonds-karp)

#include <iostream>
#include <vector>
#include <climits>
#include <queue>

struct qe {
    int vert;
    int flow;
};

void update_flow(int v, int flow, int s, std::vector<std::vector<int>> &matrix, std::vector<int> &P) {
    if(v == s) return;
    int p = P[v];
    matrix[p][v] -= flow;
    matrix[v][p] += flow;
    update_flow(p, flow, s, matrix, P);
}

int bfs(int s, int t, std::vector<std::vector<int>> &matrix, std::vector<int> &P) {
    for(int i = 0; i < P.size(); i++) P[i] = -1;

    std::queue<qe> q;
    q.push({s, INT_MAX});

    while(!q.empty()) {
        auto [v, flow] = q.front();
        q.pop();

        for(int u = 0; u < P.size(); u++) {
            if(u != s && P[u] == -1 && matrix[v][u] > 0) {
                P[u] = v;
                int updated_flow = std::min(flow, matrix[v][u]);
                if(u == t) return updated_flow;
                q.push({u, updated_flow});
            }
        }
    }
    return 0;
}

int edmonds_karp(int s, int t, int n, int m, std::vector<std::vector<int>> &matrix) {
    long long max_flow = 0;
    int flow = 0;
    std::vector<int> P(n + m + 2);

    while((flow = bfs(s, t, matrix, P)) > 0) {
        update_flow(t, flow, s, matrix, P);
        max_flow += flow;
    }

    return max_flow;
}

int main() {
    int q;
    std::cin >> q;

    while(q--) {
        int n, m;
        std::cin >> n >> m;

        std::vector<std::vector<int>> matrix(n + m + 2, std::vector<int>(n + m + 2, 0));
        std::vector<std::vector<int>> network(n + m + 2);

        for(int i = 0; i < n; i++) {
            int cap;
            std::cin >> cap;
            matrix[m + n][i] = cap;
            network[m + n].push_back(i);
            network[i].push_back(m + n);
        }

        for(int i = n; i < n + m; i++) {
            int cap;
            std::cin >> cap;
            matrix[i][n + m + 1] = cap;
            network[m + n + 1].push_back(i);
            network[i].push_back(m + n + 1);
        }

        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                int e;
                std::cin >> e;
                if(e) {
                    matrix[i][j + n] = INT_MAX;
                    network[i].push_back(j + n);
                    network[j + n].push_back(i);
                }
            }
        }

        std::cout << edmonds_karp(m + n, m + n + 1, n, m, matrix) << "\n";
    }

    return 0;
}