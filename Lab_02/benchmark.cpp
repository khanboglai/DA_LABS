#include <map>
#include <cstdio>
#include <cctype>
#include <limits.h>
#include <cstring>
#include <utility>
#include "btree.hpp"
#include <iomanip>


struct cmp_str {
    bool operator()(const char* a, const char* b) const {
        return std::strcmp(a, b) < 0;
    }
};


std::string ToLower(std::string str) {
    std::string res = "";

    for (char s : str) {
        res += tolower(s);
    }
    return res;
}


int main() {
    long double MapTime = 0;
    long double BtreeTime = 0;
    long double start, end;

    std::string input;
    std::map<std::string, uint64_t> map;
    TBTree b;

    while (std::cin >> input) {
        if (!strcmp(input.c_str(), "+")) {
            std::string inp;
            uint64_t value;
            std::cin >> inp;
            std::cin >> value;
            inp = ToLower(inp);

            // btree
            start = clock();
            TElem e;
            e.key = inp;
            e.value = value;

            if (!b.Search(e.key)) {
                b.Insert(e);
            }
            end = clock();
            BtreeTime = BtreeTime + end - start;

            // map
            start = clock();
            if (map.find(inp) == map.end()) {
                map.insert(std::make_pair(inp, value));
            } 
            end = clock();
            MapTime = MapTime + end - start;

        } else if (!strcmp(input.c_str(), "-")) {
            std::string inp;
            std::cin >> inp;
            inp = ToLower(inp);
            
            // btree
            start = clock();
            b.Delete(inp);
            end = clock();
            BtreeTime = BtreeTime + end - start;

            // map
            start = clock();
            if (map.find(inp) != map.end()) {
                map.erase(inp);
            } 
            end = clock();
            MapTime = MapTime + end - start;
        } else {
            
            // btree
            start = clock();
            b.Search(input);
            end = clock();
            BtreeTime = BtreeTime + end - start;

            // map
            start = clock();
            map.find(input);
            end = clock();
            MapTime = MapTime + end - start;
        }
    }   

    std::cout << "Btree: " << std::fixed << std::setprecision(3) << BtreeTime / 1000.0 << " ms"<< std::endl;
    std::cout << "Map: " << std::fixed << std::setprecision(3) << MapTime / 1000.0 << " ms" << std::endl;
}
