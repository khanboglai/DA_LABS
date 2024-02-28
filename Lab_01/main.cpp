#include <iostream>
#include "simple_vector.hpp"


typedef unsigned long long ull;

struct Object {
    ull key;
    std::string value;
};

ull GetDigit(ull elem, int i) {
    ull digit = (elem >> (8 * i)) & 0xFF;
    return digit;
}


void Radix(TSimpleVector<Object> & mas) {

    for (ull i = 0; i < 8; i++) {
        
        ull max_elem = 0;
        for (size_t j = 0; j < mas.Size(); j++) {
            ull digit = GetDigit(mas[j].key, i);
            if (max_elem < digit) {
                max_elem = digit;
            }
        }

        size_t cnt_sz = max_elem + 1;
        TSimpleVector<ull> cnts(cnt_sz, 0);

        for (size_t j = 0; j < mas.Size(); j++) {
            cnts[GetDigit(mas[j].key, i)]++;
        }

        for (size_t j = 1; j < cnt_sz; j++) {
            cnts[j] += cnts[j - 1];
        }

        TSimpleVector<Object> result(mas.Size());
        size_t sz = mas.Size();
        for (int j = sz - 1; j >= 0; j--) {
            ull pos = cnts[GetDigit(mas[j].key, i)] - 1;
            result[pos] = mas[j];
            cnts[GetDigit(mas[j].key, i)] = pos;
        }

        mas.Clear();
        for (size_t j = 0; j < result.Size(); j++) {
            mas.PushBack(result[j]);
        }
    }
}


int main() {
    TSimpleVector<Object> mas;

    Object elem;
    while (std::cin >> elem.key >> elem.value) {
        mas.PushBack(elem);
    }

    Radix(mas);

    for (size_t i = 0; i < mas.Size(); i++) {
        std::cout << mas[i].key << "\t" <<  mas[i].value << std::endl;
    }

}
