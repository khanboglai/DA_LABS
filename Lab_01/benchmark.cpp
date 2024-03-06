#include <iostream>
#include "simple_vector.hpp"
#include "object.hpp"
#include <algorithm>

// константы
const int COUNT_MS_SZ = 256;


// получаю разряд числа, двигаюсь по байтам
int GetDigit(uint64_t & elem, int& i) {
    int digit = (elem >> (8 * i)) & 0xFF;
    return digit;
}


void CountingSort(std::vector<TObject>& mas, int& i) {
    int sz = mas.size();

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
    std::vector<TObject> interm_result(sz);

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
void Radix(std::vector<TObject>& mas) {
    
    // 8 байт под тип данных uint64_t
    for (int i = 0; i < 8; i++) {
        CountingSort(mas, i);
    }
}


bool comp(TObject o1, TObject o2) {
    return o1.key < o2.key;
}

int main() {

    /*
    оказалось, что ввод и вывод быстрее, если использовать фукции из Си,
    а отключение синхронизации вообще не помогает

    вообще я ощущал себя самым экономным рабочим, экономия на всем абсолютно. кризиссс...
    */

    std::vector<TObject> mas;
    std::vector<TObject> stl_mas;

    // считывание элементов
    uint64_t key;
    char str[2049];
    while (scanf("%lu\t%[^\n]", &key, str) != EOF) {
        mas.push_back(TObject(key, std::string(str)));
        stl_mas.push_back(TObject(key, std::string(str)));
    }

    std::cout << "Count lines is: " << mas.size() << std::endl;

    clock_t start_r = clock();
    Radix(mas);
    clock_t end_r = clock();
    double radix = 1000.0 * (double)(end_r - start_r) / CLOCKS_PER_SEC;

    clock_t start_stl = clock();
    std::stable_sort(stl_mas.begin(), stl_mas.end(), comp);
    clock_t end_stl = clock();
    double sort = 1000.0 * (double)(end_stl - start_stl) / CLOCKS_PER_SEC;


    std::cout << "Radix_sort time: " << radix << " ms" << std::endl;
    std::cout << "Stable_sort time: " << sort << " ms" << std::endl;
    std::cout << "Difference: " << (double)(sort / radix) << std::endl;

}
