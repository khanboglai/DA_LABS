#include "sufftree.hpp"

/*
T = qababaz
P = aba
*/

int main() {

    // int n;
    // std::cin >> n;
    std::string str = "baobab";
    // while (std::cin >> str) {
    //     TSuffixTree t(str);
    //     t.PrintEdgesInOrder();
    // }

    TSuffixTree t(str);
    std::vector<int> ms;
    std::string text = "aobbaoababba";
    t.MatchStatistic(text, ms);

    std::cout << str << std::endl;
    std::cout << text << std::endl;
    for (size_t i = 0; i < ms.size(); i++) {
        std::cout << ms[i]; 
    }
    std::cout << std::endl;
}
