#include <iostream>
#include "simple_vector.hpp"
#include "object.hpp"


// получаю разряд числа, двигаюсь по байтам
int GetDigit(uint64_t & elem, int & i) {
    int digit = (elem >> (8 * i)) & 0xFF;
    return digit;
}


int MaxD(TSimpleVector<TObject> & mas, int & d) {
    int max_elem = 0;
    for (int j = 0; j < (int)(mas.Size()); j++) {
        int digit = GetDigit(mas[j].key, d);
        if (max_elem < digit) {
            max_elem = digit;
        }
    }
    return max_elem;
}


int MinD(TSimpleVector<TObject> & mas, int & d) {
    int min_elem = 0;
    for (int j = 0; j < (int)(mas.Size()); j++) {
        int digit = GetDigit(mas[j].key, d);
        if (min_elem > digit) {
            min_elem = digit;
        }
    }
    return min_elem;
}


void CountingSort(TSimpleVector<TObject>& mas, int & i) {
    int sz = mas.Size();

    int max_elem = MaxD(mas, i);
    int min_elem = MinD(mas, i);

    // создание вектора для подсчета элементов
    int cnt_sz = max_elem - min_elem + 1;
    int cnts[cnt_sz] = {0};

    // подсчет элементов
    for (int j = 0; j < sz; j++) {
        cnts[GetDigit(mas[j].key, i)]++;
    }


    // подсчет префикс-суммы. Сколько элементов >= текущему стоит находится впереди
    for (int j = 1; j < cnt_sz; j++) {
        cnts[j] += cnts[j - 1];
    }

    // блок сбора отсортированной части
    TSimpleVector<TObject> interm_result(sz);

    // идем с конца
    for (int j = sz - 1; j >= 0; j--) {

        // вычисляем позицию
        int pos = cnts[GetDigit(mas[j].key, i)] - 1;
        // сохраняем результат
        interm_result[pos] = std::move(mas[j]);
        // уменьшаем значение префиксной-суммы
        cnts[GetDigit(mas[j].key, i)] = pos;
    }
    
    // delete[] cnts;

    mas = std::move(interm_result);
}


// алгоритм поразрядной сортировки
void Radix(TSimpleVector<TObject> & mas) {
    
    // 8 байт под мой тип данных
    for (int i = 0; i < 8; i++) {
        CountingSort(mas, i);
    }
}


int main() {
    // std::ios::sync_with_stdio(false);
    // std::cin.tie(0);
    // std::cout.tie(0);

    TSimpleVector<TObject> mas;

    // считывание элементов
    TObject elem;
    while (std::cin >> elem.key >> elem.value) {
        mas.PushBack(elem);
    }

    // вызов функции сортировки
    Radix(mas);

    // вывод
    for (size_t i = 0; i < mas.Size(); i++) {
        std::cout << mas[i].key << "\t" <<  mas[i].value << std::endl;
    }

}
