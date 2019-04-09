#include "SimSearcher.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

SimSearcher::SimSearcher()
{
}

SimSearcher::~SimSearcher()
{
}

inline unsigned long long my_hash(const char* s,int len)
{
	unsigned long long hash = 0;
	int seed = 131;
	for (int i = 0; i < len; i++) {
		hash = hash * seed + (int)s[i];
	}
	return hash & 0x7FFFFFFF;
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{
	char buf[1024];
	FILE* file = fopen(filename,"r");
	for(line_count=0;fgets(buf,1024,file);++line_count){

		printf("insert jacc\n");
		printf("read: %s len:%d\n",buf,strlen(buf));
		//jaccard: split by spaces and insert into jacTrie
		char* pch = strtok (buf," \r\n");
		while (pch != nullptr)
		{
		    if(jacTrie.insert(line_count,pch,strlen(pch)))
                ++word_count[line_count];
			pch = strtok (nullptr, " \r\n");
		}
		printf("insert ed\n");

		//ED: insert adjacent q words into edTrie
		this->q=q;
		for(int j = 0; buf[j+q-1]!='\0'; ++j){
			edTrie.insert(line_count, buf+j, q);
		}


		std::set<int> aset = *jacTrie.search("line",4);
		for (auto it=aset.begin(); it!=aset.end(); it=aset.upper_bound(*it)){
			printf("%d ", *it);
		}
        for(int i2 = 0;i2 < 5; ++ i2){
            printf("find:%d\n",aset.count(i2));
		}


	}
	return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();


	//copy query into query_split to avoid modifying it in 'strtok'
	char* query_split;
	memcpy(query_split,query,strlen(query));
	int query_count = 0;

	//split query into words; then find them in jacTrie
	//search function accepts a word and returns a small set
	//containing ids of lines that owns this word.
	//to avoid multiple words, arrange their pointer in sets.
	//then count each pointer.
	std::set<std::set<int>*> word_sets;

	//avoid repeated words
	Trie query_trie;
	char* pch = strtok(query_split," \r\n");
	while (pch != nullptr)
	{
		if(!query_trie.insert(0,pch,strlen(pch))) continue;
		//count number of words
		++query_count;
		//search for this word in jacTrie
		std::set<int>* word_inverse_set = jacTrie.search(pch,strlen(pch));
		if(word_inverse_set != nullptr){
			word_sets.insert(word_inverse_set);
		}
		pch = strtok (nullptr, " \r\n");
	}


	//find differences between numbers of a word in query and in entry.
	//iterate through each word common in query and jacTrie

	//diff of all lines
	int same[50000];
	for (auto it=word_sets.begin(); it!=word_sets.end(); it=word_sets.upper_bound(*it)){
		
		//number of this word in query:
		unsigned long long query_num = word_sets.count(*it);
		std::set<int> it_setp = **it;
		for (auto it2=it_setp.begin(); it2!=it_setp.end(); it2=it_setp.upper_bound(*it2)) {
			unsigned long long entry_num = it_setp.count(*it2);
			same[*it2]+=min(query_num, entry_num);
		}
	}
	for(int i = 0; i < line_count; ++i){
		double jaccard = same[i] / (double)(query_count + word_count[i] - same[i]);
		if(jaccard > threshold)
            result.emplace_back(make_pair(i, jaccard));
	}


	return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	result.clear();


	int query_count = 0;

	//split query into words; then find them in jacTrie
	//search function accepts a word and returns a small set
	//containing ids of lines that owns this word.
	//to avoid multiple words, arrange their pointer in sets.
	//then count each pointer.
	std::set<std::set<int>*> word_sets;

	//avoid repeated words
	Trie query_trie;
	for(const char* pch = query; pch[q] != '\0'; ++pch)
	{
		if(!query_trie.insert(0,pch,(size_t)q)) continue;
		//count number of words
		++query_count;
		//search for this word in edTrie
		std::set<int>* word_inverse_set = edTrie.search(pch,(size_t)q);
		//...this one needs multiset, it seems...
		if(word_inverse_set != nullptr){
			word_sets.insert(word_inverse_set);
		}
		pch = strtok (nullptr, " \r\n");
	}


	//find differences between numbers of a word in query and in entry.
	//iterate through each word common in query and jacTrie

	//diff of all lines
	int same[50000];
	for (auto it=word_sets.begin(); it!=word_sets.end(); it=word_sets.upper_bound(*it)){

		//number of this word in query:
		unsigned long long query_num = word_sets.count(*it);
		std::set<int> it_setp = **it;
		for (auto it2=it_setp.begin(); it2!=it_setp.end(); it2=it_setp.upper_bound(*it2)) {
			unsigned long long entry_num = it_setp.count(*it2);
			same[*it2]+=min(query_num, entry_num);
		}
	}
	for(int i = 0; i < line_count; ++i){
		double jaccard = same[i] / (double)(query_count + word_count[i] - same[i]);
		if(jaccard > threshold)
            result.emplace_back(make_pair(i, jaccard));
	}


	return SUCCESS;
}

