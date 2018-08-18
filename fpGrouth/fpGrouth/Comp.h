#pragma once
#include<utility>
using std::pair;
struct CompMap {
	bool operator()(const pair<int,int>& k1, const pair<int, int>& k2) {
		return k1.second > k2.second;
	}
};

struct CompVec {
	bool operator()(const int i1, const int i2) {
		return i1 > i2;
	}
}comp;