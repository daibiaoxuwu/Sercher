//
// Created by lm on 2019/4/8.
//

#include <cstdio>
#include "Trie.h"

void Trie::insert(int num, const char *str, size_t len){
    TrieNode* node = root;
    for(int i = 0; i < len; ++i){
        TrieNode*& newNode = node->child[(int)str[i]];
        if(!newNode) {
            newNode = new TrieNode();
        }
        node = newNode;
    }
    auto search = node->entries.find(num);
    node->entries.insert(num);
    printf("insert: %s %d count:%d\n",str, num, node->entries.count(num));
}

