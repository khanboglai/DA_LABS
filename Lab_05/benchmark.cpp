#include "sufftree.hpp"
#include <algorithm>
#include <limits>
#include <iomanip>
#include <fstream>
#include <map>

const char TERMINAL = '$';

using vec_t = std::vector<size_t>;

class SuffixNode {
public:
    int number = -1;
    std::map<char, SuffixNode> m_data;
};

class SuffixTree {
public:
    SuffixTree( const std::string & pattern ) { Build( pattern + TERMINAL ); }
    ~SuffixTree() {}
    bool Search( const std::string & text, vec_t &pos ); 
private:
    void Build( const std::string & );
    void AddSuffix( const std::string &, size_t );
    void CollectPositions( const SuffixNode *node, vec_t &pos );
private:
    SuffixNode root;
};

void SuffixTree::Build( const std::string & pattern ) {
    for( size_t i = 0; i < pattern.size(); ++i ) {
        std::string suffix = pattern.substr( i, pattern.size() - i );
        AddSuffix( suffix, i+1 );
    }
}


void SuffixTree::AddSuffix( const std::string & suffix, size_t number) {
    SuffixNode *node = &root;
    for( size_t i = 0; i < suffix.size(); ++i ) {
        const char *sign = &suffix[i];
        auto it = node->m_data.find( *sign );
        // Если дуги с такой меткой нет, тогда создадим её. 
        if( it == node->m_data.end() ) {
            SuffixNode new_node = SuffixNode();
            // Лист? Тогда присвоим ему номер суффикса.
            if( i == suffix.size() - 1 ) {
                new_node.number = number;
            }
            node->m_data[*sign] = new_node;
        }
        node = &(node->m_data[*sign]);
    }
}


void SuffixTree::CollectPositions( const SuffixNode *node, vec_t &pos ) {
    for( auto const &elem : node->m_data ) {
        if( elem.first == TERMINAL ) {
            pos.push_back( elem.second.number );
        }
        
        CollectPositions( &(elem.second), pos );
    }
}


bool SuffixTree::Search( const std::string & text, vec_t &pos ) {
    SuffixNode *node = &root;
    for( size_t i = 0; i < text.size(); ++i ) {
        const char *sign = &text[i];
        auto it = node->m_data.find( *sign );
        // Если нет ребра с такой меткой, то текст мы не нашли.
        if( it == node->m_data.end() ) {
            return false;
        }
        node = &(node->m_data[*sign]);

        // Это последний символ? Тогда найдём все позиции в оставшемся
        // поддереве.
        if( i == text.size() - 1 ) {
            CollectPositions( node, pos );
            std::sort( pos.begin(), pos.end() );
        }
    }
    return true;
}


// int main() {
//     std::string text, pattern;
//     std::cin >> pattern;
    
//     return 0;
// }

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
    SuffixTree root(text);
    
    vec_t pos;
    // Образец не встретился в суффиксе.
    bool st = root.Search( pat, pos );
    
    if (st) {
        for (size_t elem : pos) {
            std::cout << elem << std::endl;
        }
    }

    end = clock();

    long double searh = end - start;
    std::cout << "Naive Suffix Tree: " << std::fixed << std::setprecision(3) << searh / 1000.0 << " ms"<< std::endl;
    
    std::cout << std::endl;

    return 0;
}