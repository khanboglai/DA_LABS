#pragma once
#include <iostream>
#include <fstream>


struct TElem {
    std::string key;
    uint64_t value;
};


class TBTree {
    private:
        struct TNode {
            int n;
            TElem* el;
            TNode** children;
            bool leaf;

            static TElem* Search(TNode *node, std::string key);
            static TElem FindSuccessor(TNode *node);
            static TElem FindPredecessor(TNode *node);
        };

        TNode *AllocateTNode();
        void Deallocate(TNode *node);
        void DeleteTree(TNode *node);

        void SplitChild(TNode *parent, int index, TNode* child);
        TNode *MergeTNodes(TNode *parent, TNode *left_child, TNode *right_child, int i);
        
        void InsertNonfull(TNode *node, TElem elem);
        bool DeleteFromTNode(TNode *node, std::string key);

        void WriteInFile(TNode *node, std::ofstream& os);
        TNode *LoadFromFile(std::ifstream& in);

        TNode *root;
        int t; // характеристическое число дерева

    public:
        TBTree();
        ~TBTree();

        void Insert(TElem TElem);
        bool Delete(std::string key);
        bool Search(std::string key);
        void Save(std::ofstream& os);
        void Load(std::ifstream& in);
        std::string SWV(std::string key);
};


TBTree::TBTree() {
    root = nullptr;
    t = 4;
};


TElem TBTree::TNode::FindSuccessor(TNode *node) { // находим преемника
    while (!node->leaf) {
        node = node->children[node->n + 1];
    }
    return node->el[node->n];
}


TElem TBTree::TNode::FindPredecessor(TNode *node) { // находим предшественника
    while (!node->leaf) {
        node = node->children[1];
    }
    return node->el[1];
}


TBTree::TNode *TBTree::AllocateTNode() {
    TNode *node = new TBTree::TNode;
    node->el = new TElem[2 * t]; // чтобы влезло 2t-1 элементов
    node->children = new TBTree::TNode*[2 * t + 1]; // детей больше, чем родителей на 1

    for (int i = 0; i < 2 * t ; i++) {
        node->el[i].key = "";
        node->children[i] = nullptr;
    }

    node->children[2 * t] = nullptr;

    return node;
}


void TBTree::Deallocate(TNode *node) {
    if (node) {
        delete[] node->el;
        delete[] node->children;
        delete node;
    }
}


void TBTree::DeleteTree(TNode *node) {
    if (node) {
        if (node->leaf) {
            Deallocate(node);
        } else {
            for(int i = 1; i <= node->n + 1; i++) {
                DeleteTree(node->children[i]);
            }
            Deallocate(node);
        }
    }
}


TBTree::~TBTree() {
    DeleteTree(root);
};


TElem* TBTree::TNode::Search(TNode *node, std::string key) {
    if (node == nullptr) {
        return nullptr;
    }
    
    int i = 1;
    while (i <= node->n && key > node->el[i].key) {
        i++;
    }

    if (i <= node->n && key == node->el[i].key) {
        return &(node->el[i]);
    }

    if (node->leaf) {
        return nullptr;
    } else {
        return Search(node->children[i], key);
    }
}


bool TBTree::Search(std::string key) {
    TElem *res = TBTree::TNode::Search(root, key);

    if (res != nullptr) {
        return true;
    }
    return false;
}


void TBTree::SplitChild(TNode *empty_node, int i, TNode*em_node_child) {
    // em_node_child это заполненый узел
    // empty_node это пустой узел

    // переносим ключи в новый узел, они больше медианного элемента
    TNode *up_median = AllocateTNode();
    up_median->leaf = em_node_child->leaf;
    up_median->n = t - 1;

    for (int j = 1; j <= t - 1; j++) { // отправляем в новую ноду элементы, котороый больше медианы
        up_median->el[j] = em_node_child->el[j + t];
    }

    if (!em_node_child->leaf) { // переносим детей
        for (int j = 1; j <= t; j++) {
            up_median->children[j] = em_node_child->children[j + t];
        }
    }

    em_node_child->n = t - 1; // обновили брата

    // делаем up_median дочерним узлом
    for (int j = empty_node->n + 1; j >= i + 1; j--) { // переместили детей
        empty_node->children[j + 1] = empty_node->children[j];
    }
    empty_node->children[i + 1] = up_median; // новая нода, стала старшим ребенком

    for (int j = empty_node->n; j >= i; j--) {
        empty_node->el[j + 1] = empty_node->el[j];
    }

    empty_node->el[i] = em_node_child->el[t]; // перенесли медиану в родителя (между em_TNode_ch и up_median)
    empty_node->n++;
}



void TBTree::Insert(TElem elem) {
    TNode *r = root;
    
    // если корень пустой, создаем ноду
    if (r == nullptr) {
        r = AllocateTNode();
        r->leaf = true;
        r->n = 0;
        root = r;
    }

    // если корень насыщенный, разделяем
    if (r != nullptr && r->n == (2 * t - 1)) {
        TNode *s = AllocateTNode();
        root = s;
        s->leaf = false;
        s->n = 0;
        s->children[1] = r;
        SplitChild(s, 1, r);
        InsertNonfull(s, elem);

    } else { // иначе просто вставляем
        InsertNonfull(r, elem);
    }
}


void TBTree::InsertNonfull(TNode *not_full_node, TElem elem) {
    // узел должен быть незаполненым
    int i = not_full_node->n;

    if(not_full_node->leaf) { // вставка в лист
        while (i >= 1 && elem.key < not_full_node->el[i].key) {
            not_full_node->el[i + 1] = not_full_node->el[i];
            i--;
        }

        not_full_node->el[i + 1] = elem;
        not_full_node->n++;

    } else { // вставка в не листовой узел
        // определяем дочерний узел, куда можем вставить элемент
        while (i >= 1 && elem.key < not_full_node->el[i].key) {
            i--;
        }
        i++;

        // если он заполнен, его надо разделить
        if (not_full_node->children[i]->n == (2 * t - 1)) {
            SplitChild(not_full_node, i, not_full_node->children[i]);

            if(elem.key > not_full_node->el[i].key) {
                i++;
            }
        }

        InsertNonfull(not_full_node->children[i], elem);
    }
}


TBTree::TNode *TBTree::MergeTNodes(TNode *parent, TNode *left_child, TNode *right_child, int i) {
    // элементы из правого дочернего узла добавляем в конец левого
    for (int j = 1; j < t; j++) {
        left_child->el[t + j] = right_child->el[j];
    }
    left_child->el[t] = parent->el[i]; // спускаем из родителя в левого ребенка

    // аналогично, переносим детей из правого в левый узел
    for (int j = 1; j <= t; j++) {
        left_child->children[t + j] = right_child->children[j];
    }
    left_child->n = 2 * t - 1;

    // сдвигаем элементы родительского узла
    for (int j = i; j < parent->n; j++) {
        parent->el[j] = parent->el[j + 1];
    }

    // аналогично, переносим детей
    for (int j = i + 1; j <= parent->n; j++) {
        parent->children[j] = parent->children[j + 1];
    }
    parent->n--; // уменьшаеи кол-во элементов в родительском узле

    Deallocate(right_child); // освобождаем правого ребенка, мы уже в левом

    if (parent->n == 0) {
        Deallocate(root);
        root = left_child;
    }

    return left_child;
}


bool TBTree::Delete(std::string key) {
    return DeleteFromTNode(root, key);
}


bool TBTree::DeleteFromTNode(TNode *node, std::string key) {

    if (node == nullptr) {
        return false;
    }

    int i = 1;
    while (i <= node->n && key > node->el[i].key) { // аналогично вставке, проходим по дереву
        i++;
    }

    // Корман: случай 1
    if (node->leaf) {
        if (i <= node->n && key == node->el[i].key) {
            for (int j = i; j < node->n; j++) {
                node->el[j] = node->el[j + 1]; // сдвигаем элементы
            }
            node->n--;
            return true;
        } else {
            return false;
        }
    }

    // Корман: случай 2
    if (i <= node->n && key == node->el[i].key) {
        
        if (node->children[i]->n >= t) { // случай 2.а
            // рассматриваем дочерний узел, предществующий родителю
            TElem tmp = TBTree::TNode::FindSuccessor(node->children[i]); // ищем последователя
            node->el[i] = tmp;
            return DeleteFromTNode(node->children[i], tmp.key);

        } else if (node->children[i + 1]->n >= t) { // случай 2.б
            // рассматриваем дочерний узел, следующий за родителем
            TElem tmp = TBTree::TNode::FindPredecessor(node->children[i + 1]); // ищем предшественника
            node->el[i] = tmp;
            return DeleteFromTNode(node->children[i + 1], tmp.key);

        } else { // случай 2.в
            TBTree::TNode *new_child = MergeTNodes(node, node->children[i], node->children[i + 1], i); // объединяем родителя и его детей
            return DeleteFromTNode(new_child, key); // получился новый ребенок, удаляем из него ключ
        }
    } else if (node->children[i]->n == t - 1) { // Корман: случай 3
        // если ключ k отсутствует во внутреннем узле
        // 3.а

        if ( i > 1 && node->children[i - 1]->n >= t) { // сосед слева содержит как мимнимут t ключей
            TBTree::TNode *current = node->children[i];
            TBTree::TNode *prev = node->children[i - 1];

            int prev_n = prev->n;

            for (int j = current->n + 1; j > 1; j--) {
                current->el[j] = current->el[j - 1]; // сдвигаем элементы
            }
            current->el[1] = node->el[i - 1]; // пердача ключа разделителя между текущим узлом и соседом

            for (int j = current->n + 2; j > 1; j--) {
                current->children[j] = current->children[j - 1]; // сдвигаем детей
            }

            current->children[1] = prev->children[prev_n + 1];
            current->n++;

            node->el[i - 1] = prev->el[prev_n]; // крайний ключ на место узла разделителя

            prev->n--;

            return DeleteFromTNode(current, key);
        } else if ( i <= node->n && node->children[i + 1]->n >= t) { // сосед справа содержит как минимум t ключей
            TBTree::TNode *current = node->children[i];
            TBTree::TNode *next = node->children[i + 1];

            int next_n = next->n;
            int current_n = current->n;

            current->el[current_n + 1] = node->el[i]; // переносим ключ-разделитель между текущим узлом и соседом
            current->children[current_n + 2] = next->children[1]; // ребенка у крайнего элемента соседа нужно сохранить
            current->n++;

            node->el[i] = next->el[1]; // подняли из соседа в узел

            for ( int j = 1; j < next_n; j++) {
                next->el[j] = next->el[j + 1]; // сдвинули элементы в соседе
            }

            for ( int j = 1; j <= next_n; j++) {
                next->children[j] = next->children[j + 1]; // сдвинули детей в соседе
            }
            next->n--;

            return DeleteFromTNode(current, key);
        } else { // 3.б

            // нет у соседей, надо объединяться
            if (i > node->n) { // крайний случай, могли по i уже выйти за пределы массива
                i--;
            }

            // соединяем узлы, ключ разделитель станет медианой нового узла
            TBTree::TNode *new_child = MergeTNodes(node, node->children[i], node->children[i + 1], i);
            return DeleteFromTNode(new_child, key);
        }
    }

    return DeleteFromTNode(node->children[i], key);
}


std::string TBTree::SWV(std::string key) { // надоело, написал отдельную функцию
    TElem *res = TBTree::TNode::Search(root, key);

    if (res != nullptr) {
        return "OK: " + std::to_string((*res).value);
    }
    return "NoSuchWord";
}


void TBTree::WriteInFile(TNode *node, std::ofstream& os) {
    // не проверяем на открытие файл

    if (node) {
        os.write(reinterpret_cast<const char *>(&node->n), sizeof(int));
        for (int i = 1; i <= node->n; i++) {
            size_t str_size = node->el[i].key.length() + 1;
            os.write(node->el[i].key.c_str(), str_size);
            os.write(reinterpret_cast<const char *>(&node->el[i].value), sizeof(uint64_t));
        }
        os.write(reinterpret_cast<const char *>(&node->leaf), sizeof(bool));

        for (int i = 1; i <= node->n + 1; i++) {
            WriteInFile(node->children[i], os);
        }
    } else {
        return;       
    }
}


TBTree::TNode *TBTree::LoadFromFile(std::ifstream& in) {
    // открыт ли файл не проверяем

    if (in.eof() || (in.peek() == EOF)) { // если файл пустой, дерево пустое
        return nullptr;
    } else {
        int n;
        in.read(reinterpret_cast<char *>(&n), sizeof(int));

        TNode *new_node = AllocateTNode();
        new_node->n = n;

        for (int i = 1; i <= n; i++) {
            std::string key;

            char ch;
            while (in.get(ch) && ch != '\0') {
                key += ch;
            }

            new_node->el[i].key = key;
            in.read(reinterpret_cast<char *>(&new_node->el[i].value), sizeof(uint64_t));
        }

        in.read(reinterpret_cast<char *>(&new_node->leaf), sizeof(bool));

        if (!new_node->leaf) {
            for (int i = 1; i <= n + 1; i++) {
                new_node->children[i] = LoadFromFile(in);
            }
        }
        return new_node;
    }
}


void TBTree::Save(std::ofstream& os) {
    WriteInFile(root, os);
}


void TBTree::Load(std::ifstream& in) {
    DeleteTree(root);
    root = LoadFromFile(in);
}
