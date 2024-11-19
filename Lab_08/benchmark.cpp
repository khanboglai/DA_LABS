#include <bits/stdc++.h>

using namespace std;

// Функция для проверки линейной независимости
bool isLinearlyIndependent(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    int m = matrix[0].size();
    
    // Приведение матрицы к ступенчатому виду
    vector<vector<int>> mat = matrix;
    for (int col = 0; col < m; ++col) {
        // Поиск строки с ненулевым элементом в текущем столбце
        int pivot = -1;
        for (int row = col; row < n; ++row) {
            if (mat[row][col] != 0) {
                pivot = row;
                break;
            }
        }
        if (pivot == -1) continue; // Ничего не найдено, переходим к следующему столбцу

        // Меняем местами текущую строку и строку с ненулевым элементом
        swap(mat[col], mat[pivot]);

        // Обнуляем элементы ниже текущего пивота
        for (int row = col + 1; row < n; ++row) {
            if (mat[row][col] != 0) {
                int factor = mat[row][col] / mat[col][col];
                for (int c = col; c < m; ++c) {
                    mat[row][c] -= factor * mat[col][c];
                }
            }
        }
    }

    // Проверяем количество ненулевых строк
    int rank = 0;
    for (int i = 0; i < n; ++i) {
        if (any_of(mat[i].begin(), mat[i].end(), [](int x) { return x != 0; })) {
            ++rank;
        }
    }

    return rank == m; // Линейно независимы, если ранг равен количеству столбцов
}

int main() {
    int M, N;
    cin >> M >> N;

    vector<vector<int>> additives(M, vector<int>(N + 1)); // +1 для цены
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N + 1; ++j) {
            cin >> additives[i][j];
        }
    }

    int minCost = 51;
    vector<int> bestCombination;

    double start, end;

    start = clock();

    // Перебор всех комбинаций добавок
    for (int mask = 0; mask < (1 << M); ++mask) {
        vector<vector<int>> selected;
        int cost = 0;

        for (int i = 0; i < M; ++i) {
            if (mask & (1 << i)) {
                selected.push_back(vector<int>(additives[i].begin(), additives[i].end() - 1));
                cost += additives[i][N];
            }
        }

        if (selected.size() == N && isLinearlyIndependent(selected)) {
            if (cost < minCost) {
                minCost = cost;
                bestCombination.clear();
                for (int i = 0; i < M; ++i) {
                    if (mask & (1 << i)) {
                        bestCombination.push_back(i + 1); // +1 для 1-индексации
                    }
                }
            }
        }
    }

    end = clock();

    double res = end - start;
    std::cout << "Naive: " << std::fixed << std::setprecision(3) << res / 1000.0 << " ms"<< std::endl;

    return 0;
}
