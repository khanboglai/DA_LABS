#include <iostream>
#include <vector>
#include <algorithm>


bool IsLinearIndependent(std::vector<std::vector<double>> &matr) {
    int rows = matr.size();
    int cols = matr[0].size() - 2;
    int rank = 0;

    for (int col = 0; col < cols; col++) {
        int current_row = -1;
        
        for (int row = col; row < rows; row++) {
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

                for (int j = col; j < rows; j++) {
                    matr[row][j] -= factor * matr[rank][j];
                }
            }
        }
        rank++;
    }
    // std::cout << rows << " " << rank << std::endl;
    return rank == rows;
}


bool Comparator(const std::vector<double>& a, const std::vector<double>& b) {
    return a.back() - 1 < b.back() - 1;
}


int main() {
    int m, n;
    std::cin >> m >> n;

    if (m < n) {
        std::cout << -1 << std::endl;
        return 0;
    }
    
    std::vector<std::vector<double>> matr(m, std::vector<double>(n + 2));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n + 1; j++) {
            std::cin >> matr[i][j];
        }
        matr[i][n + 1] = i;
    }


    std::sort(matr.begin(), matr.end(), Comparator);

    std::vector<std::vector<double>> check;

    check.push_back(matr[0]);

    for (int i = 1; i < m; i++) {
        check.push_back(matr[i]);

        if (!IsLinearIndependent(check)) {
            // check.pop_back();
            std::cout << -1 << std::endl;
            return 0;
        }
    }

    int LI = check.size();
    // std::cout << LI << std::endl;

    if (LI < n) {
        std::cout << -1 << std::endl;
    } else {
        
        std::sort(check.begin(), check.end(), Comparator);

        for (auto &elem : check) {
            std::cout << elem.back() + 1 << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
