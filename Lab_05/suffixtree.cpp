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


void TSuffixTree::CollectIdx(TNode *node, std::vector<int> &vec) {
    if (!node) {
        return;
    }

    for (auto it : node->childs) {
        CollectIdx(it.second, vec);
    }
    if (node->suff_id != -1) {
        vec.push_back(node->suff_id);
    }
}


void TSuffixTree::searchPattern(const std::string& pattern, std::vector<int>& indices) {

    TNode* currentNode = root; // Начинаем с корневого узла
    int currentIndex = 0; // Индекс текущего символа в шаблоне
    int patternLength = pattern.length();

    while (currentIndex < patternLength) {
        char currentChar = pattern[currentIndex];

        // Проверяем наличие символа в дочерних узлах
        if (currentNode->childs.find(currentChar) == currentNode->childs.end()) {
            // Символ не найден, выходим из метода
            return;
        }

        // Переходим к дочернему узлу
        TNode* childNode = currentNode->childs[currentChar];
        int edgeLength = LengthOnCurve(childNode); // Длина ребра
        int edgeStart = childNode->start; // Начало ребра

        // Проходим по ребру
        for (int i = 0; i < edgeLength && currentIndex < patternLength; i++) {
            if (text[edgeStart + i] != pattern[currentIndex]) {
                // Если символы не совпадают, выходим из метода
                return;
            }
            currentIndex++;
        }

        // Если мы прошли все символы на ребре, переходим к дочернему узлу
        currentNode = childNode;
    }

    // Если мы дошли до конца шаблона, собираем индексы суффиксов
    CollectIdx(currentNode, indices);

}

int TSuffixTree::LengthOnCurve(TNode *node) {
    return *(node->end) - (node->start) + 1;
}


void TSuffixTree::PrintEdges(TSuffixTree::TNode *node, int depth) {
    
    for (const auto& child : node->childs) {
        TNode* childNode = child.second;

        // Индексы левой и правой границы строки на ребре
        int leftIndex = childNode->start;
        int rightIndex = *(childNode->end); // Используем указатель на конец

        std::cout << depth << " " << leftIndex + 1 << " " << rightIndex + 1 << std::endl;        

        // Рекурсивно обходим дочерний узел
        PrintEdges(childNode, depth + 1);
    }
}


void TSuffixTree::PrintEdgesInOrder() {
    PrintEdges(root, 0);
}


void TSuffixTree::MatchStatistic(const std::string &str, std::vector<int> &ms) {
    TNode *current_node = root; // начинаю от корня
    int current_index = 0; // начало текста
    int str_len = str.length(); // длина текста
    int cnt_syms = 0; // сколько мы прошли
    int skip_sysm = 0; 
    TNode* before_node = nullptr;

    while (current_index < str_len) {
        auto search = current_node->childs.find(str[current_index]);

        if (search == current_node->childs.end()) {
            current_index++;
            continue;
        }

        TNode *child = search->second;

        if (!child) {
            break;
        }

        for (int i = child->start; i <= *(child->end); i++) {
            if (str[current_index] == text[i]) {
                cnt_syms++;
            } else {
                ms.push_back(cnt_syms);
                cnt_syms--;
                break;
            }
            current_index++;
        }

        current_node = child;
    }
}
