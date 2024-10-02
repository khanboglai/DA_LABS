// Строим суффиксное дерево при помощи алгоритма Укконена за O(n)
#pragma once

/*
Правила в гасфилде

Правило 1
увеличиваем end -> end++

Правило 2
создание внутренней и листовой вершины 

Правило 3
путь начинается с нового символа, значит ничего делать не нужно
*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


// константы
const char SENTINEL = '$';
const int TERMPOS = -1;
const int TERMVAL = 0;


class TSuffixTree {
    private:
        class TNode {
            public:
                TNode *suff_link;
                int start;
                int *end; // правая граница для текста на дуге
                std::unordered_map<char, TNode*> childs; // нам не нужно хранить значения в определенном порядке
                bool is_leaf;

                TNode(int st, int *e, TNode *sf_link, bool leaf_status) { // конструктор для узла
                    suff_link = sf_link;
                    start = st;
                    end = e;
                    is_leaf = leaf_status;
                }
        };

        void CreateTree(); // создание дерева
        TNode *FindChildNode(TNode *current, char s);
        void AddSuffix(int position); // развитие дерева при добавлении нового символа
        void DeleteTree(TNode *node); // удаление дерева
        int LengthOnCurve(TNode *node); // кол-во символов на дуге

        void SplitNode(TNode *next, int position);
        void UpdateCurrentPos();

        TNode *root;
        TNode *last_inner_node; // последняя внутрення вершина, которую мы создали
        TNode *current_node; // узел, от которого будем увеличивать дерево

        int current_index; // индекс символа, на котором мы стоим сейчас
        int jump_cnt; // на сколько символов надо пройти до нужного индекса, вторая эвристика
        int plannedSiffixs; // счетчик планируемых суффиксов

        std::string text;
        int sufftreeEnd; // end для всего дерева, первая эвристика


    public:
        TSuffixTree(std::string &text); // конструктор дерева
        ~TSuffixTree(); // деструктор
        void MatchStatistic(std::vector<int> &value, const std::string &text);
};
