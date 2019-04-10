#include "SimSearcher.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	SimSearcher searcher;

	vector<pair<unsigned, unsigned> > resultED;
	vector<pair<unsigned, double> > resultJaccard;

	unsigned q = 2, edThreshold = 6;
	double jaccardThreshold = 0.1;

	printf("%d\n",searcher.createIndex("sample.txt", q));
	searcher.searchJaccard("query", jaccardThreshold, resultJaccard);
	printf("jac\n");
	for(auto i : resultJaccard){
		printf("jac: %d %lf\n",i.first, i.second);
	}
	searcher.searchED("query", edThreshold, resultED);
	for(auto i : resultED){
		printf("ED: %d %d\n",i.first, i.second);
	}

	return 0;
}

