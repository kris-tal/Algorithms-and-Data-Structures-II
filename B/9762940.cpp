// robimy sobie ramke z zabronionych pol
// czyli budujemy sobie krawedzie do sasiadow o przepustowosci 1
// zrodla to nasze dzialki (wiele wierzcholkow startowych bfs z wielu miejsc) do wielu ujsc (x)
// i szukamu minimalnego przekroju = max przeplyw (edmonds-karp) 
// wierzch odwiedzone w osttanim bfs to te ktore musimy kupic

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

struct edge {
    int dest;
    int id;
};

struct info {
    int cap;
};

int idx(int i, int j, int m) {
    return i * m + j;
}

int back(int id) {
    return id % 2 == 0 ? id + 1 : id - 1;
}

void printans(std::vector<std::vector<char>> &input, std::vector<edge> &P, int n, int m) {
    for(int i = 1; i < n + 1; i++) {
        for(int j = 1; j < m + 1; j++) {
            if(input[i][j] == '.' && P[idx(i, j, m + 2)].id != -1) std::cout << '#';
            else std::cout << input[i][j];
        }
        std::cout << "\n";
    }
}

struct qe {
    int vert;
    int flow;
};

void update_flow(int v, int flow, int s, std::vector<std::vector<edge>> &network, std::vector<int> &capacity, std::vector<edge> &P) {
    if(v == s) return;
    edge e = P[v];
    capacity[back(e.id)] -= flow;     //tu sprawdz czy nie na odwrot (bylo na odwrot)
    capacity[e.id] += flow;
    update_flow(e.dest, flow, s, network, capacity, P);
}

int bfs(int s, int t, std::vector<std::vector<edge>> &network, std::vector<int> &capacity, std::vector<edge> &P) {
    for(int i = 0; i < (int)P.size(); i++) P[i] = {-1, -1};

    std::queue<qe> q;
    q.push({s, INT_MAX});

    while(!q.empty()) {
        auto [v, flow] = q.front();
        q.pop();

        for(auto e : network[v]) {
            if(e.dest != s && P[e.dest].dest == -1 && capacity[e.id] > 0) {  //czy to na pewno id w dobra strone
                P[e.dest] = {v, back(e.id)};
                int updated_flow = std::min(flow, capacity[e.id]);
                if(e.dest == t) return updated_flow;
                q.push({e.dest, updated_flow});
            }
        }
    }
    return 0;
}

void edmonds_karp(int s, int t, int n, int m, int N, std::vector<std::vector<edge>> &network, std::vector<int> &capacity, std::vector<std::vector<char>> input) {
    long long max_flow = 0;
    int flow = 0;
    std::vector<edge> P(N);

    while((flow = bfs(s, t, network, capacity, P)) > 0) {
        update_flow(t, flow, s, network, capacity, P);
        max_flow += flow;
    }

    std::cout << max_flow << "\n";
    printans(input, P, n, m);
}


int main() {
    //std::ios_base::sync_with_stdio(false);
    //std::cin.tie(nullptr);

    int q;
    std::cin >> q;

    while(q--) {
        int n, m;
        std::cin >> n >> m;

        int N = (n + 2) * (m + 2) + 2;      //liczba wierzcholkow
        int s = N - 2;
        int t = N - 1;
        
        std::vector<std::vector<edge>> network(N);

        std::vector<std::vector<char>> input(n + 2, std::vector<char>(m + 2, '.'));    //tu sobie uzupelnie input o ramke tylko czy ja chce go juz tu rozwinac czy pozniej.. (pozniej)

        for(int i = 0; i < n + 2; i++) {
            for(int j = 0; j < m + 2; j++) {
                if(i % (n + 2) == 0 || i % (n + 2) == n + 1 || j % (m + 2) == 0 || j % (m + 2) == m + 1) input[i][j] = 'x';
                else std::cin >> input[i][j];
            }
        }

        int edges = N * 8;
        std::vector<int> capacity(edges, 0);

        int edge_id = 0;
        for(int i = 0; i < n + 2; i++) {
            for(int j = 0; j < m + 2; j++) {        // i bede tworzyc dla kazdego w prawo i w dol

                //dokladamy zrodlo i ujscie
                if(input[i][j] == 'x') {
                    network[idx(i, j, m + 2)].push_back({t, edge_id * 2});
                    network[t].push_back({idx(i, j, m + 2), edge_id * 2 + 1});
                    capacity[edge_id * 2] = INT_MAX;
                    capacity[edge_id * 2 + 1] = 0;
                    edge_id++;
                    if(i == n + 1 || j == m + 1) continue;
                }
                else if(input[i][j] == '#') {
                    network[s].push_back({idx(i, j, m + 2), edge_id * 2});
                    network[idx(i, j, m + 2)].push_back({s, edge_id * 2 + 1});
                    capacity[edge_id * 2] = INT_MAX;
                    capacity[edge_id * 2 + 1] = 0;
                    edge_id++;
                }

                if(input[i][j] == '.' || input[i][j] != input[i][j + 1]) {      // dodaj krawedzie w prawo
                    network[idx(i, j, m + 2)].push_back({idx(i, j + 1, m + 2), edge_id * 2});
                    network[idx(i, j + 1, m + 2)].push_back({idx(i, j, m + 2), edge_id * 2 + 1});
                    capacity[edge_id * 2] = capacity[edge_id * 2 + 1] = 1;
                    edge_id++;
                }
                if(input[i][j] == '.' || input[i][j] != input[i + 1][j]) {      // dodaj krawedzie w dol
                    network[idx(i, j, m + 2)].push_back({idx(i + 1, j, m + 2), edge_id * 2});
                    network[idx(i + 1, j, m + 2)].push_back({idx(i, j, m + 2), edge_id * 2 + 1});
                    capacity[edge_id * 2] = capacity[edge_id * 2 + 1] = 1;
                    edge_id++;
                }
            }
        }

        edmonds_karp(s, t, n, m, N, network, capacity, input);
    }

    return 0;
}