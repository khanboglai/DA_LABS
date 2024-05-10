#include "btree.hpp"
#include <iostream>


std::string ToLower(std::string str) {
    std::string res = "";

    for (char s : str) {
        res += tolower(s);
    }
    return res;
}


int main() {
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
            if (in == "!") {
                std::string cmd;
                std::cin >> cmd;
                if (cmd == "Save") {
                    std::string file_path;
                    std::cin >> file_path;

                    std::ofstream os(file_path, std::ios::binary);

                    // можно отдельно написать проверку на открытие файла, но fail скажет, что есть проблема
                    if (os.fail()) {
                        std::cout << "ERROR: permision denied\n";
                    } else {
                        b.Save(os);
                        os.close();
                        std::cout << "OK" << std::endl;
                    }
                } else if (cmd == "Load") {
                    std::string file_path;
                    std::cin >> file_path;
                    
                    std::ifstream in(file_path, std::ios::binary);

                    if (in.fail()) {
                        std::cout << "ERROR: permision denied\n";
                    } else {
                        b.Load(in);
                        in.close();
                        std::cout << "OK" << std::endl;
                    }
                }
            } else {
                in = ToLower(in);
                std::cout << b.SWV(in) << std::endl;
            }
        }
    }
}
