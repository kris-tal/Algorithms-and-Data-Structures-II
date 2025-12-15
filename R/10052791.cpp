#include <iostream>
#include <vector>
#include <iomanip>

std::vector<std::vector<int>> I(int n) {
    std::vector<std::vector<int>> result(n, std::vector(n, 0));
    for(int i = 0; i < n; i++) result[i][i] = 1;
    return result;
}

std::vector<int> multiply_vector(std::vector<std::vector<int>> &A, std::vector<int> &v) {
    std::vector<int> result(v.size());
    for(int i = 0; i < A.size(); i++) {
        for(int k = 0; k < v.size(); k++) {
            result[i] = (result[i] + ((A[i][k] % 1000) * (v[k] % 1000)) % 1000) % 1000;
        }
    }
    return result;
}


std::vector<std::vector<int>> multiply(std::vector<std::vector<int>> &A, std::vector<std::vector<int>> &B) {
    std::vector<std::vector<int>> result(A.size(), std::vector(B[0].size(), 0));
    for(int i = 0; i < A.size(); i++) {
        for(int j = 0; j < B[0].size(); j++) {
            for(int k = 0; k < B.size(); k++) {
                result[i][j] = (result[i][j] + ((A[i][k] % 1000) * (B[k][j] % 1000)) % 1000) % 1000;
            }
        }
    }
    return result;
}

std::vector<std::vector<int>> power(std::vector<std::vector<int>> &matrix, int pow, int n) {
    std::vector<std::vector<int>> result = I(n);

    while(pow > 0) {
        if(pow % 2 == 1) result = multiply(matrix, result);
        pow /= 2;
        matrix = multiply(matrix, matrix);
    }
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int q;
    std::cin >> q;

    while(q--) {
        int k, m;
        std::cin >> k >> m;

        std::vector<std::vector<int>> matrix(k, std::vector(k, 0));
        std::vector<int> first_k(k);

        for(int i = 0; i < k; i++) {
            int a;
            std::cin >> a;
            matrix[0][i] = a % 1000;
        }

        int i = 1, j = 0;
        while(i < k) {
            matrix[i][j] = 1;
            i++; j++;
        }

        for(int i = 0; i < k; i++) {
            int a;
            std::cin >> a;
            first_k[i] = a % 1000;
        }

        if(m <= k) {
            std::cout << std::setw(3) << std::setfill('0') << first_k[k - m] << "\n";
            continue;
        }

        matrix = power(matrix, m - k, k);

        std::vector<int> result = multiply_vector(matrix, first_k);

        std::cout << std::setw(3) << std::setfill('0') << result[0] % 1000 << "\n";
    }

    return 0;
}