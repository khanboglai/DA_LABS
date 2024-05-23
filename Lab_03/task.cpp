/**
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 * };
 * Ввод:
 *    1            <---
 *  /   \
 * 2     3         <---
 *  \     \
 *   5     4       <---
 *    \
 *     10          <---
 * Вывод:
 * [1, 3, 4, 10]
 */


#include <iostream>
#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

std::vector<int> rightSideView(TreeNode* root) {
    std::vector<TreeNode*> level_nodes;
    std::vector<TreeNode*> next_level_nodes;
    std::vector<TreeNode*> result;
    level_nodes.push_back(root);

    while (!level_nodes.empty()) {
        next_level_nodes.clear();
        for (size_t i = 0; i < level_nodes.size(); ++i) {
            
            if (level_nodes[i]->left != nullptr) { // кладем левого ребенка
                next_level_nodes.push_back(level_nodes[i]->left);
            }
           
            if (level_nodes[i]->right != nullptr) { // кладем правого ребенка
                next_level_nodes.push_back(level_nodes[i]->right);
            }
        }
        result.push_back(level_nodes[level_nodes.size() - 1]); // кладем самый правый элемент
        level_nodes.clear();
        level_nodes = std::move(next_level_nodes);
    }

    std::vector<int> res_int;
    for (int i = 0; i < result.size(); i++) {
        res_int.push_back(result[i]->val);
    }
    return res_int;
}
