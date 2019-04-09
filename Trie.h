//
// Created by lm on 2019/4/8.
//

#ifndef UPLOAD_SIMSEARCHER_TRIE_H
#define UPLOAD_SIMSEARCHER_TRIE_H

#include <set>
#include <cstring>
struct TrieNode
{
    TrieNode* child[128];
    std::set<int> entries;
};
class Trie {
    TrieNode* root;
public:
    Trie() {root = new TrieNode();}
    bool insert(int, const char*, size_t);
    std::set<int>* search(const char*, size_t);

};


#endif //UPLOAD_SIMSEARCHER_TRIE_H
