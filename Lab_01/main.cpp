#include <iostream>
#include "simple_vector.hpp"
#include "object.hpp"


// константы
const int COUNT_MS_SZ = 256;
const int MAX_STR_SIZE = 2049;
const int BYTE_VAL = 8;


// получаю разряд числа, двигаюсь по байтам
int GetDigit(uint64_t & elem, int& i) {
    int digit = (elem >> (BYTE_VAL * i)) & 0xFF;
    return digit;
}


void CountingSort(TSimpleVector<TObject>& mas, int& i) {
    int sz = mas.Size();

    // создание вектора для подсчета элементов

    /*
    чтобы не ругалось на переменную длину
    больше 255 я не получу значение, это я точно знаю
    */
    int cnts[COUNT_MS_SZ] = {0};

    // подсчет элементов
    for (int j = 0; j < sz; j++) {
        cnts[GetDigit(mas[j].key, i)]++;
    }


    // подсчет префикс-суммы. Сколько элементов >= текущему стоит находится впереди
    for (int j = 1; j < COUNT_MS_SZ; j++) {
        cnts[j] += cnts[j - 1];
    }

    // блок сбора отсортированной части, сделал на векторе
    TSimpleVector<TObject> interm_result(sz);

    // идем с конца
    for (int j = sz - 1; j >= 0; j--) {

        // вычисляем позицию
        int pos = cnts[GetDigit(mas[j].key, i)] - 1;
        // сохраняем результат
        interm_result[pos] = mas[j];
        // уменьшаем значение префиксной-суммы
        cnts[GetDigit(mas[j].key, i)] = pos;
    }

    // по памяти улучшения есть
    mas = std::move(interm_result);
}


// алгоритм поразрядной сортировки
void Radix(TSimpleVector<TObject>& mas) {
    
    // 8 байт под тип данных uint64_t
    for (int i = 0; i < (int)sizeof(uint64_t); i++) {
        CountingSort(mas, i);
    }
}


int main() {

    /*
    оказалось, что ввод и вывод быстрее, если использовать фукции из Си,
    а отключение синхронизации вообще не помогает

    вообще я ощущал себя самым экономным рабочим, экономия на всем абсолютно. кризиссс...
    */

    TSimpleVector<TObject> mas;

    // считывание элементов
    uint64_t key;
    char str[MAX_STR_SIZE];
    while (scanf("%lu\t%[^\n]", &key, str) != EOF) {
        mas.PushBack(TObject(key, std::string(str)));
    }

    // вызов функции сортировки
    Radix(mas);


    // вывод
    for (int i = 0; i < mas.Size(); i++) {
        printf("%lu\t%s\n", mas[i].key, mas[i].value->c_str());
    }

}
