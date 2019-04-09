//
// Created by lm on 2019/4/8.
//

#include <cstdio>
#include "Trie.h"

bool Trie::insert(int num, const char *str, size_t len){
    TrieNode* node = root;
    for(int i = 0; i < len; ++i){
        TrieNode*& newNode = node->child[(int)str[i]];
        if(!newNode) {
            newNode = new TrieNode();
        }
        node = newNode;
    }
//    auto search = node->entries.find(num);
//    printf("insert: %s %d count:%d\n",str, num, node->entries.count(num));
    return (node->entries.insert(num)).second;
}

std::set<int>* Trie::search(const char *str, size_t len) {
    TrieNode* node = root;
    for(int i = 0; i < len; ++i){
        TrieNode*& newNode = node->child[(int)str[i]];
        if(!newNode) {
            return nullptr;
        }
        node = newNode;
    }
    return &(node->entries);
}
