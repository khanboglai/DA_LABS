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


class TSuffixTree {
    private:
        class TNode {
            public:
                TNode(TNode *slink, int st, int *e, int suffix_idx) { // конструктор для узла
                    suff_link = slink;
                    start = st;
                    end = e;
                    suff_id = suffix_idx;
                }

                TNode *suff_link;
                int start;
                int *end; // правая граница для текста на дуге
                std::unordered_map<char, TNode*> childs;
                int suff_id;
        };

        void CreateTree(); // создание дерева
        void AddSuffix(int position); // развитие дерева при добавлении нового символа
        void DeleteTree(TNode *node); // удаление дерева
        int LengthOnCurve(TNode *node); // кол-во символов на дуге

        void SplitNode(TNode *next, int position);
        void UpdateCurrentPos();

        TNode *root;
        TNode *last_inner_node; // последняя внутрення вершина, которую мы создали
        TNode *current_node; // узел, от которого будем увеличивать дерево

        int current_index; // индекс символа, на котором мы стоим сейчас
        int growth_step; // на сколько символов надо пройти до нужного индекса, вторая эвристика
        int plannedSiffixs; // счетчик планируемых суффиксов

        std::string text;
        int sufftreeEnd; // end для всего дерева, первая эвристика


    public:
        TSuffixTree(std::string &text); // конструктор дерева
        ~TSuffixTree(); // деструктор
        void MatchStatistic(std::vector<int> &value, const std::string &text);
};
