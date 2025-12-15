#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

struct point {
    long long x;
    long long y;
};

long long det(point p, point q, point r) {
    return ((p.x * q.y) + (q.x * r.y) + (r.x * p.y)) - ((q.y * r.x) + (r.y * p.x) + (p.y * q.x));
}

void find_half(point &h, std::stack<point> &H, std::vector<point> &points, int n) {
    H.push(points[0]);
    h = points[1];

    for(int i = 2; i < n; i++) {
        while(!H.empty() && det(H.top(), h, points[i]) <= 0) {
            h = H.top();
            H.pop();
        }
        H.push(h);
        h = points[i];
    }
}

long long calc_area(std::vector<point> &conv_hull, std::vector<point> &points) {
    point p = conv_hull[0];
    point v1;
    point v2 = {conv_hull[1].x - p.x, conv_hull[1].y - p.y};

    long long area = 0;

    for(int i = 1; i < conv_hull.size() - 1; i++) {
        v1 = v2;
        v2 = {conv_hull[i + 1].x - p.x, conv_hull[i + 1].y - p.y};
        area += v1.x * v2.y - v2.x * v1.y;
    }
    return area;
}

int main() {
    int q;
    std::cin >> q;

    while(q--) {
        int n;
        std::cin >> n;

        std::vector<point> points(n);

        for(auto &p : points) {
            std::cin >> p.x >> p.y;
        }

        std::sort(points.begin(), points.end(), [](point &a, point &b){
            if(a.x == b.x) return a.y < b.y;
            return a.x < b.x;
        });

        std::stack<point> D;
        point d;
        find_half(d, D, points, n);

        std::reverse(points.begin(), points.end());
        std::stack<point> U;
        point u;
        find_half(u, U, points, n);

        std::vector<point> conv_hull(D.size() + U.size());
        int i = D.size() - 1;
        int j = D.size() + U.size() - 1;

       while(!D.empty()) {
            conv_hull[i] = D.top(); D.pop();
            i--;
        }

        while(!U.empty()) {
            conv_hull[j] = U.top(); U.pop();
            j--;
        }

        std::cout << calc_area(conv_hull, points) << "\n";
    }

    return 0;
}