#include <iostream>
#include "simple_vector.hpp"
#include <vector>

typedef unsigned long long ull;

struct Object {
    ull key;
    std::string value;
};

int GetDigit(int elem, int i) {
    if (i > 0) {
        elem >>= 8;
    }

    return elem & 0xFF;
}


void Radix(TSimpleVector<Object> & mas) {

    for (ull i = 0; i < 8; i++) {
        
        int max_elem = 0;
        for (size_t j = 0; j < mas.Size(); j++) {
            int digit = GetDigit(mas[j].key, i);
            if (max_elem < digit) {
                max_elem = digit;
            }
        }

        size_t cnt_sz = max_elem + 1;
        TSimpleVector<int> cnts(cnt_sz, 0);

        for (size_t j = 0; j < mas.Size(); j++) {
            cnts[GetDigit(mas[j].key, i)]++;
        }

        for (size_t j = 1; j < cnt_sz; j++) {
            cnts[j] += cnts[j - 1];
        }

        TSimpleVector<Object> result(mas.Size());
        size_t sz = mas.Size();
        for (int j = sz - 1; j >= 0; j--) {
            int pos = cnts[GetDigit(mas[j].key, i)] - 1;
            result[pos] = mas[j];
            cnts[GetDigit(mas[j].key, i)] = pos;
        }

        for (size_t j = 0; j < mas.Size(); j++) {
            mas[j] = result[j];
        }
    }
}


int main() {
    TSimpleVector<Object> mas;
    // uint64_t val;

    // while (std::cin >> val) {
    //     mas.PushBack(val);
    // }

    Object elem;
    while (std::cin >> elem.key >> elem.value) {
        mas.PushBack(elem);
    }

    Radix(mas);

    // mas.Print();

    for (size_t i = 0; i < mas.Size(); i++) {
        std::cout << mas[i].key << "\t" <<  mas[i].value << std::endl;
    }

}