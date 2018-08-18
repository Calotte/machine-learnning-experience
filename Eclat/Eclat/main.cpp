#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include <algorithm>
#include<ctime>
#define MAX 2000
using namespace std;
void getDatabase(const string& file, vector<vector<int>>& out);
void eclat(const vector<vector<int>>& out, map<int, vector<int>>& db);
void findFrequentSet(const map<int, vector<int>>& db, const int min_sup,
	vector<map<vector<int>, vector<int>>>& L);
void output(const map<vector<int>, vector<int>>& L, long long end,
	long long begin, int minSup);
int main() {
	int min = 700;
	vector<map<vector<int>, vector<int>>> S;
	vector<vector<int>> database;
	map<int, vector<int>> db_eclat;
	getDatabase("data.txt", database);
	long long begin = clock();
	eclat(database, db_eclat);
	findFrequentSet(db_eclat, min, S);
	long long end = clock();
	cout << "ºÄÊ±£º" << end - begin << " ms" << endl;
	if (S.size() > 0) {
		map<vector<int>, vector<int>> m = S.back();
		map<vector<int>, vector<int>>::iterator it = m.begin();
		while (it != m.end()) {
			for (int i : it->first)
				cout << i << " ";
			cout <<" support count="<<(it->second).size()<< endl;
			++it;
		}
		output(m, end, begin, min);
	}		
	return 0;
}
std::ostream& operator<<(std::ostream& os, const vector<int>& iv) {
	for (const int i : iv)
		os << i << " ";
	return os;
}
void output(const map<vector<int>, vector<int>>& L, long long end,
	long long begin, int minSup) {
	std::ofstream out("out.txt", std::ofstream::app);
	out << "ºÄÊ±£º" << end - begin << " ms" << endl;
	auto it = L.cbegin();
	if (it == L.cend())
		out << "null set :(" << endl;
	else {
		out << "for min_sup=" << minSup << "   L:" << endl;
		out << "****************************************" << endl;
		while (it != L.cend()) {
			out << it->first << endl;
			out << "support count: " << (it->second).size() << endl;
			++it;
		}
		out << "****************************************" << endl;
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
void eclat(const vector<vector<int>>& out, map<int, vector<int>>& db) {
	int trans = 0;
	for (const auto& v : out) {
		trans++;
		for (const auto& i : v)
			db[i].push_back(trans);
	}
}
void findFrequent1Items(const map<int, vector<int>>& db, int min_sup,
	map<vector<int>, vector<int>>& C1) {
	map<int, vector<int>>::const_iterator it = db.begin();
	while (it != db.end()) {
		if ((it->second).size() >= min_sup) {
			vector<int> t;
			t.push_back(it->first);
			C1.insert({ t,it->second });
		}
		++it;
	}
}
void genMore(map<vector<int>, vector<int>>& LK, 
	map<vector<int>, vector<int>>& more, const int min_sup,int k) {
	vector<int> key, value(MAX);
	vector<int>::iterator it_value;
	for (auto it = LK.begin();it != LK.end();++it) {
		auto it2 = it; ++it2;
		for (;it2 != LK.end();++it2) {
			if (k == 1) {
				it_value= std::set_intersection((it->second).begin(), (it->second).end(),
					(it2->second).begin(), (it2->second).end(), value.begin());
				value.resize(it_value- value.begin());
				if (value.size() >= min_sup) {
					key.push_back(it->first[0]);
					key.push_back(it2->first[0]);
					more[key] = value;
				}
			}
			else {
				if (equal((it->first).begin(), (it->first).end() - 1, (it2->first).begin())
					&& (it->first).back() < (it2->first).back()) {
					it_value = std::set_intersection((it->second).begin(), (it->second).end(),
						(it2->second).begin(), (it2->second).end(), value.begin());
					value.resize(it_value - value.begin());
					if (value.size() >= min_sup) {
						for (int i : it->first)
							key.push_back(i);
						key.push_back((it2->first).back());
						more[key] = value;
					}
				}
			}
			key.clear(); value.clear();
			value.resize(MAX);
			it_value = value.begin();
		}
	}
}
void findFrequentSet(const map<int, vector<int>>& db, const int min_sup,
	vector<map<vector<int>, vector<int>>>& L) {
	map<vector<int>, vector<int>> items;
	map<int, vector<int>>::const_iterator it = db.begin();
	map<vector<int>, vector<int>> l1;
	findFrequent1Items(db, min_sup, l1);
	L.push_back(l1);
	size_t k = 1;
	for (;;) {
		map<vector<int>, vector<int>> m;
		m.clear();
		genMore(L[k - 1], m,min_sup,k);
		if (m.empty()) break;
		else {
			L.push_back(m);
			++k;
		}
	}
}