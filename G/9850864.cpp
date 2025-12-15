#include <iostream>
#include <vector>
#include <algorithm>

void zet_function(const std::string &S, std::vector<int> &Z, int N) {
    for(int i = 1, j = 0, k; i <= N; i += k, j = std::max(j - k, 0)) {
        while(i + j < N && S[j] == S[i + j]) j++;
        Z[i] = j;
        for(k = 1; k + Z[k] < j; k++) Z[i + k] = Z[k];
    }
}

int main() {
    int q;
    std::cin >> q;

    while(q--) {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(nullptr);

        int nw, ns;
        std::string w, s;
        std::cin >> nw >> w >> ns >> s;

        std::string S = w + "$" + s + "#" + w;
        int N = S.size();
        
        std::vector<int> Z(N + 1);
        zet_function(S, Z, N);

        std::string S_rev = S;
        std::reverse(S_rev.begin(), S_rev.end());

        std::vector<int> Z_rev(N + 1);
        zet_function(S_rev, Z_rev, N);
        std::reverse(Z_rev.begin(), Z_rev.end());

        //zaznaczanie
        std::vector<int> intervals(N + 1, 0);

        for(int i = 0; i < N + 1; i++) {
            if(Z[i] + Z_rev[i] >= nw) {
                intervals[i - Z_rev[i]] += 1;
                intervals[i + Z[i] - nw + 1] -= 1;
            }
        }

        for(int i = 1; i < N + 1; i++) intervals[i] += intervals[i - 1];

        int counter = 0;
        std::vector<int> ans;

        for(int i = nw + 1; i < N - nw - 1; i++) {
            if(intervals[i] > 0) {
                counter++;
                ans.push_back(i - nw - 1);
            }
        }

        std::cout << counter << " ";
        for(auto el : ans) std::cout << el << " ";
        std::cout << "\n";
        
    }
    return 0;
}