#pragma once
#include <vector>
#include <utility>
#include "Trie.h"
const int SUCCESS = 0;
const int FAILURE = 1;

class SimSearcher
{
	Trie trie;

public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);
};

