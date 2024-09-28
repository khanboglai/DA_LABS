#include "sufftree.hpp"
#include <sstream>
#include <iterator>

/*
T = qababaz
P = aba
*/

int main() {

    std::string str = "baobab";

    TSuffixTree t(str);
    std::vector<int> ms;
    std::string text = "aobbaoababba";
    t.MatchStatistic(ms, text);

    std::cout << str << std::endl;

    for (char el : text) {
        std::cout << el << " ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < text.length(); i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < ms.size(); i++) {
        std::cout << ms[i] << " "; 
    }
    std::cout << std::endl;    
}
