#pragma once
#include <vector>
#include <iostream>
#include <algorithm>


template<class T>
class BT {
    private:
        struct Tnode {
            std::vector<T> n; // элементы корня
            std::vector<Tnode*> leafs; // дети
            bool IsLeaf;

        // Tnode();
        // ~Tnode();
        };


        Tnode* root;
        int t; // характеристическое число

    public:
        BT(int ch_t);
        ~BT();

        void Insert(T val);
        bool Search(T val);
        void Remove(T val);
        void Print();
};


template<class T>
BT<T>::BT(int ch_t) {
    root = new Tnode();
    root->IsLeaf = false;
    t = ch_t;
}

template<class T>
BT<T>::~BT() {
    for (Tnode* elem: root->leafs) {
        delete elem;
    }
    delete root;
}

template<class T>
void BT<T>::Insert(T val) {
    if ((root->n).size() < 2 * t - 1) {
        (root->n).push_back(val);
        std::sort(begin(root->n), end(root->n));
    } else {
        Tnode* leaf_l = new Tnode();
        leaf_l->IsLeaf = true;

        int border = (2 * t - 1) / 2;
        
        // std::cout << border << std::endl;

        for (int i = 0; i <= border - 1; i++) {
            (leaf_l->n).push_back((root->n)[i]);
            (root->n).erase((root->n).begin());
        }
        (root->leafs).push_back(leaf_l);

        Tnode* leaf_r = new Tnode();
        leaf_r->IsLeaf = true;

        for (int i = 1; i <= border; i++) {
            (leaf_r->n).push_back((root->n)[i]);
            (root->n).erase((root->n).begin() + 1);
        }
        (root->leafs).push_back(leaf_r);


        if (val < *std::min_element(begin(root->n), end(root->n))) {
            (leaf_l->n).push_back(val);
        }
    }
}


template<class T>
void BT<T>::Print() {
    if (!(root->leafs).empty()) {
        for (T elem: (root->leafs)[0]->n) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }


    for (T elem: root->n) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    if (!(root->leafs).empty()) {
        for (T elem: (root->leafs)[1]->n) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}
