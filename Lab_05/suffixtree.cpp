#include "sufftree.hpp"


TSuffixTree::TNode::TNode(int start, int *finish, TNode *s_link, bool leaf) {
    begin = start;
    end = finish;
    suffix_link = s_link;
    is_leaf = leaf;
}


TSuffixTree::TSuffixTree(std::string &input_str) {
    str = std::move(input_str); // чтобы не копировать текст
    root = new TNode(TERMPOS, new int(TERMPOS), nullptr, !ISLEAF); // корень это внутренний узел
    suffixTreeEnd = TERMPOS;

    str += SENTINEL; // чтобы правильно построить compact Trie
    CreateTree();
}


void TSuffixTree::DestroyTree(TNode *node) {
    if (!node) {
        return;
    }

    if (!(node->is_leaf)) { // во внутренних узлах мы выделялм память под end
        delete node->end;
    }

    for (auto iter : node->child) {
        DestroyTree(iter.second);
    }

    delete node;
}


TSuffixTree::~TSuffixTree() {
    DestroyTree(root);
}


void TSuffixTree::CreateTree() {
    params.current_index = TERMPOS;
    params.jump_counter = TERMVAL;
    params.plannedSuffixs = TERMVAL;
    params.current_node = root;

    for (int i = 0; i < int(str.length()); i++) {
        AddSuffix(i);
    }
}


int TSuffixTree::CurveLength(TNode *node) {
    return *(node->end) - node->begin + 1;
}


void TSuffixTree::SplitNode(TNode *node, int position, TNode *last_inner_node) {
    // создаем внутреннюю вершину
    TNode *inner_node = new TNode(node->begin, new int(node->begin + params.jump_counter - 1), root, !ISLEAF);
    params.current_node->child[str[params.current_index]] = inner_node; // прикрепляем ее к дереву
    
    node->begin += params.jump_counter; // у нашего узла сдвигаем начало

    // создаем новый узел для суффикса и прикрепляем его к внутреннему узлу
    inner_node->child[str[position]] = new TNode(position, &suffixTreeEnd, nullptr, ISLEAF);
    inner_node->child[str[node->begin]] = node; // прикрепляем наш узел к внутреннему узлу

    if (last_inner_node) { // есть неприкаяная внутренння вершина
        last_inner_node->suffix_link = inner_node; // прокидываем ссылку
    }
    last_inner_node = inner_node; // теперь новая внутрення вершина последняя
}


void TSuffixTree::AddSuffix(int position) {
    TNode *last_inner_node = nullptr; // последняя внутрення вершина, котору мы создали и которую мы не привязяли
    params.plannedSuffixs++;
    suffixTreeEnd++;

    while (params.plannedSuffixs) {
        if (!(params.jump_counter)) { // прыгать некуда, надо пойти честно с заданной позиции
            params.current_index = position;
        }

        // ищем символ в дереве
        auto search_iter = params.current_node->child.find(str[params.current_index]);
        TNode *next_node = (search_iter != params.current_node->child.end()) ? search_iter->second : nullptr;

        if (!next_node) { // суффикса нет
            // создадим новый узел под него, это лист
            params.current_node->child[str[params.current_index]] = new TNode(position, &suffixTreeEnd, nullptr, ISLEAF);
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
            SplitNode(next_node, position, last_inner_node);
        }

        if (params.current_node == root) { // в корне
            params.current_index++; // пошли проверять следующий суффикс
            if (params.jump_counter) {
                params.jump_counter--;
            }
            params.plannedSuffixs--; // суффикс добавлен
        } else { // у нас есть еще ветки, надо посетить их
            params.current_node = params.current_node->suffix_link;
            continue; // надо пойти к другим суффиксам и там тоже разделить
        }
    }
}


void TSuffixTree::MatchStatistic(std::vector<int> &ms, const std::string &text) {
    // инициализируем вектор значениями 0
    ms.assign(text.length(), 0);

    // проходим по всем символам текста
    for (size_t i = 0; i < text.size(); ++i) {
        TNode *current_node = root; // начинаем с корня
        size_t j = i; // индекс в тексте
        size_t skip_equal = 0;

        while (j < text.length()) {

            if (!current_node) {
                current_node = root;
            }

            if (current_node->child.find(text[j]) != current_node->child.end()) { // поиск узла
                TNode *next_node = current_node->child[text[j]];
                size_t curve_len = CurveLength(next_node);

                j += skip_equal; // после поиска надо пропустить символы, если это возможно

                // проверяем, сколько символов совпадает
                while (skip_equal < curve_len && j < text.length() && text[j] == str[next_node->begin + skip_equal]) {
                    ms[i]++; // увеличиваем счетчик совпадений
                    j++;
                    skip_equal++;
                }

                // если мы вышли из цикла из-за несовпадения
                if (skip_equal < curve_len) {
                    if (current_node) {
                        current_node = current_node->suffix_link; // переход по суффиксной ссылке
                    }

                    skip_equal = ms[i] - 1; // устанавливаем skip_equal на следующий символ для сравнения
                    i++; // переход на следующий элемент текста

                    if (i < text.length()) { // при переходе по суффиксной ссылке у нас уже есть совпавшие символы
                        ms[i] = skip_equal; // запишем сразу число совпавших символов
                    }

                    j = i; // надо начать поиск по следующего символа текста
                } else {
                    current_node = next_node; // переходим к следующему узлу
                    skip_equal = 0; // при переходе надо обновить прыжок
                }
            } else {
                break; // если перехода нет, выходим из цикла
            }
        }
    }
}
