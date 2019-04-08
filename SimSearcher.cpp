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

		printf("insert jacc\n");
		//jaccard: split by spaces and insert into jacTrie
		char* pch = strtok (buf," \r\n");
		while (pch != nullptr)
		{
		    jacTrie.insert(i,pch,strlen(pch));
			pch = strtok (nullptr, " \r\n");
		}
		printf("insert ed\n");
		//ED: insert adjacent q words into edTrie
		this->q=q;
		for(int j = 0; buf[j+q-1]!='\0'; ++j){
			edTrie.insert(i, buf+j, q);
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

