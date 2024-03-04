#include <iostream>
#include "simple_vector.hpp"
#include "object.hpp"


// получаю разряд числа, двигаюсь по байтам
int GetDigit(uint64_t & elem, int & i) {
    int digit = (elem >> (8 * i)) & 0xFF;
    return digit;
}


void CountingSort(TSimpleVector<TObject>& mas, int & i) {
    int sz = mas.Size();


    // создание вектора для подсчета элементов
    int cnt_sz = 256;
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
    TObject interm_result[sz];

    // идем с конца
    for (int j = sz - 1; j >= 0; j--) {

        // вычисляем позицию
        int pos = cnts[GetDigit(mas[j].key, i)] - 1;
        // сохраняем результат
        interm_result[pos] = mas[j];
        // уменьшаем значение префиксной-суммы
        cnts[GetDigit(mas[j].key, i)] = pos;
    }

    for (int i = 0; i < sz; i++) {
        mas[i] = interm_result[i];
    }
}


// алгоритм поразрядной сортировки
void Radix(TSimpleVector<TObject> & mas) {
    
    // 8 байт под мой тип данных
    for (int i = 0; i < 8; i++) {
        CountingSort(mas, i);
    }
}


int main() {

    TSimpleVector<TObject> mas;

    // считывание элементов
    
    uint64_t key;
    char str[2049];
    while (scanf("%lu\t%[^\n]", &key, str) != EOF) {
        mas.PushBack(TObject(key, std::string(str)));
    }

    

    // clock_t start = clock();
    // вызов функции сортировки
    Radix(mas);

    // clock_t end = clock();
    // double time = (double)(end - start) / CLOCKS_PER_SEC;
    // printf("%lf\n", time);

    // вывод
    for (int i = 0; i < mas.Size(); i++) {
        printf("%lu\t%s\n", mas[i].key, mas[i].value->c_str());
    }

}
