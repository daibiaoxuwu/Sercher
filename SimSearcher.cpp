#include "SimSearcher.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

SimSearcher::SimSearcher()
{
	word_count = new int[50000];
	char_count = new int[50000];
	lines = new char*[50000];
	memset(word_count, 0, 50000*sizeof(int));
	memset(char_count, 0, 50000*sizeof(int));
}

SimSearcher::~SimSearcher()
{
	delete[](word_count);
	delete[](char_count);
	for(int i = 0; i < line_count; ++ i){
		delete[](lines[i]);
	}
	delete[](lines);
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


		if(buf[strlen(buf)-1]=='\n'){
			buf[strlen(buf)-1]='\0';
		}
		printf("str:%s\n",buf);
		char_count[line_count] = strlen(buf);
		lines[line_count] = new char[strlen(buf)+1];
		memcpy(lines[line_count],buf,strlen(buf)+1);



		//ED: insert adjacent q words into edTrie
		this->q=q;
		for(int j = 0; buf[j+q-1]!='\0'; ++j){
			edTrie.insert(line_count, buf+j, q);
		}


		//printf("%s count:%d copied to %s\n",buf,strlen(buf),lines[line_count]);

		//jaccard: split by spaces and insert into jacTrie
		char* pch = strtok (buf," \r\n");
		while (pch != nullptr)
		{
		    if(jacTrie.insert(line_count,pch,strlen(pch)))
                ++word_count[line_count];
			pch = strtok (nullptr, " \r\n");
		}



        /*
		std::multiset<int> aset = *jacTrie.search("line",4);
		for (auto it=aset.begin(); it!=aset.end(); it=aset.upper_bound(*it)){
			printf("%d ", *it);
		}
        for(int i2 = 0;i2 < 5; ++ i2){
            printf("find:%d\n",aset.count(i2));
		}
        */


	}
	return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();


	//copy query into query_split to avoid modifying it in 'strtok'
	char* query_split = new char[strlen(query)+1];
	memcpy(query_split,query,strlen(query)+1);
	int query_count = 0;

	//split query into words; then find them in jacTrie
	//search function accepts a word and returns a small multiset
	//containing ids of lines that owns this word.
	//to count multiple words, arrange their pointer in multisets.
	//then count each pointer.
	std::multiset<std::multiset<int>*> word_sets;

	//count multiple words
	Trie query_trie;
	char* pch = strtok(query_split," \r\n");
	while (pch != nullptr)
	{
		if(!query_trie.insert(0,pch,strlen(pch))) continue;
		//count number of words
		++query_count;
		//search for this word in jacTrie
		std::multiset<int>* word_inverse_set = jacTrie.search(pch,strlen(pch));
		if(word_inverse_set != nullptr){
			word_sets.insert(word_inverse_set);
		}
		pch = strtok (nullptr, " \r\n");
	}


	//find differences between numbers of a word in query and in entry.
	//iterate through each word common in query and jacTrie

	//diff of all lines
	int* same = new int[50000];
	memset(same, 0, 50000*sizeof(int));
	for (auto it=word_sets.begin(); it!=word_sets.end(); it=word_sets.upper_bound(*it)){

		//number of this word in query:
		unsigned long long query_num = word_sets.count(*it);
		std::multiset<int> it_setp = **it;
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
	delete[](same);


	return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	result.clear();


	int query_count = (int)strlen(query);

	//split query into words; then find them in jacTrie
	//search function accepts a word and returns a small multiset
	//containing ids of lines that owns this word.
	//to count multiple words, arrange their pointer in sets.
	//then count each pointer.
	std::multiset<std::multiset<int>*> word_sets;

	//count multiple words
	Trie query_trie;
	for(const char* pch = query; pch[q-1] != '\0'; ++pch)
	{
		//search for this word in edTrie
		std::multiset<int>* word_inverse_set = edTrie.search(pch,(size_t)q);
		//...this one needs multiset, it seems...
		if(word_inverse_set != nullptr){
			word_sets.insert(word_inverse_set);
		}
	}


	//find differences between numbers of a word in query and in entry.
	//iterate through each word common in query and jacTrie

	//diff of all lines
	int* same=new int[50000];
	memset(same, 0, 50000*sizeof(int));
	for (auto it=word_sets.begin(); it!=word_sets.end(); it=word_sets.upper_bound(*it)){

		//number of this word in query:
		unsigned long long query_num = word_sets.count(*it);
		std::multiset<int> it_setp = **it;
		for (auto it2=it_setp.begin(); it2!=it_setp.end(); it2=it_setp.upper_bound(*it2)) {
			unsigned long long entry_num = it_setp.count(*it2);
			same[*it2]+=min(query_num, entry_num);
		}
	}
	for(int i = 0; i < line_count; ++i){

		//too long or too short
		if(abs(query_count-char_count[i])>threshold) continue;


		double ed_max = query_count - q + 1 + char_count[i] - q + 1 - (same[i] * 2);
		if(ed_max <= threshold * q){
			int ed_value = calculate_ED(query, lines[i], (int) threshold);
			if (ed_value <= threshold)
				result.emplace_back(make_pair(i, ed_value));
		}
	}
	delete[](same);


	return SUCCESS;
}

int SimSearcher::calculate_ED(const char *query, char *line, int threshold){
    printf("%s %s\n",query,line);
	int query_len = (int)strlen(query);
	int line_len = (int)strlen(line);
	int data[query_len+1][line_len+1];
	for(int i = 0; i <= min(threshold+1, query_len); ++i) data[i][0]=i;
	for(int i = 0; i <= min(threshold+1, line_len); ++i) data[0][i]=i;

	int left = 1, right = min(threshold + 1, line_len);
	//endmax
	data[query_len][line_len] = threshold + 1;
	for(int i = 1; i <= query_len; ++ i){
		printf("%d %d\n",left,right);
		for(int i = 0; i <= query_len; ++ i, printf("\n"))
			for(int j = 0; j <= line_len; ++ j)
				printf("%d\t",data[i][j]);
		//not found
		if(left>right) return threshold + 1;

		//left max
		data[i][left-1] = 9;

		//narrow range for next round
		bool leftflag = true;
		int next_right = left;

		for(int j = left; j <= right; ++ j){
			data[i][j] = min(min(data[i-1][j-1] + (query[i-1] != line[j-1]),
					data[i-1][j] + 1), data[i][j-1]+1);
			//narrow the left and right for next iter
			if(data[i][j]>threshold){
				if(leftflag) left = j;
			} else {
				leftflag = false;
				next_right = j;
			}
		}
		right = next_right+1;
		//right max
		data[i][right] = 9;

	}
	for(int i = 0; i <= query_len; ++ i, printf("\n"))
		for(int j = 0; j <= line_len; ++ j)
			printf("%d\t",data[i][j]);
	return data[query_len][line_len];

}

