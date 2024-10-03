#include "sufftree.hpp"
#include <sstream>

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

    for (size_t i = 0; i < ms.size(); i++) {
        std::cout << ms[i] << " "; 
    }
    std::cout << std::endl;

    for (size_t i = 0; i < ms.size(); i++) {
        if (ms[i] == int(str1.size())) {
            std::cout << i + 1 << std::endl;
        }
    }    

    // std::string text;
    // std::cin >> text;

    // std::string pattern;
    // int cnt = 1;

    // while (std::cin >> pattern) {
    //     std::stringstream res;
    //     int p_l = pattern.length();
    //     TSuffixTree t(pattern);
    //     std::vector<int> ms;
    //     t.MatchStatistic(ms, text);

    //     for (size_t i = 0; i < ms.size(); i++) {
    //         if (ms[i] == p_l) {
    //             res << i + 1 << ", ";
    //         }
    //     }

    //     std::string re = res.str();

    //     if (!re.empty()) {
    //         re = re.erase(re.length() - 2);
    //         std::cout << cnt << ": " << re << std::endl;
                
    //     } 
    //     cnt++;
        
    // }
}
