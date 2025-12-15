//chce znalezc max zbior niezalezny
//czyli chce znalezc min pokrycie wierzcholkowe

#include <iostream>
#include <vector>

int idx(int i, int j, int m) {
    return (i - 1) * m + j;
}

void printans(int n, int m, std::vector<bool> &cover, std::vector<std::vector<char>> input) {
    int counter = 0;
    int ans = 0;
    for(int i = 1; i < n + 1; i++) {
        for(int j = 0; j < m; j++) {
            if(input[i][j] != 'x' && cover[counter] == 0) {
                input[i][j] = 's';
                ans++;
            }
            counter++;
        }
    }
    std::cout << ans << '\n';

    counter = 0;
    
    for(int i = 1; i < n + 1; i++) {
        for(int j = 0; j < m; j++) {
            if(input[i][j] == 'x') std::cout << 'x';
            else if(cover[counter] == 0) std::cout << 's';
            else std::cout << '.';
            counter++;
        }
        std::cout << "\n";
    }
}

void dfs(int v, bool odd, bool prev, std::vector<std::vector<int>> &graph, std::vector<int> &match, std::vector<bool> &visited, std::vector<bool> &cover) {
    visited[v] = true;
    if(odd) cover[v] = true;

    if(!prev) {
        if(match[v] == -1) return;
        dfs(match[v], !odd, !prev, graph, match, visited, cover);
        return;
    }
    
    for(auto u : graph[v]) {
        if(!visited[u] && match[v] != u) dfs(u, !odd, !prev, graph, match, visited, cover);
    }
}

void find_cover(int n, int m, std::vector<std::vector<int>> &graph, std::vector<int> &match, std::vector<std::vector<char>> &input, std::vector<bool> &color) {
    std::vector<bool> visited(n * m, false);
    std::vector<bool> cover(n * m, false);

    for(int v = 0; v < n * m; v++) {
        if(match[v] == -1 && !visited[v]) {
            dfs(v, 0, true, graph, match, visited, cover);
        } 
    }
    for(int v = 0; v < n * m; v++) {
        if(!visited[v] && color[v] == 1) {
            cover[v] = true;
        }
    }

    printans(n, m, cover, input);
}

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
        if(!visited[match[u]] && augment(match[u], graph, match, visited)) {    //xorujemy sie rekrencyjnie
            match[u] = v;
            match[v] = u;
            return true;
        }
    }
    return false;
}

void turbo_matching(int n, int m, std::vector<std::vector<int>> &graph, std::vector<std::vector<char>> &input, std::vector<bool> color) {
    std::vector<int> match(n * m, -1);
    std::vector<bool> visited(n * m);

    bool found = true;
    while(found) {
        for(int i = 0; i < n * m; i++) visited[i] = false;
        found = false;
        for(int u = 0; u < n * m; u++) {
            if(color[u] == 0 && match[u] == -1 && augment(u, graph, match, visited)) found = true;
        }
    }

    find_cover(n, m, graph, match, input, color);
}

int main() {
    int q;
    std::cin >> q;

    while(q--) {
        int n, m;
        std::cin >> n >> m;

        std::vector<std::vector<int>> graph(n * m);
        std::vector<bool> color(n * m, 0);
        std::vector<std::vector<char>> input(n + 2, std::vector<char>(m + 1));

        for(int j = 0; j < m + 1; j++) {
            input[0][j] = 'x';
            input[n + 1][j] = 'x';
        }
        for(int i = 1; i < n + 1; i++) {
            for(int j = 0; j < m; j++) {
                char c;
                std::cin >> c;
                input[i][j] = c;
            }
            input[i][m] = 'x';
        }

        for(int i = 1; i < n + 1; i++) {
            for(int j = 0; j < m; j++) {
                color[idx(i, j, m)] = j % 2;
                if(input[i][j] == 'x') continue;
                if(input[i - 1][j + 1] != 'x') {
                    graph[idx(i, j, m)].push_back(idx(i - 1, j + 1, m));
                    graph[idx(i - 1, j + 1, m)].push_back(idx(i, j, m));
                }
                if(input[i][j + 1] != 'x') {
                    graph[idx(i, j, m)].push_back(idx(i, j + 1, m));
                    graph[idx(i, j + 1, m)].push_back(idx(i, j, m));
                }
                if(input[i + 1][j + 1] != 'x') {
                    graph[idx(i, j, m)].push_back(idx(i + 1, j + 1, m));
                    graph[idx(i + 1, j + 1, m)].push_back(idx(i, j, m));
                }
            }
        }

        turbo_matching(n, m, graph, input, color);
    }

    return 0;
}