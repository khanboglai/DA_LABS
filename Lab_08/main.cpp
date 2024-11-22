#include <bits/stdc++.h>

// Функция для нахождения ранга матрицы
int GetRank(std::vector<std::vector<double>> &matr) {
    int rows = matr.size();
    int cols = matr[0].size() - 2;
    int rank = 0;

    for (int col = 0; col < cols; col++) {
        int current_row = -1;
        for (int row = rank; row < rows; row++) {
            if (matr[row][col] != 0) {
                current_row = row;
                break;
            }
        }

        if (current_row == -1) {
            continue;
        }

        std::swap(matr[rank], matr[current_row]);

        for (int row = 0; row < rows; row++) {
            if (row != rank) {
                double factor = matr[row][col] / matr[rank][col];
                for (int j = col; j < cols; j++) {
                    matr[row][j] -= factor * matr[rank][j];
                }
            }
        }
        rank++;
    }
    
    return rank;
}

bool ComporatorCost(const std::vector<double> &a, const std::vector<double> &b) {
    return a[a.size() - 2] < b[b.size() - 2];
}

int main() {
    int m, n;
    std::cin >> m >> n;

    std::vector<std::vector<double>> matrix(m, std::vector<double>(n + 2)); // + 2 для сохранения цены и входного индекса

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n + 1; j++) {
            std::cin >> matrix[i][j];
        }
        matrix[i][n + 1] = i; // сохраняем индекс добавки
    }

    std::sort(matrix.begin(), matrix.end(), ComporatorCost); // сортировка по цене

    std::vector<int> selected_indices;
    std::vector<std::vector<double>> current_matrix;

    for (const auto &additive : matrix) {
        current_matrix.push_back(additive);
        int rank = GetRank(current_matrix);
        
        if (rank == int(selected_indices.size()) + 1) { // если ранг увеличился
            selected_indices.push_back(additive.back() + 1); // сохраняем индекс добавки
        }
        
        if (int(selected_indices.size()) == n) { // если нашли n линейно независимых добавок
            break;
        }
    }

    if (int(selected_indices.size()) == n) {
        std::sort(selected_indices.begin(), selected_indices.end()); // сортируем индексы
        for (const int &index : selected_indices) {
            std::cout << index << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << -1 << std::endl; // не удалось найти линейно независимые добавки
    }

    return 0;
}
