#include "sufftree.hpp"


TSuffixTree::TSuffixTree(std::string &str) {
    text = std::move(str);
    root = new TNode(nullptr, TERMPOS, new int(TERMPOS), TERMPOS); // корень это внутрення вершина, суфф индекс = -1
    last_inner_node = nullptr;
    current_node = nullptr;
    current_index = TERMPOS;
    jump_cnt = TERMVAL;
    plannedSiffixs = TERMVAL;
    sufftreeEnd = TERMPOS;

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
    if (node->suff_id == TERMPOS) { // проверим, есть ли указатели, которые мы не выделяли
        delete node->end;
    }
    delete node;
}


TSuffixTree::~TSuffixTree() {
    DeleteTree(root);
}


void TSuffixTree::SplitNode(TNode *next, int position) {
    // символа нет на дуге, надо создать внутреннюю вершину
    TNode *split_node = new TNode(root, next->start, new int(next->start + jump_cnt - 1), TERMPOS); // -1 для внутренней вершины
    current_node->childs[text[current_index]] = std::move(split_node); // добавим к текущей ноде нового потомка

    next->start += jump_cnt; // следующая вершина начинается после внутренне вершины

    // прикрепим листовые вершины к внутренней
    split_node->childs[text[position]] = std::move(new TNode(root, position, &sufftreeEnd, position)); // создаем новую листовую
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


TSuffixTree::TNode* TSuffixTree::FindChildNode(TNode *current, char s) {
    auto it = current->childs.find(s);
    if (it == current->childs.end()) { // такого нет
        return nullptr;
    }
    return it->second; // возвращаем найденный узел
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
        TNode *next = FindChildNode(current_node, text[current_index]);

        if (!next) { // если такого нет
            // std::cout << current_index << " " << position << " " << plannedSiffixs << std::endl;
            current_node->childs[text[current_index]] = std::move(new TNode(root, position, &sufftreeEnd, position));
        
            if (last_inner_node) { // если мы работаем не скорнем, то надо прокинуть ссылки для внутренних вершин
                last_inner_node->suff_link = current_node; // правило 2
                last_inner_node = nullptr;
            }
        } else { // символ есть
            int curve_len = LengthOnCurve(next);

            if (jump_cnt >= curve_len) { // можем ли перепрыгнуть всю дугу
                current_index += curve_len; // след символ
                jump_cnt -= curve_len;
                current_node = next; // след узел
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

            SplitNode(next, position); // делим узел если символа нет
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
    return *(node->end) - node->start + 1;
}


void TSuffixTree::MatchStatistic(std::vector<int> &ms, const std::string &str) {
    // Инициализируем вектор значениями 0
    ms.assign(str.size(), 0);

    // Проходим по всем символам текста
    for (size_t i = 0; i < str.size(); ++i) {
        TNode *currentNode = root; // Начинаем с корня
        size_t j = i; // Индекс в тексте

        // Пока есть символы для проверки
        while (j < str.size()) {
            // Проверяем, есть ли переход по текущему символу
            if (!currentNode) {
                currentNode = root;
            }

            if (currentNode->childs.find(str[j]) != currentNode->childs.end()) {
                TNode *next_node = currentNode->childs[str[j]];
                size_t curve_len = LengthOnCurve(next_node); // Длина дуги

                // Проверяем, сколько символов совпадает
                size_t k = 0;
                while (k < curve_len && j < str.size() && str[j] == text[next_node->start + k]) {
                    ms[i]++; // Увеличиваем счетчик совпадений
                    j++;
                    k++;
                }

                // Если мы вышли из цикла из-за несовпадения
                if (k < curve_len) {
                    // Здесь мы должны сделать переход по суффиксной ссылке
                    if (currentNode) {
                        currentNode = currentNode->suff_link; // Переход по суффиксной ссылке
                    }

                    // Пропускаем символы, которые были сравнены на предыдущей дуге
                    j = next_node->start + 1; // Устанавливаем j на следующий символ для сравнения
                    break;
                } else {
                    currentNode = next_node; // Переходим к следующему узлу
                }
            } else {
                // Если перехода нет, выходим из цикла
                break;
            }
        }
    }
}
