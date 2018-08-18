#include <iostream>
#include<fstream>
#include<sstream>
#include <cmath>
#include <vector>
#include<map>
#include<set>
#include <string>
#include <algorithm>
#include"Node.h"
using namespace std;
void generateDecisionTree(vector<vector<int>>& examples, Node& tree);
void printTree(const Node& tree, int depth);
void readData(const string& file, vector<vector<int>>& rules);
int main()
{
	vector<vector<int> > rules;
	readData("data.txt", rules);
	Node tree;
	generateDecisionTree(rules, tree);
	cout << "**********************" << endl;
	cout << "* "<<"Wait Decision Tree "<<"*"<<endl;
	cout << "**********************" << endl;
	printTree(tree, 0);
	return 0;
}
void readData(const string& file, vector<vector<int>>& rules) {
	ifstream in(file);
	if (!in) cout << "open file error!" << endl;
	else {
		string line;
		while (getline(in, line)) {
			istringstream record(line);
			unsigned val; vector<int> v;
			while (record >> val)
				v.push_back(val);
			rules.push_back(v);
			v.clear();
		}
	}
}
template <typename T>
vector<T> unique(const vector<T>& vals)
{
	set<T> s;
	vector<T> get_unic;
	s.insert(vals.begin(), vals.end());
	get_unic.insert(get_unic.begin(),s.begin(), s.end());
	return get_unic;
}
double computeEntropy(const vector<int>& v)
{
	map<int, int> m;
	double entropy = 0.0;
	int total = v.size();
	for (const auto& i : v)
		m[i]++;
	map<int, int>::iterator it = m.begin();
	while (it != m.end()) {
		double cnt = it->second;
		entropy -= cnt / total * log2(cnt / total);
		++it;
	}
	return entropy;
}
void computeGain(const vector<vector<int>>& trains, vector<double>&gain)
{
	int total = trains.size();
	//vector<double> gain(trains[0].size() - 1, 0);
	vector<int> attribute_vals;
	vector<int> dec;
	vector<vector<int>>::const_iterator it = trains.begin();
	while(it!=trains.end()){
		dec.push_back(it->back());
		++it;
	}
	double HN= computeEntropy(dec);
	for (size_t i = 0; i < trains[0].size() - 1; i++){
		for (size_t j = 0; j < trains.size(); j++)
			attribute_vals.push_back(trains[j][i]);
		set<int> unique_vals;
		unique_vals.insert(attribute_vals.begin(), attribute_vals.end());
		set<int>::iterator itr = unique_vals.begin();
		vector<int> subset;
		while (itr != unique_vals.end())
		{
			for (size_t k = 0; k < trains.size(); k++)
			{
				if (*itr == attribute_vals[k])
					subset.push_back(trains[k].back());
			}
			double ti= (double)subset.size();
			gain[i] += ti/total * computeEntropy(subset);
			itr++;
			subset.clear();
		}
		gain[i] =HN-gain[i];
		attribute_vals.clear();
	}
	//return gain;
}
template <typename T>
T findMostCommon(const vector<vector<T>>& data)
{
	map<T, int> m;
	auto it_data = data.begin();
	while (it_data != data.end()) {
		m[it_data->back()]++;
		++it_data;
	}
	T most;
	int most_cnt= 0;
	map<T, int>::iterator mit = m.begin();
	while (mit != m.end()) {
		if (mit->second > most_cnt) {
			most = mit->first;
			most_cnt = mit->second;
		}
		++mit;
	}
	return most;
}
template <typename T>
vector<T> findAttributeValues(T attribute, const vector<vector<T>>& data)
{
	vector<T> values;
	for (size_t i = 0; i < data.size(); i++){
		values.push_back(data[i][attribute]);
	}
	return unique(values);
}
template <typename T>
void dropOneAttribute(T attribute, vector<vector<T>>& data){
	for (size_t i = 0; i < data.size(); i++)
		data[i][attribute] =-1;
}
bool isSame(const vector<vector<int>>& trains, vector<int>& tags) {
	vector<vector<int>>::const_iterator it = trains.begin();
	while (it != trains.end()) {
		tags.push_back(it->back());
		++it;
	}
	if (unique(tags).size() == 1)
		return true;
	else 
		return false;
}
void generateDecisionTree(vector<vector<int>>& trains,Node& tree){
	vector<int> tags;
	if(isSame(trains,tags))
	{
		tree.root = tags[0];
		return;
	}
	if (count(trains[0].begin(), trains[0].end(),-1) == trains[0].size() - 1)
	{
		tree.root = findMostCommon(trains);
		return;
	}
	vector<double> standard(trains[0].size() - 1, 0.0);
	computeGain(trains, standard);
	tree.root = 0;
	for (size_t i = 0; i < standard.size(); i++)
	{
		if (standard[i] >= standard[tree.root] && trains[0][i] !=-1)
			tree.root = i;
	}
	tree.values = findAttributeValues(tree.root, trains);
	vector<vector<int>> new_examples(trains.begin(), trains.end());
	dropOneAttribute(tree.root, new_examples);
	vector<vector<int>> subset;
	for (size_t i = 0; i < tree.values.size(); i++)
	{
		for (size_t j = 0; j < trains.size(); j++)
			for (size_t k = 0; k < trains[0].size(); k++)
				if (tree.values[i] == trains[j][k])
					subset.push_back(new_examples[j]);
		Node new_tree;
		generateDecisionTree(subset, new_tree);
		tree.children.push_back(new_tree);
		subset.clear();
	}
}
void printTree(const Node& tree, int depth)
{
	for (int d = 0; d < depth; d++) 
		cout << "\t";
	if (!tree.values.empty())
	{
		cout << attribute_names[tree.root] << endl;
		for (size_t i = 0; i < tree.values.size(); i++)
		{
			for (int d = 0; d < depth + 1; d++) cout << "\t";
			cout << attribute_values[tree.values[i]] << endl;
			printTree(tree.children[i], depth + 2);
		}
	}
	else 
		cout << attribute_values[tree.root] << endl;
}