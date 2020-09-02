#ifndef gd_hpp
#define gd_hpp

#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#define LINEWORD 200
using namespace std;

float stringtofloat(string word);
vector<float> linetoarray(string line);
vector<vector<float> > getdata(char* filename); 

#endif
