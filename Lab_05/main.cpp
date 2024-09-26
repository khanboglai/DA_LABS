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
    t.MatchStatistic(text, ms);

    std::cout << str << std::endl;
    std::cout << text << std::endl;
    for (size_t i = 0; i < ms.size(); i++) {
        std::cout << ms[i]; 
    }
    std::cout << std::endl;    
}
