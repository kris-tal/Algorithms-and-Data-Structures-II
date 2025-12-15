#include <iostream>
#include <vector>
#include <unordered_map>

int square_root(int p) {
    int root = 1;
    while(root * root < p) root++;
    return root;
}

int power(int a, int pow, int p) {
    long long result = 1;
    while(pow > 0) {
        if(pow % 2 == 1) result = (result * (long long)a) % p;
        a = ((long long)a * (long long)a) % p;
        pow /= 2;
    }
    return result;
}

int inverse(int a, int p) {
    return power(a, p - 2, p);
}

int find_solution(int a, int b, int p) {
    long long d = square_root(p);
    std::unordered_map<int, int> map;

    int inv_a = inverse(a, p);
    int inv_b = inverse(b, p);
    long long right = 1;
    for(int i = 0; i < d; i++) {
        if(map.find(right) == map.end()) map[right] = i;
        right = (right * (long long)inv_a) % p;
    }

    long long left = 1;
    int a_d = power(a, d, p);
    int m;
    for(int i = 0; i < d; i++) {
        if(map.find((left * (long long)inv_b) % p) != map.end()) {
            m = map[(left * (long long)inv_b) % p];
            return (i * d + m) % p;
        }
        left = (left * (long long)a_d) % p;
    }
    return -1;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int q;
    std::cin >> q;

    while(q--) {
        int a, b, p;
        std::cin >> a >> b >> p;
        std::cout << find_solution(a, b, p) << "\n";
    }

    return 0;
}