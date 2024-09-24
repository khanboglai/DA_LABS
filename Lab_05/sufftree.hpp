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
#include <map>
#include <vector>
#include <algorithm>


class TSuffixTree {
    private:
        class TNode {
            public:
                TNode(TNode *slink, int st, int *e) { // конструктор для внутренней вершины
                    suff_link = slink;
                    start = st;
                    end = e;
                    suff_id = -1;
                }

                TNode(TNode *slink, int st, int *e, int suffix_idx) { // конструктор для листа
                    suff_link = slink;
                    start = st;
                    end = e;
                    suff_id = suffix_idx;
                }

                TNode *suff_link;
                int start;
                int *end; // правая граница для текста на дуге
                std::map<char, TNode*> childs;
                int suff_id;
        };

        void GrowthTree(int pos); // развитие дерева при добавлении нового символа
        void DeleteTree(TNode *node); // удаление дерева

        int LengthOnCurve(TNode *node); // кол-во символов на дуге
        void CollectIdx(TNode *node, std::vector<int> &vec); // сбор индексов при проходе по дереву

        TNode *root;
        TNode *last_inner_node; // последняя внутрення вершина, которую мы сосздали
        TNode *growth_node; // узел, от которого будем увеличивать дерево

        int growth_index; // идекс символа, на котором мы стоим сейчас
        int growth_step; // на сколько символов надо пройти до нужного индекса
        int CountPlannedSuffix; // счетчик планируемых суффиксов

        std::string text;
        int SuffTreeEnd; // end для всего дерева, первая эвристика

        void PrintEdges(TNode* node, int depth);


    public:
        TSuffixTree(std::string &text); // конструктор дерева
        ~TSuffixTree(); // деструктор

        void searchPattern(const std::string& pattern, std::vector<int>& indices);
        void CreateTree(); // создание дерева
        void PrintEdgesInOrder();
};
