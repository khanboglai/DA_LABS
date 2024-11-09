#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


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
    std::cin >> n;
    TPair res = MinCost(n);
    std::cout << res.first << std::endl;
    
    for (const std::string & str : res.second) {
        std::cout << str << " ";
    }
    
    std::cout << std::endl;
    return 0;
}
