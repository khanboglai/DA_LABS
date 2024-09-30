#include "sufftree.hpp"

/*
T = qababaz
P = aba
*/

int main() {

    /*
    T = aobbaoababba
    P = baobab
    */
    std::string str;
    std::cin >> str;
    std::string str1 = str;
    TSuffixTree t(str);
    std::vector<int> ms;
    std::string text;
    std::cin >> text;
    t.MatchStatistic(ms, text);

    // std::cout << str1 << std::endl;

    // for (char el : text) {
    //     std::cout << el << " ";
    // }
    // std::cout << std::endl;

    for (size_t i = 0; i < ms.size(); i++) {
        std::cout << ms[i] << " "; 
    }
    std::cout << std::endl;

    for (size_t i = 0; i < ms.size(); i++) {
        if (ms[i] == int(str1.size())) {
            std::cout << i + 1 << std::endl;
        }
    }    
}
