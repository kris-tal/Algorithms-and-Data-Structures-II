#include <iostream>
#include <vector>
#include <bitset>
#include <unordered_set>

struct lin_eq {
    long long a;
    long long b;
    long long c;
};

struct point {
    long long x;
    long long y;
};

unsigned long long count_plateaus(std::vector<lin_eq> rivers, int n) {
    unsigned long long counter = 1;
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(rivers[i].a * rivers[j].b != rivers[j].a * rivers[i].b) counter++;
        }
    }
    counter += n;
    return counter;
}

int main() {
    int q;
    std::cin >> q;

    while(q--) {
        int n, m;
        std::cin >> n >> m;

        std::vector<lin_eq> rivers(n);
        std::vector<point> knights(m);

        for(auto &el : rivers) {
            std::cin >> el.a >> el.b >> el.c;
        }

        for(auto &el : knights) {
            std::cin >> el.x >> el.y;
        }

        unsigned long long plateaus = count_plateaus(rivers, n);

        std::unordered_set<std::bitset<100>> all_knights;

        for(auto p : knights) {
            std::bitset<100> bs(0);
            for(int i = 0; i < n; i++) {
                auto le = rivers[i];
                long long val = le.a * p.x + le.b * p.y + le.c;
                if(val > 0) bs[i] =  true;
            }
            all_knights.insert(bs);
        }

        if(all_knights.size() >= plateaus) std::cout << "PROTECTED\n";
        else std::cout << "VULNERABLE\n";

    }

    return 0;
}