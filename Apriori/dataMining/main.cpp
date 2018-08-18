#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<map>
#include<utility>
#include<algorithm>
#include<ctime>
using std::ifstream; using std::cout; using std::endl;
using std::vector; using std::string; using std::map;
using std::pair;  
std::ostream& operator<<(std::ostream& os, const vector<int>& iv);
void getDatabase(const string& file, vector<vector<int>>& out);
void findFrequent1Items(vector<vector<int>>& D,int min_sup, map<vector<int>, int>& C1);
map<vector<int>, int> aprioriGen(map<vector<int>, int>& Lk,int k);
bool hasInfrequentSubset(pair<vector<int>, int> c, map<vector<int>, int>& L);
map<vector<int>, int> findFrequent(vector<vector<int>>& D, int min_sup);
vector<vector<int>> genSubset(vector<int>& t);
void subset(vector<int>& t, int size, int beg, int k, 
	vector<int>& v, vector<vector<int>>& vv);
void output(const map<vector<int>, int>& L, long long end, long long begin, int minSup);
int main() {
vector<vector<int>> database;
cout << "loading..." << endl;
getDatabase("data.txt", database);
cout << "load completed!" << endl;
cout << "computing..." << endl;
int minSup= 900;
long long begin = clock();
map<vector<int>, int> L=findFrequent(database, minSup);
long long end = clock();
cout << "耗时：" << end - begin << " ms" << endl;
output(L, end, begin, minSup);
auto it = L.cbegin();
if (it == L.cend())
cout << "null set :(" << endl;
else {
	cout << "for min_sup="<<minSup<<"   L:" << endl;
	cout << "****************************************" << endl;
	while (it != L.cend()) {
		cout << it->first << endl;
		cout << "support count: " << it->second << endl;
		++it;
	}
	cout << "****************************************" << endl;
}
return 0;
}
std::ostream& operator<<(std::ostream& os, const vector<int>& iv) {
	for (const int i : iv)
		os << i << " ";
	return os;
}
void output(const map<vector<int>, int>& L, long long end, long long begin, int minSup) {
	std::ofstream out("out.txt", std::ofstream::app);
	out << "耗时：" << end - begin << " ms" << endl;
	auto it = L.cbegin();
	if (it == L.cend())
		out << "null set :(" << endl;
	else {
		out << "for min_sup=" << minSup << "   L:" << endl;
		out << "****************************************" << endl;
		while (it != L.cend()) {
			out << it->first << endl;
			out << "support count: " << it->second << endl;
			++it;
		}
		out << "****************************************" << endl;
	}
}
map<vector<int>, int> findFrequent(vector<vector<int>>& D, int min_sup) {
	vector<map<vector<int>, int>> L;
	map<vector<int>, int> c;
	findFrequent1Items(D, min_sup, c);
	L.push_back(c);
	size_t k = 1;
	for (k;!L[k - 1].empty();k++) {
		map<vector<int>, int> Ck = aprioriGen(L[k - 1],k);
		auto it = Ck.begin();
		for (vector<int> item : D) {
			vector<int> v;
			vector<vector<int>> Ct;
			if (item.size() > k) {
			subset(item, item.size(), 0, k+1,v,Ct);  //对每一个事务产生k大小的子集
				for (auto it = Ck.begin();it != Ck.end();++it)      //对与每个候选进行计数
					if (find(Ct.begin(), Ct.end(), it->first) != Ct.end())
						++it->second;
			}
		}
		map<vector<int>, int> Ctemp;
		L.push_back(Ctemp);
		for (auto it = Ck.begin();it != Ck.end();++it)
			if (it->second >= min_sup)
				L[k].insert(*it);
	}
	return k==1?L[0]:L[k - 2];
}
map<vector<int>, int> aprioriGen(map<vector<int>, int>& Lk,int k) {//产生候选项集
	map<vector<int>, int> Ck;
	vector<int> c;
	for (auto it = Lk.begin();it != Lk.end();++it) {
		auto it2 = it; ++it2;
		for (;it2 != Lk.end();++it2) {
			if (k == 1) {
				c.push_back(it->first[0]);
				c.push_back(it2->first[0]);
			}
			else {
				if (equal((it->first).begin(), (it->first).end() - 1, (it2->first).begin())
					&& (it->first).back()<(it2->first).back()) {
					for (int i : it->first)
						c.push_back(i);
					c.push_back((it2->first).back());
			}
			}
			if (!hasInfrequentSubset({ c,0 }, Lk))
				Ck.insert({ c,0 });
			c.clear();
		}
	}
	return Ck;
}
bool hasInfrequentSubset(pair<vector<int>, int> c, map<vector<int>, int>& L) {
	vector<vector<int>> s;
	vector<int> iv;
	subset(c.first, c.first.size(), 0, c.first.size() - 1, iv, s);
	for (const auto& v : s) {
		auto it = L.begin();
		for (;it != L.end();it++)
			if (it->first == v)
				break;
		if (it == L.end()) return true;
	}
	return false;
}
vector<vector<int>> genSubset(vector<int>& t) {
	vector<vector<int>> ss;
	vector<int> s;
	int len = t.size();
	for (int i = 0;i < len;++i) {
		for (int j = 0;j < len;j++)
			if (j != i) s.push_back(t[j]);
		ss.push_back(s);
		s.clear();
	}
	return ss;
}
void findFrequent1Items(vector<vector<int>>& D, int min_sup,
	map<vector<int>, int>& C1) {
	//map<vector<int>, int> C1;
	map<int, int> temp;
	for (const vector<int>& vi : D) {
		for (int i : vi)
			++temp[i];
	}
	auto it = temp.cbegin();
	for (it;it != temp.cend();++it) {
		if (it->second >= min_sup) {
			vector<int> t;
			t.push_back(it->first);
			C1.insert({ t,it->second });
		}
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
void subset(vector<int>& t, int size, int beg, int k, vector<int>& v, 
	vector<vector<int>>& vv) {
	if (k == 0) {
		vv.push_back(v);
		return;
	}
	for (int i = beg;i < size;i++) {
		v.push_back(t[i]);
		subset(t, size, i + 1, k - 1, v, vv);
		v.pop_back();
	}
}