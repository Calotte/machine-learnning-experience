#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<map>
#include<utility>
#include<algorithm>
#include<ctime>
#include"FPNode.h"
#include"Comp.h"
using std::ifstream; using std::cout; using std::endl;
using std::vector; using std::string; using std::map;
using std::pair; using std::cin;
void getDatabase(const string& file, vector<vector<int>>& out);
void FPGrowth(vector<vector<int>>& D, int min_sup, FPNode* T, 
	map<pair<int, int>, list<FPNode*>, CompMap>& L);
void FP_growth(map<pair<int, int>, list<FPNode*>, CompMap>& L, map<vector<int>, int>& S,
	FPNode* T, int item, int min_sup);
void printMap(map<vector<int>, int>& S);
int main() {
	vector<vector<int>> database;
	cout << "loading..." << endl;
	getDatabase("data.txt", database);
	cout << "load completed!" << endl;
	cout << "computing..." << endl;
	FPNode* root=new FPNode(-1,nullptr);
	map<pair<int, int>, list<FPNode*>, CompMap> L;  //频繁项集列表
	map<vector<int>, int> S;
	int min_sup =400;
	long long begin = clock();
	FPGrowth(database, min_sup, root, L);
	FP_growth(L, S, root, -1, min_sup);
	long long end = clock();
	cout << "耗时：" << end - begin << " ms" << endl;
	cout << "最小支持度阈值：" << min_sup << endl;
	cout << "****************************************" << endl;
	printMap(S);
	return 0;
}
void printMap(map<vector<int>, int>& S) {
	S.clear();
	ifstream is("test.txt"); 
	int v1, v2, cnt;
	while (is>> v1 >> v2 >> cnt) {
		vector<int> v; v.push_back(v1); v.push_back(v2);
		S[v] = cnt;
	}
	auto it = S.begin();
	for (;it != S.end();++it) {
		for (const auto i : it->first) {
			if(i>0)
			cout << i << " ";
		}			
		cout << " support count:" << it->second << endl;
	}
}
void getDatabase(const string& file, vector<vector<int>>& out) {
	ifstream in(file);
	string line;
	int item;
	while (getline(in, line)) {
		std::istringstream trans(line);
		vector<int> items;
		while (trans >> item)
			items.push_back(item);
		out.push_back(items);
	}
}
void findFrequentItem_1(vector<vector<int>>& D, int min_sup,
	map<pair<int, int>, list<FPNode*>,CompMap>& item_heads) {
	map<int, int> temp;
	for (const vector<int>& vi : D) {
		for (int i : vi)
			++temp[i];
	}
	auto it = temp.cbegin();
	for (it;it != temp.cend();++it) {
		if (it->second >= min_sup) {
			pair<int, int> p;
			p.first = it->first;
			p.second = it->second;
			list<FPNode*> f;
			item_heads.insert({p,f});
		}
	}
}
bool inL(int item, const map<pair<int, int>, list<FPNode*>, CompMap>& L);
void insertTree(vector<int>& P, FPNode* fp, map<pair<int, int>, list<FPNode*>, CompMap>& L);
void FPGrowth(vector<vector<int>>& D, int min_sup, FPNode* T,
	map<pair<int, int>, list<FPNode*>, CompMap>& L)
{
	findFrequentItem_1(D, min_sup, L);
	vector<int> trans;
	for (const vector<int>& v : D) {
		for (const int item : v)
			if (inL(item, L))
				trans.push_back(item);
		std::sort(trans.begin(), trans.end(), comp);
		insertTree(trans, T,L);
	}
}
bool inL(int item, const map<pair<int, int>, list<FPNode*>, CompMap>& L) {
	auto it = L.begin();
	for (;it != L.end();it++)
		if ((it->first).first == item)
			return true;
	return false;
}
FPNode* findChild(int i, const FPNode* fp) {
	for (FPNode* fp : fp->childs)
		if (fp->item_name == i)
			return fp;
	return nullptr;
}
void insertTree(vector<int>& P, FPNode* T, map<pair<int, int>, list<FPNode*>, CompMap>& L){
	auto it = P.begin();
	if (it != P.end()) {
		FPNode* f = findChild(*it, T);
		if(f==nullptr){
			FPNode* fpn = new FPNode(*it,T);
			(T->childs).push_back(fpn);
			auto itL = L.begin();
			for (;itL != L.end();itL++)
				if ((itL->first).first == fpn->item_name)
					(itL->second).push_back(fpn);
			it=P.erase(it);
			insertTree(P, fpn,L);
		}
		else {
			f->count++;
			it=P.erase(it);
			insertTree(P, f,L);
		}
	}
}
bool singlePath(FPNode* T) {
	if ((T->childs).size() == 0) return true;
	if ((T->childs).size() > 1)
		return false;
	else {
		auto p = (T->childs).begin();
		FPNode* fp = *p;
		while ((fp->childs).size()) {
			if ((fp->childs).size() > 1) return false;
			p = (fp->childs).begin();
			fp = *p;
		}
		return true;
	}
}
void subset(vector<pair<int,int>>& t, int size, int beg, int k, vector<int>& v,
	map<vector<int>, int>& S) {//产生所有大小为k的子集
	int min = t[beg].second;
	if (k == 0) {
		S.insert({v,min});
		return;
	}
	for (int i = beg;i < size;i++) {
		if (t[i].second < min) min = t[i].second;
		v.push_back(t[i].first);
		subset(t, size, i + 1, k - 1, v, S);
		v.pop_back();
	}
}
FPNode*  conPattern(pair<pair<int, int>, list<FPNode*>> item, 
	FPNode* T,int min_sup, map<pair<int, int>, list<FPNode*>, CompMap>& L) {
	auto p = item.second;
	auto it = p.begin();
	vector<vector<int>> d;
	for (;it != p.end();++it) {
		FPNode* fp = *it;
		vector<int> v;   
		while (fp->item_name != -1) {
			v.push_back(fp->item_name);
			fp = fp->parent;
		}
		d.push_back(v);
		v.clear();
	}
	FPNode* tree=new FPNode(-1, nullptr);
	FPGrowth(d, min_sup, tree, L);
	return tree;
}
void FP_growth(map<pair<int, int>, list<FPNode*>, CompMap>& L,
	map<vector<int>, int>& S, FPNode* T, int item, int min_sup){
	if (singlePath(T)) {
		vector<int> v; int min = T->count;
		vector<pair<int, int>> s;
		if (item != -1)
			v.push_back(item);
		v.push_back(T->item_name);
		s.push_back({ T->item_name, T->count });
		FPNode* fp = T;
		while ((fp->childs).size() > 0) {
			fp = *((fp->childs).begin());
			v.push_back(fp->item_name);
			s.push_back({ fp->item_name, fp->count });
			if (fp->count < min)
				min = fp->count;
		}
		S.insert({ v,min });
		for (size_t k = 2;k < s.size();k++) {
			vector<int> v;
			subset(s, s.size(), 0, k, v, S);
		}
	}
	else {
		auto rit = L.rbegin();
		for (;rit != L.rend();++rit) {
			map<pair<int, int>, list<FPNode*>, CompMap> l;
			FPNode* t = conPattern(*rit, T, min_sup, l);
			if ((t->childs).size()) {
				auto p = rit->second;
				auto pit = *(p.begin());
				FP_growth(l, S, t, pit->item_name, min_sup);
			}
		}
	}
}
