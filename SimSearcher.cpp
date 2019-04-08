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

int SimSearcher::createIndex(const char *filename, unsigned q)
{
	char buf[1024];
	FILE* file = fopen(filename,"r");
	for(int i=0;fgets(buf,1024,file);++i){
		char* pch = strtok (buf," \r\n");
		while (pch != nullptr && strlen(pch)>1)
		{
		    trie.insert(i,pch);
			pch = strtok (nullptr, " \r\n");
		}
	}
	return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();
	return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	result.clear();
	return SUCCESS;
}

