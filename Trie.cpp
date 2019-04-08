//
// Created by lm on 2019/4/8.
//

#include <cstdio>
#include "Trie.h"

void Trie::insert(const int num, const char *str){
    TrieNode* node = root;
    for(int i = 0; str[i] != '\0'; ++i){
        TrieNode*& newNode = node->child[(int)str[i]];
        if(!newNode) {
            newNode = new TrieNode();
        }
        node = newNode;
    }
    auto search = node->entries.find(num);
    node->entries.insert(num);
}

