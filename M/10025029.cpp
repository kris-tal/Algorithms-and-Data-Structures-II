#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

enum type {
    BEGIN = 6,
    END = 21,
    QUERY = 18
};

struct event {
    type t;
    int x;
    int y1;
    int y2;

    event(type t, int x, int y1, int y2) : t(t), x(x), y1(y1), y2(y2) {} 
};

struct segment {
    int x;
    int y;
    int l;
};

bool sweep(int d, std::vector<segment> &segments_vert, std::vector<segment> &segments_hori) {
    std::vector<event> events;

    for(auto seg : segments_vert) {
        if(seg.l >= 2 * d) events.emplace_back(QUERY, seg.x, seg.y + d, seg.y + seg.l - d);
    }

    for(auto seg : segments_hori) {
        if(seg.l >= 2 * d) {
            events.emplace_back(BEGIN, seg.x + d, seg.y, 0);
            events.emplace_back(END, seg.x + seg.l - d, seg.y, 0);
        }
    }

    std::sort(events.begin(), events.end(), [&](const event &a, const event &b){
        if(a.x == b.x) return a.t < b.t;
        return a.x < b.x;
    });

    std::set<int> s;

    for(auto ev : events) {
        switch(ev.t) {
            case BEGIN:
                s.insert(ev.y1);
            break;
            case END:
                s.erase(ev.y1);
            break;
            case QUERY:
                auto it = s.lower_bound(ev.y1);
                if(it != s.end() && *(it) <= ev.y2) return true;
            break;
        }
    }
    return false;
}

int binsearch(int max, std::vector<segment> &segments_vert, std::vector<segment> &segments_hori) {
    int l = 0, r = max + 1;
    while(l < r) {
        int mid = (l + r + 1) / 2;
        if(!sweep(mid, segments_vert, segments_hori)) r = mid - 1;
        else l = mid;
    }
    return l;       //
}

int main() {
    int q;
    std::cin >> q;

    while(q--) {
        int n, m;
        std::cin >> n >> m;

        std::vector<event> events;
        std::vector<segment> segments_vert(n); 
        std::vector<segment> segments_hori(m); 

        int max_l_vert = 0;
        int max_l_hori = 0;

        for(int i = 0; i < n; i++) {
            int x, y, l;
            std::cin >> x >> y >> l;
            segments_vert[i] = {x, y, l};
            max_l_vert = std::max(max_l_vert, l);
        }

        for(int i = 0; i < m; i++) {
            int x, y, l;
            std::cin >> x >> y >> l;
            segments_hori[i] = {x, y, l};
            max_l_hori = std::max(max_l_hori, l);
        }

        std::cout << binsearch(std::min(max_l_vert, max_l_hori), segments_vert, segments_hori) << "\n";
    }

    return 0;
}