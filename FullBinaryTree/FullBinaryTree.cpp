#include "FullBinaryTree.h"

TNode* insertRec(TNode* node, string value) {
    if (!node) return new TNode(value);
    if (value < node->value) node->left = insertRec(node->left, value);
    else if (value > node->value) node->right = insertRec(node->right, value);
    return node;
}

void TINSERT(BinaryTree& tree, string value) {
    tree.root = insertRec(tree.root, value);
}

bool isFullRec(TNode* node) {
    if (!node) return true;
    if (!node->left && !node->right) return true;
    if (node->left && node->right) return isFullRec(node->left) && isFullRec(node->right);
    return false;
}

bool TCHECK(BinaryTree& tree) {
    return isFullRec(tree.root);
}

void TPRINT(TNode* node) {
    if (!node) return;
    TPRINT(node->left);
    cout << node->value << " ";
    TPRINT(node->right);
}

void TPRINT(const BinaryTree& tree) {
    TPRINT(tree.root);
    cout << endl; 
}
