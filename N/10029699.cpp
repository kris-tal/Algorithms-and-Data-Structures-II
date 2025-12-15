#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>

//jak mam gorna polowke to musze sprawdzic czy odleglosc ich centrow 

#define ll long long

void printv(const std::vector<int> &v) {
    for(auto el : v) std::cout << el + 1 << "\n";
}

struct circle {
    ll x;
    ll y;
    ll r;
};

enum type {
    BEGIN,
    END
};

enum half {
    BOTTOM,
    TOP,
    
};

struct event {
    type t;
    ll x;
    ll y;
    ll id;
};

struct semi_circ {
    half h;
    ll y;
    ll id;
};

struct cmp {
    bool operator()(const semi_circ& a, const semi_circ& b) const {
        //if(a.id == b.id) return a.h < b.h;   //jak ten sam to chcemy zeby najpierw byl BOTTOM potem TOP
        return a.y < b.y;   //tak potownuje jak w tym miejscu maja sie ich y, i generalnie nie powinno nigdy wyjsc to samo bo sa rozlaczne
    }
};

bool check(const circle &a, const circle &b) {
    auto [x1, y1, r1] = a;
    auto [x2, y2, r2] = b;
    return (ll)(x1 - x2)*(x1 - x2) + (ll)(y1 - y2)*(y1 - y2) < (ll)(r1 - r2)*(r1 - r2);
}

std::vector<int> contains(int n, std::vector<circle> &circles) {
    std::vector<int> smallest_containing(n, -1);
    std::vector<event> events;

    for(int i = 0; i < (int)circles.size(); i++) {
        auto [x, y, r] = circles[i];
        events.push_back({BEGIN, x - r, y, i});
        events.push_back({END, x + r, y, i});
    }

    std::sort(events.begin(), events.end(), [&](const event &a, const event &b){
        if(a.x == b.x) return a.y < b.y;        //nie musze patrzec na type bo nie przecinaja sie i r > 0
        return a.x < b.x;
    });

    std::set<semi_circ, cmp> s;

    for(auto ev : events) {
        if(ev.t == BEGIN) {
            auto it = s.lower_bound({TOP, ev.y, ev.id});
            if(it != s.end()) {
                if(it->h == TOP && check(circles[ev.id], circles[it->id])) {
                    smallest_containing[ev.id] = it->id;
                } 
                else if(it->h == BOTTOM && smallest_containing[it->id] != -1 && check(circles[ev.id], circles[smallest_containing[it->id]])) {
                    smallest_containing[ev.id] = smallest_containing[it->id];
                }
            }
            s.insert({TOP, ev.y + circles[ev.id].r, ev.id});
            s.insert({BOTTOM, ev.y - circles[ev.id].r, ev.id});
        }
        else {
            s.erase({TOP, ev.y + circles[ev.id].r, ev.id});
            s.erase({BOTTOM, ev.y - circles[ev.id].r, ev.id});
        }
    }

    return smallest_containing;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int q;
    std::cin >> q;

    while(q--) {
        int n;
        std::cin >> n;

        std::vector<circle> circles(n);

        for(int i = 0; i < n; i++) {
            std::cin >> circles[i].x >> circles[i].y >> circles[i].r;
        }

        std::vector<int> smallest_containing = contains(n, circles);
        printv(smallest_containing);
    }

    return 0;
}