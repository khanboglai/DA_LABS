#include "sufftree.hpp"

std::string SENTINEL = "$";


TSuffixTree::TSuffixTree(std::string &str) {
    text = std::move(str);
    root = new TNode(nullptr, -1, new int(-1), -1); // корень это внутрення вершина, суфф индекс = -1
    last_inner_node = nullptr;
    current_node = nullptr;
    current_index = -1;
    jump_cnt = 0;
    plannedSiffixs = 0;
    sufftreeEnd = -1;

    text += SENTINEL;
    CreateTree();
}


void TSuffixTree::DeleteTree(TNode *node) {
    if (!node) { // если узел пустой, то выходим
        return;
    }

    for (auto it : node->childs) {
        DeleteTree(it.second);
    }
    if (node->suff_id == -1) { // проверим, есть ли указатели, которые мы не выделяли
        delete node->end;
    }
    delete node;
}


TSuffixTree::~TSuffixTree() {
    DeleteTree(root);
}


void TSuffixTree::SplitNode(TNode *next, int position) {
    // символа нет на дуге, надо создать внутреннюю вершину
    TNode *split_node = new TNode(root, next->start, new int(next->start + jump_cnt - 1), -1); // -1 для внутренней вершины
    current_node->childs[text[current_index]] = std::move(split_node); // добавим к текущей ноде нового потомка

    next->start += jump_cnt; // следующая вершина начинается после внутренне вершины

    // прикрепим листовые вершины к внутренней
    split_node->childs[text[position]] = std::move(new TNode(root, position, &sufftreeEnd, position - plannedSiffixs + 1)); // создаем новую листовую
    split_node->childs[text[next->start]] = std::move(next); // цепляем старую

    if (last_inner_node) {
        last_inner_node->suff_link = split_node; // цепляем к новой внутренней вершине суффиксную ссылку
    }
    last_inner_node = split_node; // последняя внутренняя вершина теперь точка разделения
}


void TSuffixTree::UpdateCurrentPos() {
    if (current_node == root && jump_cnt) {
        jump_cnt--;
        current_index++;
    } else if (current_node != root) {
        current_node = current_node->suff_link; // перемещение по суффиксной ссылке
    }
}


void TSuffixTree::AddSuffix(int position) {
    last_inner_node = nullptr;
    sufftreeEnd++; // правило 1, увеличиваем текст на дугах
    plannedSiffixs++; // планируемое число новых суффиксов

    while (plannedSiffixs) {
        if (!jump_cnt) { // надо искать из корня
            current_index = position;
        }

        // ищем текущий символ среди детей у нашего узла
        auto find = current_node->childs.find(text[current_index]);

        if (find == current_node->childs.end()) { // если такого нет
            current_node->childs[text[current_index]] = std::move(new TNode(root, position, &sufftreeEnd, position - plannedSiffixs + 1));
        
            if (last_inner_node) { // если мы работаем не скорнем, то надо прокинуть ссылки для внутренних вершин
                last_inner_node->suff_link = current_node; // правило 2
                last_inner_node = nullptr;
            }
        } else { // символ есть
            TNode *next = find->second; // идем по дуге
            int curve_len = LengthOnCurve(next);

            if (jump_cnt >= curve_len) {
                current_index += curve_len;
                jump_cnt -= curve_len;
                current_node = next;
                continue;
            }

            // если уже есть символ, по правилу 3 идем к следующему символу
            if (text[next->start + jump_cnt] == text[position]) {

                // проверка на внутренние вершины
                if (last_inner_node && current_node != root) { // here
                    last_inner_node->suff_link = current_node; // установка суффиксной ссылки
                }

                jump_cnt++; // увеличиваем шаг 
                break; // правило 3, стоп
            }

            SplitNode(next, position);
        }

        plannedSiffixs--; // осталось создать меньше суффиксов
        
        UpdateCurrentPos();
    }
}


void TSuffixTree::CreateTree() {
    current_node = root;
    for (size_t i = 0; i < text.length(); i++) {
        AddSuffix(i);
    }
}

int TSuffixTree::LengthOnCurve(TNode *node) {
    return *(node->end) - (node->start) + 1;
}


void TSuffixTree::MatchStatistic(std::vector<int> &ms, const std::string &str) {
    // Инициализируем вектор значениями 0
    ms.resize(str.size(), 0);

    // Проходим по всем символам текста
    for (size_t i = 0; i < str.size(); ++i) {
        TNode *currentNode = root; // Начинаем с корня
        size_t j = i; // Индекс в тексте

        // Пока есть символы для проверки
        while (j < str.size()) {
            // Проверяем, есть ли переход по текущему символу
            if (currentNode->childs.find(str[j]) != currentNode->childs.end()) {
                TNode *next_node = currentNode->childs[str[j]];
                size_t curve_len = LengthOnCurve(next_node); // Длина дуги

                // Проверяем, сколько символов совпадает
                for (size_t k = 0; k < curve_len && j < str.size(); ++k) {
                    if (str[j] == text[next_node->start + k]) {
                        ms[i]++; // Увеличиваем счетчик совпадений
                        j++;
                    } else {
                        break; // Если символы не совпадают, выходим из цикла
                    }
                }

                currentNode = next_node; // Переходим к следующему узлу
            } else {
                break; // Если перехода нет, выходим из цикла
            }
        }
    }
}
