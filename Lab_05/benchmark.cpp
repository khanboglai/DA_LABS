#include "sufftree.hpp"
#include <algorithm>
#include <limits>
#include <iomanip>
#include <fstream>


class SuffixTreeNode {
public:
    std::unordered_map<char, SuffixTreeNode*> children;
    int start, *end;
    int suffixIndex;

    SuffixTreeNode(int start, int* end) : start(start), end(end), suffixIndex(-1) {}
};

class SuffixTree {
private:
    SuffixTreeNode* root;
    std::string text;

    void buildSuffixTree() {
        int n = text.length();
        int* end = new int;
        *end = -1;

        for (int i = 0; i < n; i++) {
            addSuffix(i, end);
        }
    }

    void addSuffix(int startIndex, int* end) {
        SuffixTreeNode* currentNode = root;
        for (int i = startIndex; i < text.length(); i++) {
            char currentChar = text[i];
            if (currentNode->children.find(currentChar) == currentNode->children.end()) {
                SuffixTreeNode* newNode = new SuffixTreeNode(i, end);
                currentNode->children[currentChar] = newNode;
                currentNode = newNode;
            } else {
                currentNode = currentNode->children[currentChar];
            }
        }
    }

    void searchUtil(SuffixTreeNode* node, const std::string& pattern, int depth, std::vector<int>& result) {
        if (depth == pattern.length()) {
            if (node->suffixIndex != -1) {
                result.push_back(node->suffixIndex);
            }
            return;
        }

        char currentChar = pattern[depth];
        if (node->children.find(currentChar) != node->children.end()) {
            searchUtil(node->children[currentChar], pattern, depth + 1, result);
        }
    }

public:
    SuffixTree(const std::string& text) : text(text) {
        root = new SuffixTreeNode(-1, new int(-1));
        buildSuffixTree();
    }

    std::vector<int> search(const std::string& pattern) {
        std::vector<int> result;
        searchUtil(root, pattern, 0, result);
        return result;
    }
};



int main() {
    std::ofstream nullStream("/dev/null");
    long double start, end;

    std::string str;
    std::cin >> str;
    std::string pat = str;
    int pattern_len = str.length();
    std::vector<int> ms;
    std::string text;
    std::cin >> text;

    start = clock();
    TSuffixTree t(str);
    t.MatchStatistic(ms, text);

    for (size_t i = 0; i < ms.size(); i++) {
        if (ms[i] == pattern_len) {
            std::cout << i + 1 << std::endl;
        }
    } 
    end = clock();

    long double Uk = end - start;
    std::cout << "Ukkonen+MS: " << std::fixed << std::setprecision(3) << Uk / 1000.0 << " ms"<< std::endl;


    start = clock();
    SuffixTree suffixTree(text);
    std::vector<int> occurrences = suffixTree.search(pat);

    if (!occurrences.empty()) {
        for (int index : occurrences) {
            std::cout << index << std::endl;
        }
    end = clock();

    long double searh = end - start;
    std::cout << "std::search: " << std::fixed << std::setprecision(3) << searh / 1000.0 << " ms"<< std::endl;
    
    std::cout << std::endl;

    return 0;
}