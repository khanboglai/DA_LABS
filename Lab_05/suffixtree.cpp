#include "sufftree.hpp"


TSuffixTree::TNode::TNode(int start, int *finish, TNode *s_link, bool leaf) {
    begin = start;
    end = finish;
    suffix_link = s_link;
    is_leaf = leaf;
}


TSuffixTree::TSuffixTree(std::string &input_str) {
    str = std::move(input_str); // чтобы не копировать текст
    root = new TNode(-1, new int(-1), nullptr, false); // корень это внутренний узел
    suffixTreeEnd = -1;

    str += '$'; // чтобы правильно построить compact Trie
    CreateTree();
}


void TSuffixTree::DestroyTree(TNode *node) {
    if (!node) {
        return;
    }

    for (auto iter : node->child) {
        DestroyTree(iter.second);
    }

    if (!(node->is_leaf)) { // во внутренних узлах мы выделялм память под end
        delete node->end;
    }
    delete node;
}


TSuffixTree::~TSuffixTree() {
    DestroyTree(root);
}


void TSuffixTree::CreateTree() {
    params.current_index = -1;
    params.jump_counter = 0;
    params.plannedSuffixs = 0;
    params.current_node = root;

    for (int i = 0; i < int(str.length()); i++) {
        AddSuffix(i);
    }
}


int TSuffixTree::CurveLength(TNode *node) {
    return *(node->end) - node->begin + 1;
}


void TSuffixTree::AddSuffix(int position) {
    TNode *last_inner_node = nullptr; // последняя внутрення вершина, котору мы создали и которую мы не привязяли
    params.plannedSuffixs++;
    suffixTreeEnd++;

    while (params.plannedSuffixs) {
        if (!(params.jump_counter)) {
            params.current_index = position;
        }

        // ищем символ в дереве
        auto search_iter = params.current_node->child.find(str[params.current_index]);
        TNode *next_node = (search_iter != params.current_node->child.end()) ? search_iter->second : nullptr;

        if (!next_node) { // суффикса нет
            // создадим новый узел под него, это лист
            params.current_node->child[str[params.current_index]] = new TNode(position, &suffixTreeEnd, nullptr, true);

            // if (last_inner_node) {
            //     last_inner_node->suffix_link = current_node;
            //     last_inner_node = nullptr;
            // }
        } else { // суффикс есть

            // символ был вставлен ранее, ничего не делаем, правило 3
            if (str[position] == str[next_node->begin + params.jump_counter]) {
                params.jump_counter++; // увеличиваем прыжок
                break; // выходим
            }

            int curve_len = CurveLength(next_node);
            
            // проверим, можем ли перепрыгнуть на следующее ребро
            if (params.jump_counter >= curve_len) {
                params.current_index += curve_len; // текущий индекс ставим на следующее ребро
                params.jump_counter -= curve_len; // прыжок уменьшаем, мы прыгнули
                params.current_node = next_node; // начнем поиск по следующего узла
                continue;
            }

            // дошли сюда, значит надо разделить ребро
            // создаем внутреннюю вершину
            TNode *inner_node = new TNode(next_node->begin, new int(next_node->begin + params.jump_counter - 1), root, false);
            params.current_node->child[str[params.current_index]] = inner_node; // прикрепляем ее к дереву
            next_node->begin += params.jump_counter; // у нашего узла сдвигаем начало

            // создаем новый узел для суффикса и прикрепляем его к внутреннему узлу
            inner_node->child[str[position]] = new TNode(position, &suffixTreeEnd, nullptr, true);
            inner_node->child[str[next_node->begin]] = next_node; // прикрепляем наш узел к внутреннему узлу

            if (last_inner_node) { // есть неприкаяная внутренння вершина
                last_inner_node->suffix_link = inner_node; // прокидываем ссылку
            }
            last_inner_node = inner_node; // теперь новая внутрення вершина последняя
        }

        params.plannedSuffixs--; // суффикс добавлен

        if (params.current_node == root) { // в корне
            params.current_index++; // пошли проверять следующий суффикс
            if (params.jump_counter) {
                params.jump_counter--;
            }
        } else { // у нас есть еще ветки, надо посетить их
            params.current_node = params.current_node->suffix_link;
        }
    }
}


void TSuffixTree::MatchStatistic(std::vector<int> &ms, const std::string &text) {
    // инициализируем вектор значениями 0
    ms.assign(text.length(), 0);

    // проходим по всем символам текста
    for (size_t i = 0; i < text.size(); ++i) {
        TNode *currentNode = root; // начинаем с корня
        size_t j = i; // индекс в тексте
        size_t k = 0;

        // Пока есть символы для проверки
        while (j < text.length()) {
            // Проверяем, есть ли переход по текущему символу
            if (!currentNode) {
                currentNode = root;
            }

            if (currentNode->child.find(text[j]) != currentNode->child.end()) {
                TNode *next_node = currentNode->child[text[j]];
                size_t curve_len = CurveLength(next_node);

                // проверяем, сколько символов совпадает
                
                while (k < curve_len && j < text.length() && text[j] == str[next_node->begin + k]) {
                    ms[i]++; // увеличиваем счетчик совпадений
                    j++;
                    k++;
                }

                // если мы вышли из цикла из-за несовпадения
                if (k < curve_len) {
                    // здесь мы должны сделать переход по суффиксной ссылке
                    if (currentNode) {
                        currentNode = currentNode->suffix_link; // переход по суффиксной ссылке
                    }

                    // пропускаем символы, которые были сравнены на предыдущей дуге
                    j = ms[i] - 1; // устанавливаем j на следующий символ для сравнения
                    k = ms[i] - 1; // устанавливаем k на следующий символ для сравнения
                    break;
                } else {
                    currentNode = next_node; // переходим к следующему узлу
                    k = 0;
                }
            } else {
                // если перехода нет, выходим из цикла
                break;
            }
        }
    }
}
