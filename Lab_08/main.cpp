#include <bits/stdc++.h>


// Функция для нахождения ранга матрицы
bool IsLinearIndependent(std::vector<std::vector<double>> &matr) {
    
    // ранг находим путем приведения матрицы к ступенчатому виду

    int rows = matr.size(); // кол-во строк матрицы
    int cols = matr[0].size() - 2; // кол-во столбцов матрицы, без цены и входного индекса
    int rank = 0; // ранг матрицы

    for (int col = 0; col < cols; col++) { // проходим по всем столбцам
        int current_row = -1;
        
        for (int row = rank; row < rows; row++) { // будем искать первую не нулевую строку в столбце
            if (matr[row][col] != 0) {
                current_row = row;
                break;
            }
        }

        if (current_row == -1) { // в столбце нет ненулевых элементов
            continue;
        }

        std::swap(matr[rank], matr[current_row]); // если нашли строку с ненулевым элементом в столбце, поднимаеем ее выше

        for (int row = 0; row < rows; row++) { // проходим по всем строкам матрицы
            if (row != rank) {
                double factor = matr[row][col] / matr[rank][col]; // вычисляем коэффициент

                for (int j = col; j < cols; j++) { // обнуляем элементы в столбце
                    matr[row][j] -= factor * matr[rank][j];
                }
            }
        }
        rank++;
    }
    
    return rank == cols; // проверяем, совпадает ли ранг с количсеством столбцов 
}


// компоратор, для сортировки матрицы
bool ComporatorCost(const std::vector<double> &a, const std::vector<double> &b) {
    return a[a.size() - 2] < b[b.size() - 2];
}


int main() {
    int m, n;
    std::cin >> m >> n;

    std::vector<std::vector<double>> matrix(m, std::vector<double>(n + 2)); // + 2 для сохранения цены и входного индекса

    for (int i = 0; i < m; i++) { // ввод матрицы
        for (int j = 0; j < n + 1; j++) {
            std::cin >> matrix[i][j];
        }
        matrix[i][n + 1] = i;
    }

    /*
        Идея --- сортировать по цене, после искать линейнонезависимые строки путем подсчета ранга

        Ранг должен быть равен количеству столбцов (веществ)
    */

    std::sort(matrix.begin(), matrix.end(), ComporatorCost); // сортировка матрицы по возрастанию

    if (IsLinearIndependent(matrix)) { // проверяем на линейную независимость, ранг = n?
        
        std::vector<int> res;
        for (int i = 0; i < n; i++) {
            res.push_back(matrix[i].back() + 1); // сохраняем входные индексы
        }
 
        std::sort(res.begin(), res.end()); // сортируем, чтобы они шли в порядке, в котором их ввели
        for (const int &elem : res) { // выводим
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << -1 << std::endl; // не удалось найти такую ЛНЗ систему
    }

    return 0;
}
