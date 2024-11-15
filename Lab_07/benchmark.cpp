#include <bits/stdc++.h>


using namespace std;

// Функция для вычисления минимальной стоимости преобразования n в 1
long long minCost(int n) {
    // Если n уже равно 1, стоимость равна 0
    if (n == 1) return 0;

    // Начинаем с операции "вычесть 1"
    long long cost = n + minCost(n - 1);

    // Проверяем операцию "разделить на 2"
    if (n % 2 == 0) {
        cost = min(cost, n + minCost(n / 2));
    }

    // Проверяем операцию "разделить на 3"
    if (n % 3 == 0) {
        cost = min(cost, n + minCost(n / 3));
    }

    return cost;
}


using TPair = std::pair<int, std::vector<std::string>>;


std::vector<std::string> MakeSequence(int n, std::vector<std::string> &opers) {
    /*
        функция для восстановления последовательности действий
    */
    std::vector<std::string> seq; // итоговая последовательность

    int cur_num = n;

    while (cur_num > 1) {
        seq.push_back(opers[cur_num]);

        if (opers[cur_num] == "-1") {
            cur_num -= 1;
        } else if (opers[cur_num] == "/2") {
            cur_num /= 2;
        } else if (opers[cur_num] == "/3") {
            cur_num /= 3;
        }
    }
    return seq;
}


TPair MinCost(int n) {
    // вектор для сохранения минимальной стоимости преобразования чисел от 1 до n -> 1
    std::vector<long long> dp(n + 1, 0);
    std::vector<std::string> opers(n + 1, ""); // вектор для сохранения операций

    // применяем восходящий метод

    // можно и нисходящий метод, там будем использовать рекурсию
    for (int i = 2; i <= n; i++) {

        dp[i] = dp[i - 1] + i; // вычитаем 1, чтобы воспользоваться результатом предыдущих вычислений, +i --- стоимость
        opers[i] = "-1";

        // проверяем, делится ли число на 2 и сравниваем с текущим, чтобы выбрать выгодное (минимальное) значение
        if (i % 2 == 0 && dp[i] > dp[i / 2] + i) {
            dp[i] = dp[i / 2] + i;
            opers[i] = "/2";
        }

        // аналогично, только с 3
        if (i % 3 == 0 && dp[i] > dp[i / 3] + i) {
            dp[i] = dp[i / 3] + i;
            opers[i] = "/3";
        }
    }

    std::vector<std::string> seq = MakeSequence(n, opers);

    return std::make_pair(dp[n], seq);
}


int main() {
    int n;
    cin >> n;

    double start_naive, end_naive;
    double start, end;

    start_naive = clock();
    long long result = minCost(n);
    end_naive = clock();

    start = clock();
    TPair res = MinCost(n);
    end = clock();

    // std::cout << res.first << std::endl;
    double res_n = end_naive - start_naive;
    std::cout << "Naive: " << std::fixed << std::setprecision(3) << res_n / 1000.0 << " ms"<< std::endl;

    double res_f = end - start;
    std::cout << "DP: " << std::fixed << std::setprecision(3) << res_f / 1000.0 << " ms"<< std::endl;

    return 0;
}
