#ifndef FULLBINARYTREE_H
#define FULLBINARYTREE_H
#include <string>
#include <iostream>
using namespace std;

struct TNode {
    string value;
    TNode* left;
    TNode* right;
    TNode(string val) : value(val), left(nullptr), right(nullptr) {}
};

struct BinaryTree {
    TNode* root = nullptr;
};

void TINSERT(BinaryTree& tree, string value);
bool TCHECK(BinaryTree& tree);
void TPRINT(TNode* node);
void TPRINT(const BinaryTree& tree); 
#endif
