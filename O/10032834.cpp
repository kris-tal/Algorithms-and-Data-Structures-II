#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>

const double PI = acos(-1);
std::vector<std::vector<std::complex<double>>> roots(2);

void printv_rev(const std::vector<int> &v, bool sign) {
    if(sign) std::cout << "-";
    for(int i = v.size() - 1; i >= 0; i--) std::cout << v[i];
    std::cout << "\n";
}

std::vector<int> normalize(int N, std::vector<std::complex<double>> A) {
    std::vector<int> result;
    int end = N - 1;
    while(std::round(A[end].real()) == 0) end--;

    int carry = 0;
    for(int i = 0; i <= end; i++) {
        int a = std::round(A[i].real()) + carry;
        carry = a / 10;
        result.push_back(a % 10);
    }

    if(carry != 0) result.push_back(carry);
    return result;
}

void generate_roots(int N) {
    roots[0].resize(N);
    roots[1].resize(N);
    double angle = 2 * PI / N;
    for(int i = 0; i < N; i++) {
        roots[0][i] = std::polar(1.0, angle * i); 
        roots[1][i] = std::polar(1.0, -1 * angle * i); 
    }
}

void fft(int n, std::vector<std::complex<double>> &A, int N, bool invert) {
    if(n == 1) return;

    std::vector<std::complex<double>> A_e(n / 2), A_o(n / 2);
    for(int i = 0; 2 * i < n; i++) {
        A_e[i] = A[2 * i];
        A_o[i] = A[2 * i + 1];
    }

    fft(n / 2, A_e, N, invert);
    fft(n / 2, A_o, N, invert);

    int step = N / n;
    for(int i = 0; i < n / 2; i++) {
        std::complex<double> w = roots[invert][i * step];
        A[i] = A_e[i] + w * A_o[i];
        A[i + (n / 2)] = A_e[i] - w * A_o[i];
    }

    if(invert) {
        for(int i = 0; i < n; i++) A[i] /= 2.0;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int q;
    std::cin >> q;

    while(q--) {
        std::string s1, s2;

        std::cin >> s1 >> s2;
        if(s1 == "0" || s2 == "0") {
            std::cout << "0\n";
            continue;
        }
        
        int n = s1.size() + s2.size() - 1;
        int N = 1 << std::__lg(n);
        if(N != n) N <<= 1;
        std::vector<std::complex<double>> A(N, 0), B(N, 0), C(N, 0);

        bool sign_1 = s1[0] == '-';
        bool sign_2 = s2[0] == '-';

        for(int i = s1.size() - sign_1 - 1; i >= 0; i--) {
            A[i] = s1[s1.size() - i - 1] - '0';
        }
        for(int i = s2.size() - sign_2 - 1; i >= 0; i--) {
            B[i] = s2[s2.size() - i - 1] - '0';
        }

        generate_roots(N);

        fft(N, A, N, false);
        fft(N, B, N, false);
        
        for(int i = 0; i < N; i++) C[i] = A[i] * B[i];

        fft(N, C, N, true);
        
        std::vector<int> result = normalize(N, C);

        printv_rev(result, (sign_1 + sign_2) % 2);
    }

    return 0;
}