#include <iostream>
#include <vector>
#include <unordered_map>

std::vector<int> findAnagrams(std::string s, std::string p) {
    std::vector<int> result;

    std::unordered_map<char, int> p_map; // вставка и удлаение за O(1)
    for (char c : p) {
        p_map[c]++;
    }

    std::unordered_map<char, int> s_map; // расчитаем для первых p cимволов
    for (int i = 0; i < p.size(); i++) {
        s_map[s[i]]++;
    }

    if (s_map == p_map) {
        result.push_back(0);
    }

    for (int i = p.size(); i < s.size(); i++) {

        s_map[s[i - p.size()]]--; // помечаем те, что уже прошли

        if (s_map[s[i - p.size()]] == 0) {
            s_map.erase(s[i - p.size()]); // удаляем те, что прошли
        }

        s_map[s[i]]++; // считаем новые

        if (s_map == p_map) {
            result.push_back(i - p.size() + 1); // мы находимся на i+p
        }
    }

    return result;
}


int main() {
    std::vector<int> r = findAnagrams("cbacebabcacd", "abcc");

    for (int elem : r) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}
