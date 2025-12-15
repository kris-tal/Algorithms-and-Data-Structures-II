#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>

struct picture {
    int cost;
    std::bitset<20*20> bitmap;

    picture() : cost(0), bitmap(0) {}
};

long long independent_cost(int n, int size, std::vector<picture> &pictures) {
    long long total = 0;
    std::sort(pictures.begin(), pictures.end(), [&](const picture &a, const picture &b){
        return a.cost < b.cost;
    });

    for(int i = 0; i < n; i++) {
        int j = 0;
        while(j < size && pictures[i].bitmap[j] != 1) j++;
        if(j == size) continue;
        total += pictures[i].cost;
        for(int k = i + 1; k < n; k++) {
            if(pictures[k].bitmap[j]) pictures[k].bitmap ^= pictures[i].bitmap;
        }
    }

    return total;
}

int main() {
    int q;
    std::cin >> q;

    while(q--) {
        int n, a, b;
        std::cin >> n >> a >> b;

        std::vector<picture> pictures(n);

        for(int i = 0; i < n; i++) {
            std::cin >> pictures[i].cost;
            for(int j = 0; j < a; j++) {
                for(int k = 0; k < b; k++) {
                    char c;
                    std::cin >> c;
                    pictures[i].bitmap[j * b + k] = (c == 'x') ? 1 : 0;
                }
            }
        }
        std::cout << independent_cost(n, a * b, pictures) << "\n";
    }

    return 0;
}