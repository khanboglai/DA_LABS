#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


const char SENTINEL = '$';
const int TERMPOS = -1;
const int TERMVAL = 0;
const bool ISLEAF = true;


class TSuffixTree {
    private:
        class TNode {
            public:
                int begin; // начало ребра
                int *end; // конец ребра, указатель, чтобы можно было его менять
                TNode *suffix_link; // суффиксная ссылка
                bool is_leaf; // статус узла
                std::unordered_map<char, TNode *> child; // потомки
                // вибрал unordered_map потому что в среднем вставка, поиск, удаления за O(1)

                TNode(int start, int *finish, TNode* s_link, bool leaf); // конструктор узла
                ~TNode() = default;
        };

        // структура для параметров дерева
        struct TreeData {
            TNode *current_node; // текущий узел
            int current_index; // текущий символ
            int jump_counter; // счетчик прыжков
            int plannedSuffixs; // количество суффиксов, которое нужно создать
            TNode *last_inner_node; // последняя внутрення вершина, котору мы создали и которую мы не привязяли
        };


        TNode *root; // корень
        std::string str; // срока, по которой строим
        int suffixTreeEnd; // конец ребра для всего дерева
        TreeData params; // параметры, которые нужно запомнить

        void CreateTree(); // создание дерева
        void AddSuffix(int position); // добавлений суффикса
        void DestroyTree(TNode *node); // удаление дерева
        int CurveLength(TNode *node); // длина текста на ребре
        void SplitCurve(TNode *node, int position); // разделение ребра (создание внутреннего узла)

    public:
        TSuffixTree(std::string &input_str); // конструктор дерева
        ~TSuffixTree(); // дествруктор
        void MatchStatistic(std::vector<int> &ms, const std::string &str); // подсчет статистики совпадений
};