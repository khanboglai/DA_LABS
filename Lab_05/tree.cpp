#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

class SuffixTree {
public:
    struct Node {
        std::unordered_map<char, Node*> children;
        int start, *end;
        int suffixLink;
        int length;

        Node(int start, int* end) : start(start), end(end), suffixLink(-1), length(0) {}
    };

    SuffixTree(const std::string& text) : text(text), root(new Node(-1, new int(-1))), activeNode(root), activeEdge(0), activeLength(0), remainder(0), currentEnd(0) {
        build();
    }

    ~SuffixTree() {
        deleteTree(root);
    }

private:
    std::string text;
    Node* root;
    Node* activeNode;
    int activeEdge;
    int activeLength;
    int remainder;
    int currentEnd;

    void build() {
        for (int i = 0; i < text.size(); ++i) {
            extend(i);
        }
    }

    void extend(int pos) {
        currentEnd = pos;
        remainder++;
        while (remainder > 0) {
            if (activeLength == 0) {
                activeEdge = pos;
            }

            if (activeNode->children.find(text[activeEdge]) == activeNode->children.end()) {
                activeNode->children[text[activeEdge]] = new Node(pos, new int(currentEnd));
                if (activeNode != root) {
                    activeNode->suffixLink = root->children[text[activeEdge]]->start;
                }
                remainder--;
            } else {
                Node* next = activeNode->children[text[activeEdge]];
                if (walkDown(next)) {
                    continue;
                }

                if (text[next->start + activeLength] == text[pos]) {
                    activeLength++;
                    if (activeNode != root) {
                        activeNode->suffixLink = root->children[text[activeEdge]]->start;
                    }
                    break;
                }

                int* splitEnd = new int(next->start + activeLength - 1);
                Node* split = new Node(next->start, splitEnd);
                activeNode->children[text[activeEdge]] = split;
                split->children[text[pos]] = new Node(pos, new int(currentEnd));
                next->start += activeLength;
                split->children[text[next->start]] = next;

                if (activeNode != root) {
                    activeNode->suffixLink = root->children[text[activeEdge]]->start;
                }
                remainder--;
            }

            if (activeNode == root && activeLength > 0) {
                activeLength--;
                activeEdge = pos - remainder + 1;
            } else {
                activeNode = activeNode->suffixLink != -1 ? root->children[text[activeEdge]] : root;
            }
        }
    }

    bool walkDown(Node* next) {
        if (activeLength >= next->length) {
            activeEdge += next->length;
            activeLength -= next->length;
            activeNode = next;
            return true;
        }
        return false;
    }

    void deleteTree(Node* node) {
        for (auto& child : node->children) {
            deleteTree(child.second);
        }
        delete node->end;
        delete node;
    }

    void SuffixTree::CollectIdx(Node *node, std::vector<int> &vec) {
        if (!node) {
            return;
        }

        for (auto it : node->children) {
            CollectIdx(it.second, vec);
        }
        if (node->suff_id != -1) {
            vec.push_back(node->suff_id);
        }
    }


    void SuffixTree::searchPattern(const std::string& pattern, std::vector<int>& indices) {

        Node* currentNode = root; // Начинаем с корневого узла
        int currentIndex = 0; // Индекс текущего символа в шаблоне
        int patternLength = pattern.length();

        while (currentIndex < patternLength) {
            char currentChar = pattern[currentIndex];

            // Проверяем наличие символа в дочерних узлах
            if (currentNode->children.find(currentChar) == currentNode->children.end()) {
                // Символ не найден, выходим из метода
                return;
            }

            // Переходим к дочернему узлу
            Node* childNode = currentNode->children[currentChar];
            int edgeLength = childNode->length; // Длина ребра
            int edgeStart = childNode->start; // Начало ребра

            // Проходим по ребру
            for (int i = 0; i < edgeLength && currentIndex < patternLength; i++) {
                if (text[edgeStart + i] != pattern[currentIndex]) {
                    // Если символы не совпадают, выходим из метода
                    return;
                }
                currentIndex++;
            }

            // Если мы прошли все символы на ребре, переходим к дочернему узлу
            currentNode = childNode;
        }

        // Если мы дошли до конца шаблона, собираем индексы суффиксов
        CollectIdx(currentNode, indices);

    }
};



int main() {
    std::string text = "banana";
    SuffixTree tree(text);
    std::cout << "Suffix tree built for the text: " << text << std::endl;
    return 0;
}
