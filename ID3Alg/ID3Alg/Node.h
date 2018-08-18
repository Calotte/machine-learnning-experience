#pragma once
#include<vector>
#include<string>
struct Node{
	int root;
	std::vector<int> values;
	std::vector<Node> children; 
};
std::vector<std::string> attribute_names={ "alt","bar","Fri/Sat","hungry","Patrons",
"Price","rain","res","Type","WaitEstimate"};
std::vector<std::string> attribute_values= { "T", "F","None", "Some", "Full", "$",
"$$", "$$$","French","Thai","Burger","Italian","0-10","10-30","30-60",">60"};