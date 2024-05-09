#pragma once
#include <iostream>


struct Elem {
    std::string key;
    uint64_t value;
};


class BTree {
    private:
        struct Node {
            int n;
            Elem* el;
            Node** children;
            bool leaf;
        };

        struct MyPair {
            Node **node;
            int idx;
        };
        
        Elem* Search(Node *node, std::string key);

        Node *AllocateNode();
        void Deallocate(Node *node);

        void SplitChild(Node *parent, int index, Node* child);
        Node *MergeNodes(Node *parent, Node *left_child, Node *right_child, int i);
        
        void InsertNonfull(Node *node, Elem elem);
        Elem FindSuccessor(Node *node);
        Elem FindPredecessor(Node *node);
        bool DeleteFromNode(Node *node, std::string key);
        
        void DeleteTree(Node *node);

        Node *root;
        int t; // характеристическое число дерева

    public:
        BTree();
        ~BTree();

        void Insert(Elem elem);
        bool Delete(std::string key);
        bool Search(std::string key);
        std::string SWV(std::string key);
};

BTree::BTree() {
    root = nullptr;
    t = 4;
};


Elem BTree::FindSuccessor(Node *node) { // находим самый большой ключ
    while (!node->leaf) {
        node = node->children[node->n + 1];
    }
    return node->el[node->n];
}


Elem BTree::FindPredecessor(Node *node) { // находим самый маленький ключ
    while (!node->leaf) {
        node = node->children[1];
    }
    return node->el[1];
}


void BTree::DeleteTree(Node *node) {
    if (node) {
        if(node->leaf) {
            Deallocate(node);
        } else {
            for(int i = 1; i <= node->n + 1; i++) {
                DeleteTree(node->children[i]);
            }
            Deallocate(node);
        }
    }
}


BTree::~BTree() {
    DeleteTree(root);
};


BTree::Node *BTree::AllocateNode() {
    Node *node = new Node;
    node->el = new Elem[2 * t]; // чтобы влезло 2t-1 элементов
    node->children = new Node*[2 * t + 1]; // детей больше, чем родителей на 1

    for(int i = 0; i < 2 * t ; i++) {
        node->el[i].key = "";
        node->children[i] = nullptr;
    }

    node->children[2 * t] = nullptr;

    return node;
}


void BTree::Deallocate(Node *node) {
    if (node) {
        delete[] node->el;
        delete[] node->children;
        delete node;
    }
}


Elem* BTree::Search(Node *node, std::string key) {
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


bool BTree::Search(std::string key) {
    Elem *res = BTree::Search(root, key);

    if (res != nullptr) {
        return true;
    }
    return false;
}


void BTree::SplitChild(Node *empty_node, int i, Node*em_node_child) {
    // em_node_child это заполненый узел
    // empty_node это пустой узел

    // переносим ключи в новый узел
    Node *up_median = AllocateNode();
    up_median->leaf = em_node_child->leaf;
    up_median->n = t - 1;

    for(int j = 1; j <= t - 1; j++) { // отправляем в новую ноду элементы, котороый больше медианы
        up_median->el[j] = em_node_child->el[j + t];
    }

    if(!em_node_child->leaf) { // переносим детей
        for(int j = 1; j <= t; j++) {
            up_median->children[j] = em_node_child->children[j + t];
        }
    }

    em_node_child->n = t - 1; // обновили брата

    // делаем up_median дочерним узлом
    for(int j = empty_node->n + 1; j >= i + 1; j--) { // переместили детей
        empty_node->children[j + 1] = empty_node->children[j];
    }
    empty_node->children[i + 1] = up_median; // новая нода, стала старшим ребенком

    for(int j = empty_node->n; j >= i; j--) {
        empty_node->el[j + 1] = empty_node->el[j];
    }

    empty_node->el[i] = em_node_child->el[t]; // перенесли медиану в родителя (между em_node_ch и up_median)
    empty_node->n++;
}



void BTree::Insert(Elem elem) {
    Node *r = root;
    
    // если корень пустой, создаем ноду
    if(r == nullptr) {
        r = AllocateNode();
        r->leaf = true;
        r->n = 0;
        root = r;
    }

    // если корень насыщенный, разделяем
    if (r != nullptr && r->n == (2 * t - 1)) {
        Node *s = AllocateNode();
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


void BTree::InsertNonfull(Node *not_full_node, Elem elem) {
    // узел должен быть незаполненым
    int i = not_full_node->n;

    if(not_full_node->leaf) { // вставка в лист
        while(i >= 1 && elem.key < not_full_node->el[i].key) {
            not_full_node->el[i + 1] = not_full_node->el[i];
            i--;
        }

        not_full_node->el[i + 1] = elem;
        not_full_node->n++;

    } else { // вставка в не листовой узел
        // определяем дочерний узел, куда можем вставить элемент
        while(i >= 1 && elem.key < not_full_node->el[i].key) {
            i--;
        }
        i++;

        // если он заполнен, его надо разделить
        if(not_full_node->children[i]->n == (2 * t - 1)) {
            SplitChild(not_full_node, i, not_full_node->children[i]);

            if(elem.key > not_full_node->el[i].key) {
                i++;
            }
        }

        InsertNonfull(not_full_node->children[i], elem);
    }
}


BTree::Node *BTree::MergeNodes(Node *parent, Node *left_child, Node *right_child, int i) {
    for(int j = 1; j < t; j++) {
        left_child->el[t + j] = right_child->el[j];
    }
    left_child->el[t] = parent->el[i];

    for(int j = 1; j <= t; j++) {
        left_child->children[t + j] = right_child->children[j];
    }
    left_child->n = 2 * t - 1;

    for(int j = i; j < parent->n; j++) {
        parent->el[j] = parent->el[j + 1];
    }

    for(int j = i + 1; j <= parent->n; j++) {
        parent->children[j] = parent->children[j + 1];
    }
    parent->n--;

    Deallocate(right_child);

    if (parent->n == 0) {
        Deallocate(root);
        root = left_child;
    }

    return left_child;
}


bool BTree::Delete(std::string key) {
    return DeleteFromNode(root, key);
}


bool BTree::DeleteFromNode(Node *node, std::string key) {

    if (node == nullptr) {
        return false;
    }

    int i = 1;
    while(i <= node->n && key > node->el[i].key) { // аналогично вставке, проходим по дереву
        i++;
    }

    // Корман: случай 1
    if(node->leaf) {
        if(i <= node->n && key == node->el[i].key) {
            for(int j = i; j < node->n; j++) {
                node->el[j] = node->el[j + 1];
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
            // рассматриваем дочерний узел, предществующий текущему родителю
            Elem tmp = FindSuccessor(node->children[i]);
            node->el[i] = tmp;
            return DeleteFromNode(node->children[i], tmp.key);

        } else if(node->children[i + 1]->n >= t) { // случай 2.б
            // рассматриваем дочерний узел, следующий за текущим родителем
            Elem tmp = FindPredecessor(node->children[i + 1]);
            node->el[i] = tmp;
            return DeleteFromNode(node->children[i + 1], tmp.key);

        } else { // случай 2.с
            Node *new_child = MergeNodes(node, node->children[i], node->children[i + 1], i); // объединяем родителя и его детей
            return DeleteFromNode(new_child, key); // получился новый ребенок, удаляем из него ключ
        }
    } else if(node->children[i]->n == t - 1) { // Корман: случай 3
        
        // 3.а

        if ( i > 1 && node->children[i - 1]->n >= t) {
            Node *current = node->children[i];
            Node *prev = node->children[i - 1];

            int prev_n = prev->n;

            for (int j = current->n + 1; j > 1; j--) {
                current->el[j] = current->el[j - 1];
            }
            current->el[1] = node->el[i - 1];

            for (int j = current->n + 2; j > 1; j--) {
                current->children[j] = current->children[j - 1];
            }

            current->children[1] = prev->children[prev_n + 1];
            current->n++;

            node->el[i - 1] = prev->el[prev_n];

            prev->n--;

            return DeleteFromNode(current, key);
        } else if( i <= node->n && node->children[i + 1]->n >= t) {
            Node *current = node->children[i];
            Node *next = node->children[i + 1];

            int next_n = next->n;
            int current_n = current->n;

            current->el[current_n + 1] = node->el[i];
            current->children[current_n + 2] = next->children[1];
            current->n++;

            node->el[i] = next->el[1];

            for( int j = 1; j < next_n; j++) {
                next->el[j] = next->el[j + 1];
            }

            for( int j = 1; j <= next_n; j++) {
                next->children[j] = next->children[j + 1];
            }
            next->n--;

            return DeleteFromNode(current, key);
        } else { // 3.б
            if (i > node->n) {
                i--;
            }

            Node *new_child = MergeNodes(node, node->children[i], node->children[i + 1], i);
            return DeleteFromNode(new_child, key);
        }
    }

    return DeleteFromNode(node->children[i], key);
}


std::string BTree::SWV(std::string key) {
    Elem *res = BTree::Search(root, key);

    if (res != nullptr) {
        return "OK: " + std::to_string((*res).value);
    }
    return "NoSuchWord";
}
