//
//  main.cpp
//  test
//
//  Created by WU,MENG-TING on 2020/6/11.
//  Copyright © 2020 WU,MENG-TING. All rights reserved.
//

#include <iostream>
#include <vector>
#include "getdata.hpp"
#include "RandomForest.hpp"
#define SIZE 1000
using namespace std;


int main() {
    vector<vector<float> > data;
    char filename[50] = "NSL-KDDTrain+.txt";
    data = getdata(filename);
    
    cout<<"Menu:"<<endl;
    cout<<"1.create forest and predict"<<endl;
    int x;
    cin>>x;
    vector<vector<float> > train;

    
    for (int i=0;i<SIZE;i++){
        train.push_back(data[i]);
    }
    
    
    switch (x) {
        case 1:
            FindOptimalForest(train);
            break;
        default:
            break;
    }
    return 0;
}
