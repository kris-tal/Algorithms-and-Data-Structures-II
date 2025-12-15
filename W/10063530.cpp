#include <iostream>
#include <vector>

void dfs(int v, int parent, int &time, int l, std::vector<std::vector<int>> &graph, std::vector<int> &in, std::vector<int> &out, std::vector<std::vector<int>> &ancest, std::vector<int> &euler_tour) {
    in[v] = time++;
    euler_tour.push_back(v);

    ancest[v][0] = parent;
    for(int i = 1; i <= l; i++) {
        ancest[v][i] = ancest[ancest[v][i - 1]][i - 1];
    }

    for(auto u : graph[v]) {
        if(u == parent) continue;
        dfs(u, v, time, l, graph, in, out, ancest, euler_tour);
    }

    euler_tour.push_back(v);
    out[v] = time++;
}

bool ancest_check(int u, int v, std::vector<int> &in, std::vector<int> &out) {
    if(in[u] <= in[v] && out[u] >= out[v]) return true;
    return false;
}

int lca(int u, int v, int l, std::vector<int> &in, std::vector<int> &out, std::vector<std::vector<int>> &ancest) {
    if(ancest_check(u, v, in, out)) return u;
    if(ancest_check(v, u, in, out)) return v;

    int a = u;
    for(int i = l; i >= 0; i--) {
        if(!ancest_check(ancest[a][i], v, in, out)) {
            a = ancest[a][i];
        }
    }
    return ancest[a][0];
}

void update(int val, int v, int N, std::vector<int> &s_tree) {
    v += N;
    s_tree[v] = val;
    while(v /= 2) s_tree[v] = s_tree[2 * v] + s_tree[2 * v + 1];
}

int query(int q, int N, std::vector<int> &s_tree) {     //w sumie tyle starczy bo pytam zawsze o prefix
    q += N;
    int res = s_tree[q];
    while(q) {
        if(q % 2 == 1) res += s_tree[q - 1];
        q /= 2;
    }
    return res;
}

int main() {
    int q;
    std::cin >> q;

    while(q--) {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(nullptr);
        int n;
        std::cin >> n;

        std::vector<std::vector<int>> intersections(n);

        for(int i = 0; i < n - 1; i++) {
            int a, b;
            std::cin >> a >> b;
            a--; b--;
            intersections[a].push_back(b);
            intersections[b].push_back(a);
        }

        int l = std::__lg(n);
        if(1 << l != n) l++;
        //std::cout << l << "\n";

        std::vector<int> in(n, -1);
        std::vector<int> out(n, -1);
        std::vector<int> euler_tour;
        std::vector<std::vector<int>> ancest(n, std::vector<int>(l + 1));
        int time = 0;
        dfs(0, 0, time, l, intersections, in, out, ancest, euler_tour);

        //std::cout << lca(6, 4, l, in, out, ancest) << "\n";

        int N = 1 << std::__lg(euler_tour.size());
        if(N != euler_tour.size()) N <<= 1;

        std::vector<int> s_tree(2 * N, 0);
        
        int m;
        std::cin >> m;
        int a, b;
        for(int i = 0; i < m; i++) {
            char c;
            std::cin >> c;
            switch(c) {
                case '-':
                    std::cin >> a; a--;
                    update(1, in[a], N, s_tree);
                    update(-1, out[a], N, s_tree);
                    break;
                case '+':
                    std::cin >> a; a--;
                    update(0, in[a], N, s_tree);
                    update(0, out[a], N, s_tree);
                    break;
                default:
                    std::cin >> a >> b; a--; b--;
                    int lca_ = lca(a, b, l, in, out, ancest);
                    int res = query(in[a], N, s_tree);
                    res += query(in[b], N, s_tree);
                    res -= 2 * query(in[lca_], N, s_tree);
                    res += s_tree[in[lca_] + N];
                    std::cout << ((res == 0) ? "TAK" : "NIE") << "\n";
            }
        }
    }

    return 0;
}