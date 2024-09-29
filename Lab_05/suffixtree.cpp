#include "sufftree.hpp"

std::string SENTINEL = "$";


TSuffixTree::TSuffixTree(std::string &str) : text(str) {
    root = new TNode(nullptr, -1, new int(-1));
    last_inner_node = nullptr;
    growth_node = nullptr;
    growth_index = -1;
    growth_step = 0;
    CountPlannedSuffix = 0;
    SuffTreeEnd = -1;

    text += SENTINEL;
    CreateTree();
}


void TSuffixTree::DeleteTree(TNode *node) {
    for (auto it : node->childs)
        DeleteTree(it.second);
    if (node->suff_id == -1)
        delete node->end;
    delete node;
}


TSuffixTree::~TSuffixTree() {
    DeleteTree(root);
}


void TSuffixTree::GrowthTree(int pos) {
    last_inner_node = nullptr;
    SuffTreeEnd++; // правило 1, увеличиваем текст на дугах
    CountPlannedSuffix++; // планируемое число новых суффиксов

    while (CountPlannedSuffix) {
        if (growth_step == 0) { // надо искать из корня
            growth_index = pos;
        }

        // ищем текущий символ среди детей у нашего узла
        auto find = growth_node->childs.find(text[growth_index]);

        if (find == growth_node->childs.end()) { // если такого нет
            growth_node->childs.insert(std::make_pair(text[growth_index], new TNode(root, pos, &SuffTreeEnd, pos - CountPlannedSuffix + 1))); // добавим новую дугу, идущую в лист

            if (last_inner_node != nullptr) { // если мы работаем не скорнем, то надо прокинуть ссылки для внутренних вершин
                last_inner_node->suff_link = growth_node; // правило 2
                last_inner_node = nullptr;
            }
        } else { // символ есть
            TNode *next = find->second; // идем по дуге
            int edge_length = LengthOnCurve(next);

            if (growth_step >= edge_length) {
                growth_index += edge_length;
                growth_step -= edge_length;
                growth_node = next;
                continue;
            }


            // если уже есть символ, по правилу 3 идем к следующему символу
            if (text[next->start + growth_step] == text[pos]) {

                // проверка на внутренние вершины
                if (last_inner_node != nullptr && growth_node != root) { // && growth_node != root
                    last_inner_node->suff_link = growth_node; // установка суффиксной ссылки
                }

                // if (last_inner_node != nullptr && text[pos] == *(SENTINEL.c_str())) {
                //     last_inner_node->suff_link = root; // Установить ссылку на корень
                // }
                
                growth_step++; // увеличиваем шаг 
                break; // правило 3, стоп
            }

            // символа нет на дуге, надо создать внутр вершину
            TNode *split = new TNode(root, next->start, new int(next->start + growth_step - 1));
            growth_node->childs[text[growth_index]] = split; // добавим к текущей ноде нового потомка

            next->start += growth_step; // следующая вершина начинается после внутренне вершины

            // прикрепим листовые вершины к внутренней
            split->childs.insert(std::make_pair(text[pos], new TNode(root, pos, &SuffTreeEnd, pos - CountPlannedSuffix + 1))); // создаем новую листовую
            split->childs.insert(std::make_pair(text[next->start], next)); // цепляем старую

            if (last_inner_node != nullptr) {
                last_inner_node->suff_link = split; // цепляем к новой внутренней вершине суффиксную ссылку
            }
            last_inner_node = split; // последняя внутреннея вершина 
        }

        CountPlannedSuffix--; // осталось создать меньше суффиксов

        if (growth_node == root && growth_step > 0) {
            growth_step--;
            growth_index++;
        } else if (growth_node != root) { // перемещение по суффиксной ссылке
            growth_node = growth_node->suff_link;
        }
    }
}


void TSuffixTree::CreateTree() {
    growth_node = root;
    for (size_t i = 0; i < text.length(); i++) {
        GrowthTree(i);
    }
}

int TSuffixTree::LengthOnCurve(TNode *node) {
    return *(node->end) - (node->start) + 1;
}


void TSuffixTree::MatchStatistic(std::vector<int> &ms, const std::string &str) {
    // Инициализируем вектор значениями 0
    ms.assign(str.size(), 0);

    // Проходим по всем символам текста
    for (size_t i = 0; i < str.size(); ++i) {
        TNode *current_node = root; // Начинаем с корня
        size_t j = i; // Индекс в тексте

        // Пока есть символы для проверки
        while (j < str.size()) {
            // Проверяем, есть ли переход по текущему символу
            if (current_node->childs.find(str[j]) != current_node->childs.end()) {
                TNode *next_node = current_node->childs[str[j]];
                size_t edge_length = LengthOnCurve(next_node); // Длина дуги

                // Проверяем, сколько символов совпадает
                for (size_t k = 0; k < edge_length && j < str.size(); ++k) {
                    if (str[j] == text[next_node->start + k]) {
                        ms[i]++; // Увеличиваем счетчик совпадений
                        j++;
                    } else {
                        break; // Если символы не совпадают, выходим из цикла
                    }
                }

                current_node = next_node; // Переходим к следующему узлу
            } else {
                break; // Если перехода нет, выходим из цикла
            }
        }
    }
}
