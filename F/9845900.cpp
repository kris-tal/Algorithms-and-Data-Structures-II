#include <iostream>
#include <vector>

void printv(std::vector<int> v) {
    for(auto el : v) std::cout << el << " ";
    std::cout << "\n";
}
struct ans {
    int len;
    int rep;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int q;
    std::cin >> q;

    while(q--) {
        int n;
        std::string s;
        std::cin >> n >> s;

        std::vector<int> pi(n + 1, 0);
        int t = pi[0] = -1;

        for(int i = 0; i < n; i++) {
            while(t != -1 && s[t] != s[i]) t = pi[t];
            pi[i + 1] = ++t;
        }

        std::vector<ans> answ;

        for(int i = 1; i < n + 1; i++) {
            if(pi[i] != 0 && i % (i - pi[i]) == 0) answ.push_back({i, i / (i - pi[i])});
        }

        std::cout << answ.size() << "\n";
        for(auto el : answ)  std::cout << el.len << " " << el.rep << "\n";
    }

    return 0;
}