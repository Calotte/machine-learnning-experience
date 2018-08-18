#pragma once
#include<list>
using std::list;
struct FPNode {
	FPNode() { item_name = -1; parent = nullptr; count = 0; }
	FPNode(int name, FPNode* p) { item_name = name; parent = p; count = 1; }
	int item_name;
	int count;
	list<FPNode*> childs;
	FPNode* parent;
};