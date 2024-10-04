#include "sufftree.hpp"


int main() {

    std::string str;
    std::cin >> str;
    int pattern_len = str.length();
    
    TSuffixTree t(str);
    std::vector<int> ms;
    std::string text;
    std::cin >> text;

    t.MatchStatistic(ms, text);

    for (size_t i = 0; i < ms.size(); i++) {
        if (ms[i] == pattern_len) {
            std::cout << i + 1 << std::endl;
        }
    }    
}
