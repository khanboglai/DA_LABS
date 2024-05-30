#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>


const int SEP = -1; // разделитель
using TDigitSRH = std::pair<int64_t, std::pair<size_t, size_t>>; // псевдоним, чтобы меньше писать


std::vector<int> prefix_function(const std::vector<TDigitSRH>& s) {
    std:: vector<int> pi(s.size(), 0);

    for (int i = 1; i < (int)s.size(); i++) {
        int j = pi[i - 1];

        while (j > 0 && s[i].first != s[j].first) {
            j = pi[j - 1];
        }

        if (s[i].first == s[j].first) {
            pi[i] = j + 1;
        } else {
            pi[i] = j;
        }
    }

    return pi;
}



std::vector<int> Z_func_naive(const std::vector<TDigitSRH>& S) {
    int n = S.size();
    std::vector<int> Z(n, 0);

    for (int i = 1; i < n; i++) {
        while (i + Z[i] < n && S[Z[i]].first == S[i + Z[i]].first) { // это стандартный подсчет z-функции
            Z[i]++;
        }
    }

    return Z;
}



std::vector<int> Z_func(const std::vector<TDigitSRH>& S) {
    int n = S.size();
    int l = 0, r = 0;
    std::vector<int> Z(n);

    for (int i = 1; i < n; i++) {
        
        Z[i] = std::max(0, std::min(r - i, Z[i - l]));

        while (i + Z[i] < n && S[Z[i]].first == S[i + Z[i]].first) { // это стандартный подсчет z-функции
            Z[i]++;
        }
        
        if (i + Z[i] > r) {
            l = i;
            r = i + Z[i];
        }
    }

    return Z;
}


int main() {

    long double Z_naive = 0;
    long double Z_eff = 0;
    long double KMP = 0;
    long double start, end;

    std::string P;
    getline(std::cin, P);
    std::vector<TDigitSRH> vec_p;
    
    // подготовка паттерна
    std::istringstream ss(P);
    int64_t number = 0;
    size_t cnt_n = 0;
    while (ss >> number) {
        vec_p.push_back(std::make_pair(number, std::make_pair(cnt_n, 1)));
        cnt_n++;
    }

    //int t_len = vec_p.size();
    TDigitSRH sep_elem = std::make_pair(SEP, std::make_pair(vec_p.size(), 1));

    vec_p.push_back(sep_elem);

    // подготовка текста
    std::string T;
    size_t cnt_line = 1;
    
    while (getline(std::cin, T)) {
        
        std::istringstream ss(T);
        int64_t num = 0;
        size_t cnt_num = 0;
        while (ss >> num) {
            vec_p.push_back(std::make_pair(num, std::make_pair(cnt_num, cnt_line)));
            cnt_num++; // номер слова
        }
        cnt_line++; // номер строки
    }


    start = clock();
    std::vector<int> Z = Z_func(vec_p);
    end = clock();
    Z_eff = Z_eff + end - start;

    start = clock();
    std::vector<int> Z_n = Z_func_naive(vec_p);
    end = clock();
    Z_naive = Z_naive + end - start;


    start = clock();
    std::vector<int> pi = prefix_function(vec_p);
    end = clock();
    KMP = KMP + end - start;

    // for (int i = 0; i < (int)vec_p.size() - t_len - 1; i++) {
    //     if (pi[t_len + 1 + i] == t_len) {
    //         std::cout << "s[" << i - t_len + 1 << ".." << i << "] = t" << std::endl;
    //     }
    // }


    std::cout << "Z naive: " << std::fixed << std::setprecision(3) << Z_naive / 1000.0 << " ms"<< std::endl;
    std::cout << "Z effective: " << std::fixed << std::setprecision(3) << Z_eff / 1000.0 << " ms" << std::endl;
    std::cout << "KMP: " << std::fixed << std::setprecision(3) << KMP / 1000.0 << " ms" << std::endl;

    // std::vector<int> Z_naive_r;
    // std::vector<int> Z_r;
    // for (size_t i = p_sz + 1; i < Z.size(); i++) {
    //     if (Z[i] == p_sz) {
    //         Z_naive_r.push_back(vec_p[i].second.first + 1);
    //         Z_r.push_back(vec_p[i].second.first + 1);
    //     }
    // }

    // for (size_t i = 0; i < Z_r.size(); i++) {
    //     std::cout << Z_r[i] << " " << Z_naive_r[i] << " " << kmp_res[i] << std::endl;
    // }
}
