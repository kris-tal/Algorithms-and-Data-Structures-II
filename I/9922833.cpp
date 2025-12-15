#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

struct kmr {
   int half[2];
   int idx; 
};

std::vector<kmr> countsort(std::vector<kmr> &T, int n, int k, int place) {
    std::vector<kmr> sorted(n + 1);
    std::vector<int> count(k + 1, 0);
    for(auto el : T) count[el.half[place]]++;

    for(int i = 1; i < k + 1; i++) count[i] += count[i - 1];
    
    for(int i = n; i >= 0; i--) {
        sorted[count[T[i].half[place]] - 1] = T[i];
        count[T[i].half[place]]--;
    }
    return sorted;
}


void KMR(std::vector<int> &KMR, int n, int k) {
    int r = 1;
    std::vector<kmr> T(n + 1);

    while(r < n + 1) {
        for(int i = 0; i < n + 1; i++) T[i] = {{KMR[i], (i + r < n + 1 ? KMR[i + r] : 0)}, i};
        T = countsort(T, n, k, 1);
        T = countsort(T, n, k, 0);
        
        k = 0;
        kmr prev = T[0];
        for(auto el : T) {
            if(prev.half[0] != el.half[0] || prev.half[1] != el.half[1]) k++;
            KMR[el.idx] = k;
            prev = el;
        }
        r *= 2;
    }
}

void LCPR(std::vector<int> &suf, std::vector<int> &r, std::vector<int> &lcpr, std::vector<int> &lots, int n) {
    int p = 0;
    lcpr[0] = 0;
    for(int i = 0; i < n; i++) {
        while(std::max(i, suf[r[i] - 1]) + p < n + 1 && lots[i + p] == lots[suf[r[i] - 1] + p]) p++;
        lcpr[i] = p;
        p = std::max(p - 1, 0);
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    std::cin >> q;

    while(q--) {
        int n;
        std::cin >> n;

        std::vector<int> lots(n + 1);
        std::map<int, int> map;

        int k = 1;  //ilosc kolorow
        for(int i = 0; i < n; i++) {
            int a;
            std::cin >> a;
            if(!map.count(a)) {
                map[a] = k;
                k++;
            }
            lots[i] = map[a];
        }
        lots[n] = 0;        //dodajemy sobie pusty lot ktory zawsze bedzie pierwszy leksykograficznie
        std::vector<int> r(lots);

        KMR(r, n, k);

        std::vector<int> suf(n + 1);
        for(int i = 0; i < n + 1; i++) suf[r[i]] = i;

        std::vector<int> lcpr(n);
        LCPR(suf, r, lcpr, lots, n);

        int max_substr = 0;
        for(auto el : lcpr) max_substr = std::max(max_substr, el);
        std::cout << max_substr + 1 << "\n";   
    }

    return 0;
}