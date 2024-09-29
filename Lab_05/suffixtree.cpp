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
    ms.resize(str.length(), 0);

    TNode *current_node = root;
    size_t current_id = 0;
    size_t skip_syms = 0;
    size_t cnt_syms = 0;
    size_t lens = str.length();
    bool increase_fl = true;

    while(current_id < lens) {
        bool flag_cont = false;
        
        std::cout << "# id: " << current_id << std::endl;

        auto search = current_node->childs.find(str[current_id]);

        if (search == current_node->childs.end()) { // если не нашли текущий символ
            current_node = current_node->suff_link;
            // current_id++; // переходим к следующему
            continue;
        }

        // все же нашли
        TNode *next_node = search->second; // пошли в потомка, которого нашли
        
        for (int i = next_node->start; i <= *(next_node->end); i++) {
            if (str[current_id] == text[i]) {
                cnt_syms++; // считаем, сколько совпало
            } else{
                if (current_node->suff_link) { // пустая внутрення вершина
                    current_node = current_node->suff_link; // перешли по суффиксной ссылке
                }
                
                std::cout << "cur id: " << current_id << std::endl;
                std::cout << "cnt: " << cnt_syms << std::endl;

                skip_syms = cnt_syms - 1;

                std::cout << "skip: " << skip_syms << std::endl;

                ms[current_id - cnt_syms] = cnt_syms; // положить в нужную позицию счетчик

                current_id -= cnt_syms - 1; // откуда ищем

                cnt_syms = 0; // уменьшить совпавшие при переходе на ветку

                flag_cont = true; // надо продолжить без перехода в потомка
                
                // if (skip_syms == 1) { 
                //     increase_fl = false;
                //     skip_syms = 0;
                // }
                break;
            }
            current_id++; // перескакиваем на следующую букву
        }

        if (flag_cont) { // без перехода в потомка
            continue;
        }

        if (skip_syms != 1) {
            current_node = next_node; // переход в потомка
        } else {
            skip_syms = 0;
            if (current_node->suff_link) {
                current_node = current_node->suff_link;
            }
            ms[current_id - 1] = cnt_syms;
            cnt_syms = 0;
            increase_fl = true;
        }
    }
}
