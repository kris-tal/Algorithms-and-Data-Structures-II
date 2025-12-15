#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <climits>

#define epsilon 10e-8

typedef long double ld;

void print_ans(int n, int m, std::vector<std::vector<ld>> &a, std::vector<int> &B) {
    std::vector<ld> answer(n + 1, 0);
    for(int i = 1; i < m + 1; i++) {
        if(B[i] < n + 1) answer[B[i]] = a[i][0];
    }

    std::cout << std::fixed << std::setprecision(6);
    std::cout << a[0][0] << "\n";
    for(int i = 1; i < n + 1; i++) {
        std::cout << "x_" << i << " = " << answer[i] << "\n";
    }
}

void pivot(int in, int out, int n, int m, std::vector<std::vector<ld>> &a, std::vector<int> &N, std::vector<int> &B) {
    ld alpha = a[out][in];

    for(int j = 0; j < n + 1; j++) {
        a[out][j] = a[out][j] / (-alpha);
    }

    a[out][in] = 1 / alpha;

    for(int i = 0; i < m + 1; i++) {
        if (i == out) continue;
        ld beta = a[i][in];
        a[i][in] = 0;

        for(int j = 0; j < n + 1; j++) {
            a[i][j] += beta * a[out][j];
        }
    }

    std::swap(B[out], N[in]);

}

bool simplex(int n, int m, std::vector<std::vector<ld>> &a, std::vector<int> &N, std::vector<int> &B) {
    while(true) {
        int max_j = -1;
        ld max_val = INT_MIN;
        for(int j = 1; j < n + 1; j++) {
            if(a[0][j] > max_val) {     //przy remisie biore wiekszy index
                max_j = j;
                max_val = a[0][j]; 
            }  
        }
        if(max_val <= 0) break;

        int min_k = -1;
        ld min_val = INT_MAX;
        bool check_if_unbounded = true;
        for(int k = 1; k < m + 1; k++) {
            if(a[k][max_j] < 0) {
                check_if_unbounded = false;
                ld val = -a[k][0] / a[k][max_j];
                if(val < min_val) {                 //znow przy remisie wybieram mniejszy index
                    min_k = k;
                    min_val = val;
                }
            }
        }
        if(check_if_unbounded) return false;

        pivot(max_j, min_k, n, m, a, N, B);

    }

    print_ans(n, m, a, B);
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int q;
    std::cin >> q;

    while(q--) {
        int n, m;
        std::cin >> n >> m;

        std::vector<std::vector<ld>> a(m + 1, std::vector<ld>(n + 1, 0));


        std::vector<int> N(n + 1);      //bede indeksowac od 1 zeby mi sie nie mylilo (i-ta kolumna odpowiada N[i-tej] zmiennej)
        std::iota(N.begin(), N.end(), 0);

        std::vector<int> B(m + 1);      //i to mi generalnie mowi ze i-ty wiersz odpowiada B[i-tej] zmiennej
        std::iota(B.begin(), B.end(), n);
        B[0] = 0;               //jak bede debugowac to tu mozna dac B[0] = -1 i moze sie cos wywali jak zle zindeksowalam

        for(int i = 1; i < n + 1; i++) {
            std::cin >> a[0][i];
        }

        for(int i = 1; i < m + 1; i++) {
            for(int j = 1; j < n + 1; j++) {
                std::cin >> a[i][j];
                a[i][j] *= -1;
            }
            std::cin >> a[i][0];
        }

        if(!simplex(n, m, a, N, B)) std::cout << "UNBOUNDED\n";
    }

    return 0;
}