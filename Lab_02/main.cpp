#include "btree.hpp"
#include <iostream>


std::string ToLower(std::string str) {
    std::string res = "";

    for (char s : str) {
        res += tolower(s);
    }
    return res;
}


int main()
{
    BTree b;

    std::string in;
    while (std::cin >> in) {
        if (in == "+") {
            Elem e;
            std::cin >> e.key >> e.value;
            e.key = ToLower(e.key);
            if (!b.Search(e.key)) {
                b.Insert(e);
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "Exist" << std::endl;
            }
        } else if (in == "-") {
            std::string key;
            std::cin >> key;
            key = ToLower(key);
            bool res = b.Delete(key);
            if (res) {
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "NoSuchWord" << std::endl;
            }
        } else {
            in = ToLower(in);
            std::cout << b.SWV(in) << std::endl;
        }
    }
}
