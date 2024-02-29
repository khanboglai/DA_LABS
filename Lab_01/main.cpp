#include <iostream>
#include "simple_vector.hpp"
#include "object.hpp"


// объявил свой тип, чтобы не писать много
typedef unsigned long long Tull;


// получаю разряд числа, двигаюсь по байтам
Tull GetDigit(Tull elem, int i) {
    Tull digit = (elem >> (8 * i)) & 0xFF;
    return digit;
}


// алгоритм поразрядной сортировки
void Radix(TSimpleVector<TObject> & mas) {

    size_t sz = mas.Size();

    // 8 байт под мой тип данных
    for (Tull i = 0; i < 8; i++) {
        
        // поиск маскимального разряда
        Tull max_elem = 0;
        for (size_t j = 0; j < sz; j++) {
            Tull digit = GetDigit(mas[j].key, i);
            if (max_elem < digit) {
                max_elem = digit;
            }
        }

        // создание вектора для подсчета элементов
        size_t cnt_sz = max_elem + 1;
        TSimpleVector<Tull> cnts(cnt_sz, 0);

        // подсчет элементов
        for (size_t j = 0; j < sz; j++) {
            cnts[GetDigit(mas[j].key, i)]++;
        }


        // подсчет префикс-суммы. Сколько элементов >= текущему стоит находится впереди
        for (size_t j = 1; j < cnt_sz; j++) {
            cnts[j] += cnts[j - 1];
        }

        // блок сбора отсортированной части
        TSimpleVector<TObject> interm_result(sz);

        // идем с конца
        for (int j = sz - 1; j >= 0; j--) {

            // вычисляем позицию
            Tull pos = cnts[GetDigit(mas[j].key, i)] - 1;
            // сохраняем результат
            interm_result[pos] = std::move(mas[j]);
            // уменьшаем значение префиксной-суммы
            cnts[GetDigit(mas[j].key, i)] = pos;
        }

        mas = std::move(interm_result);
    }
}


int main() {
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
