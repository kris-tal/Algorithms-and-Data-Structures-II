#include <iostream>
#include <vector>
#include <cstdint>
#include <random>

uint64_t power(uint64_t x, uint64_t pow, uint64_t mod) {
    uint64_t result = 1;
    x %= mod;
    while(pow > 0) {
        if(pow % 2 == 1) result = ((__uint128_t)result * x) % mod;
        x = ((__uint128_t)x * x) % mod;
        pow /= 2;
    }
    return result;
}

bool check(uint64_t a, uint64_t d, int s, uint64_t mod) {
    uint64_t factor = power(a, d, mod);
    if(factor == 1 || factor == mod - 1) return false;

    for(int i = 1; i < s; i++) {
        factor = ((__uint128_t)factor * factor) % mod;
        if(factor == mod - 1) return false;
    }
    return true;
}

bool miller_rabin(uint64_t n) {
    if(n == 0 || n == 1 || n == 4) return false;
    else if(n == 2 || n == 3) return  true;

    int tries = 6;
    
    int s = 0;
    uint64_t d = n - 1;
    while(d % 2 == 0) {
        d /= 2;
        s++;
    }

    for(int i = 0; i < tries; i++) {
        int a = (rand() % (n - 4)) + 2;       //
        if(check(a, d, s, n)) return false;
    }

    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int q;
    std::cin >> q;

    while(q--) {
        uint64_t a;
        std::cin >> a;

        bool prime = miller_rabin(a);

        if(prime) std::cout << "TAK\n";
        else std::cout << "NIE\n";
    }

    return 0;
}