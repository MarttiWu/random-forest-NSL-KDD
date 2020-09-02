//
//  RandomForest.hpp
//  test
//
//  Created by 吳孟庭 on 2020/6/11.
//  Copyright © 2020 吳孟庭. All rights reserved.
//

#ifndef RandomForest_hpp
#define RandomForest_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <queue>
#include <fstream>
#include"getdata.hpp"
#include "DecisionTree.hpp"
#include <ctime>
#include <cstdlib>
#include <memory.h> 
#include <ctime>

#define FORESTSIZE 20 
#define MINDIM 1 
#define MAXDIM 20 

using namespace std;




class RF;
class RFT;

class RFTreeNode{
public:
    RFTreeNode(vector<vector<float> > d);
    friend class RF;
    friend class RFT;
protected:
    RFTreeNode *parent,*leftchild,*rightchild;
    vector<vector<float> > data;
    int *nkey;  //nkey[0] is dimension nkey[1] is which point to split
    float pred_num;
    float gini;
};


class RFT{ 
public:
    RFT(vector<vector<float> > data);
    float calGini(RFTreeNode *node,int index1,int index2,vector<vector<float> > &sdata);
    void creatbootstrap(vector<vector<float> > data);
    int predict();
    int *test_split(RFTreeNode *node,int numDim);
    void split(RFTreeNode *node, int depth, int numDim);
    void BuildTree(int numDim);
    float traverse(RFTreeNode *r,const vector<float> &d);
    void set_predict_num(RFTreeNode *node);
    float get_predict_num(const vector<float> &d);
    friend class RF;
protected:
    vector<vector<float> > bsdata,outofbag;
    RFTreeNode *root;
};



class RF{
public:
    RF():outofbag_correct(0),outofbag_num(0){;}
    void FindOptimalForest(vector<vector<float> > data);
    void BuildForest(vector<vector<float> > data,int numDim);
    void predict();
    
    void predictOutOfBag();
    int getpredictnum(const vector<float> &d);
    float getForestAccuracy();
protected:
    vector<RFT> forest;
    float outofbag_correct;
    float outofbag_num;
    float ForestAccuracy;
};


void FindOptimalForest(vector<vector<float> > data);





#endif /* RandomForest_hpp */
