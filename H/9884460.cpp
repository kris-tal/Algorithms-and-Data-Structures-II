#include <iostream>
#include <vector>
#include <queue>

struct node {
    std::vector<int> links;
    int parent;
    int mismatch;
    bool accept;

    node() : links(2, -1), parent(-1), mismatch(-1), accept(false) {}
};

int add_vert(int v, int a, std::vector<node> &trie) {
    int u = trie.size();
    trie.push_back(node());
    trie[v].links[a] = u;
    trie[u].parent = v;
    return u;
}

void build_word(std::string s, std::vector<node> &trie) {
    int v = 1;
    for(auto c : s) {
        int a = c - '0';
        if(trie[v].links[a] == -1) v = add_vert(v, a, trie);
        else v = trie[v].links[a];
    }
    trie[v].accept = true;
}

void mismatch(int v, std::vector<node> &trie) {
    for(auto a : {0, 1}) {
        int u = trie[v].links[a];
        if(u != -1) {
            int m = trie[v].mismatch;
            while(trie[u].mismatch == -1) {
                if(trie[m].links[a] != -1) {
                    trie[u].mismatch = trie[m].links[a];
                    if(trie[trie[u].mismatch].accept) trie[u].accept = true;
                }
                m = trie[m].mismatch;
            }
        }
    }
}

void mismatch_all(std::vector<node> &trie) {
    std::queue<int> q;
    trie[1].mismatch = 0;
    q.push(1);

    while(!q.empty()) {
        int v = q.front();
        q.pop();

        mismatch(v, trie);

        for(auto a : {0, 1}) {
            if(trie[v].links[a] != -1) q.push(trie[v].links[a]);
        }
    }
}

void add_edge_vert(int v, int a, std::vector<node> &trie) {
    int u = trie[v].mismatch;

    while(trie[u].links[a] == -1) {
        u = trie[u].links[a];
    }

    trie[v].links[a] = trie[u].links[a];
}

void add_edges_all(std::vector<node> &trie) {
    std::queue<int> q;
    q.push(1);

    while(!q.empty()) {
        int v = q.front();
        q.pop();

        for(auto a : {0, 1}) {
            if(trie[v].links[a] != -1) q.push(trie[v].links[a]);
        }

        for(auto a : {0, 1}) {
            if(trie[v].links[a] == -1) add_edge_vert(v, a, trie);
        }
        
    }
}

bool dfs(int v, std::vector<node> &trie, std::vector<int> &color) {
    color[v] = 1;

    for(auto a : {0, 1}) {
        int u = trie[v].links[a];
        if(trie[u].accept) continue;
        if(color[u] == 1) return true;
        else if(color[u] == 0) {
            if(dfs(u, trie, color)) return true;
        }
    }
    color[v] = 2;
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    std::cin >> q;

    while(q--) {
        int n;
        std::cin >> n;

        std::vector<node> trie;
        trie.push_back(node());
        trie.push_back(node());
        trie[0].links[0] = 1;
        trie[0].links[1] = 1;

        for(int i = 0; i < n; i++) {
            std::string s;
            std::cin >> s;
            build_word(s, trie);
        }

        mismatch_all(trie);
        add_edges_all(trie);

        std::vector<int> color(trie.size(), 0);
        if(dfs(1, trie, color)) std::cout << "TAK\n";
        else std::cout << "NIE\n";
    }

    return 0;
}