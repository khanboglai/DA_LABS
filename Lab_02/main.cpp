#include "btree.hpp"
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <set>

int main()
{
    BTree b;

    std::string in;
    while (std::cin >> in) {
        if (in == "+") {
            int val;
            std::cin >> val;
            if (!b.Search(val)) {
                b.Insert(val);
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "Exist" << std::endl;
            }
        } else if (in == "-") {
            int val;
            std::cin >> val;
            bool res = b.Delete(val);
            if (res) {
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "NoSuchWord" << std::endl;
            }
        }
    }

    // b.Insert(5);
    // b.Insert(6);
    // b.Insert(7);
    // b.Insert(4);

    // b.Display();

    // b.Delete(6);
    // b.Display();
}