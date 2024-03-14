#include <iostream>
#include <vector>


/*
* Даны два отсортированных списка, нужно смерджить одинаковые элементы в один список
* без повторений.
* Ввод: l1=[1,2,5,5,6,10], l2=[2,5,8,10,20]
* Вывод: [2,5,10]
*/

std::vector<int32_t> merge(const std::vector<int32_t> &l1, const std::vector<int32_t> &l2)
{
    std::vector<int32_t> res;
    int idx_l1 = 0, idx_l2 = 0;
    while (idx_l1 < l1.size() && idx_l2 < l2.size()) {
        if (l1[idx_l1] < l2[idx_l2]) {
            idx_l1++;
        } else if (l2[idx_l2] < l1[idx_l1]) {
            idx_l2++;
        } else {
            res.push_back(l1[idx_l1]);
            idx_l1++;
            idx_l2++;
        }
    }
    return res;
}


int main() {
    std::vector<int32_t> l1 = {1, 2, 5, 5, 6, 10};
    std::vector<int32_t> l2 = {2, 5, 8, 10, 20};

    std::vector<int32_t> res = merge(l1, l2);

    for (size_t i = 0; i < res.size(); i++) {
        std::cout << res[i] << " ";
    }
    std::cout << std::endl;
}