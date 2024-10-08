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
    params.last_inner_node = nullptr;

    for (int i = 0; i < int(str.length()); i++) {
        AddSuffix(i);
    }
}


int TSuffixTree::CurveLength(TNode *node) {
    return *(node->end) - node->begin + 1;
}


void TSuffixTree::SplitCurve(TNode *node, int position) {
    // создаем внутреннюю вершину
    TNode *inner_node = new TNode(node->begin, new int(node->begin + params.jump_counter - 1), root, !ISLEAF);
    params.current_node->child[str[params.current_index]] = inner_node; // прикрепляем ее к дереву
    
    node->begin += params.jump_counter; // у нашего узла сдвигаем начало

    // создаем новый узел для суффикса и прикрепляем его к внутреннему узлу
    inner_node->child[str[position]] = new TNode(position, &suffixTreeEnd, nullptr, ISLEAF);
    inner_node->child[str[node->begin]] = node; // прикрепляем наш узел к внутреннему узлу

    if (params.last_inner_node) { // есть неприкаяная внутренння вершина
        // std::cout << "here" << std::endl;
        params.last_inner_node->suffix_link = inner_node; // прокидываем ссылку
    }
    params.last_inner_node = inner_node; // теперь новая внутрення вершина последняя
}


void TSuffixTree::AddSuffix(int position) {
    params.last_inner_node = nullptr;
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
            SplitCurve(next_node, position);
        }

        if (params.current_node == root) { // в корне
            params.current_index++; // пошли проверять следующий суффикс
            if (params.jump_counter) {
                params.jump_counter--;
            }
            params.plannedSuffixs--; // суффикс добавлен
        } else { // у нас есть еще ветки, надо посетить их
            params.current_node = params.current_node->suffix_link;
        }
    }
}


void TSuffixTree::MatchStatistic(std::vector<int> &ms, const std::string &text) {
    // инициализируем вектор значениями 0
    size_t len_t = text.length();
    ms.assign(len_t, 0);

    // проходим по всем символам текста
    for (size_t i = 0; i < len_t; ++i) {
        TNode *current_node = root; // начинаем с корня
        size_t j = i; // индекс в тексте
        size_t skip_equal = 0;

        while (j < len_t) {

            if (!current_node) {
                current_node = root;
            }

            if (current_node->child.find(text[j]) != current_node->child.end()) { // поиск узла
                TNode *next_node = current_node->child[text[j]];
                size_t curve_len = CurveLength(next_node);

                // проверяем, сколько символов совпадает
                while (skip_equal < curve_len && j < len_t && text[j] == str[next_node->begin + skip_equal]) {
                    ms[i]++; // увеличиваем счетчик совпадений
                    j++;
                    skip_equal++;
                }

                // если мы вышли из цикла из-за несовпадения
                if (skip_equal < curve_len) {
                    current_node = current_node->suffix_link; // переход по суффиксной ссылке
                    
                    break;
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
