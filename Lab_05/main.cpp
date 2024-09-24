#include "sufftree.hpp"

/*
T = qababaz
P = aba
*/

int main() {

    int n;
    std::cin >> n;
    std::string str;
    while (std::cin >> str) {
        TSuffixTree t(str);
        t.PrintEdgesInOrder();
    }
    
}
