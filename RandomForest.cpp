//
//  RandomForest.cpp
//  test
//
//  Created by 吳孟庭 on 2020/6/11.
//  Copyright © 2020 吳孟庭. All rights reserved.
//

#include "RandomForest.hpp"

RFTreeNode::RFTreeNode(vector<vector<float> > d){
    TreeNode();
    parent=NULL;
    leftchild=NULL;
    rightchild=NULL;
    nkey=NULL;
    pred_num=2;
    gini=1;
    for (int i=0;i<d.size();i++)
        data.push_back(d[i]);
}

RFT::RFT(vector<vector<float> > data){
    RFTreeNode *newnode = new RFTreeNode(data);
    root = newnode;
}

void RFT::creatbootstrap(vector<vector<float> > data){
	srand(time(NULL));
    unsigned long size = data.size();
    int *index = new int [size];
    memset(index, 0, size);
    for (int i=0;i<size;i++){
        
        int r = rand()%size;
        index[r] = 1;
        bsdata.push_back(data[r]);
    }
    for (int i=0;i<size;i++){
        if (index[i]==0)
            outofbag.push_back(data[i]);
    }
    
}

void FindOptimalForest(vector<vector<float> > data){
    vector<RF> F(MAXDIM-MINDIM+1);
    for (int i=0;i<MAXDIM-MINDIM+1;i++){
        RF *rf = new RF();
        F[i] = *rf;
    }
    cout<<"F.size(): "<<F.size()<<endl;
    float max=0;
    int index=0;
    cout<<"mindim: "<<MINDIM<<" maxdim: "<<MAXDIM<<endl;
    for (int i=MINDIM;i<=MAXDIM;i++){
        cout<<"build forest "<<i<<endl;
        F[i-MINDIM].BuildForest(data, i);
        F[i-MINDIM].predictOutOfBag();
        cout<<"get forest accuracy"<<endl;
        float cur = F[i-MINDIM].getForestAccuracy();
        cout<<"cur: "<<cur<<endl;
        if (cur>max){
            max=cur;
            index=i-MINDIM;
        }
    } 
    F[index].predict(); 
}

float RF::getForestAccuracy(){
    return ForestAccuracy;
}


void RF::BuildForest(vector<vector<float> > data,int numDim){
    for (int i=0;i<FORESTSIZE;i++){
        RFT *tree = new RFT(data);
        tree->creatbootstrap(data);
        tree->BuildTree(numDim);
        forest.push_back(*tree);
    }
    
}

void RFT::BuildTree(int numDim){
    root = new RFTreeNode(bsdata);
    root->nkey = test_split(root,numDim);
    split(root,1,numDim);
}

void RF::predictOutOfBag(){
    outofbag_correct=0;
    outofbag_num=0;
    for (int i=0;i<FORESTSIZE;i++){
        float correct=0,incorrect=0;
        for (int j=0;j<forest[i].outofbag.size();j++){
            float ans = forest[i].outofbag[j][forest[i].outofbag[0].size()-1];
            if ( ans == forest[i].get_predict_num(forest[i].outofbag[j]) )
                correct++;
            else
                incorrect++;
        }
        outofbag_correct += correct;
        outofbag_num += (correct+incorrect);
    }
    ForestAccuracy = outofbag_correct/outofbag_num;
}



void RF::predict(){
    vector<vector<float> > test;
    char filename[50] = "NSL-KDDTest+.txt";
    test = getdata(filename);
    string output = "output.txt";
    ofstream fout;
    fout.open(output);
    cout<<"getpredictnum"<<endl;
    for (int i=0;i<test.size();i++){
        cout<<getpredictnum(test[i])<<" ";
        fout<<getpredictnum(test[i]);
    }
    fout.close();
}


int RF::getpredictnum(const vector<float> &d){
    int r0=0,r1=0;
    for (int i=0;i<FORESTSIZE;i++){
        if (forest[i].get_predict_num(d)==0){
            r0++;
        }
        else{
            r1++;
        }
    }
    return r0>r1?0:1;
}


int *RFT::test_split(RFTreeNode *node, int numDim){
    int *index = new int[2];
    float min_gini=1;
    int *dim = new int [numDim];
    
    for(int i=0;i<numDim;i++){
        dim[i] = rand()%(41);
        for(int j=0;j<i;j++){
             if(dim[i]==dim[j]){
                 i--;
                 break;
             }
        }
    }

    for (unsigned int i=0;i<numDim;i++){
        //sort vector(sort_data) based on column i
        vector<vector<float> > sort_data;
        for (int j=0;j<node->data.size();j++)
            sort_data.push_back(node->data[j]);
        std::sort(sort_data.begin(),
        sort_data.end(),
        [=] (const std::vector<float> &a, const std::vector<float> &b)
        {
            return a[dim[i]] < b[dim[i]];
        });
        float cur_value = node->data[0][dim[i]];
 
        float col_min_gini = calGini(node,dim[i],0,sort_data);
 
        float cur = col_min_gini;
        unsigned int minj=0;
        for (unsigned int j=1;j<sort_data.size()-1;j++){
            for (unsigned int k=j+1;k<sort_data.size()-1;k++){
                if (node->data[k][dim[i]]==cur_value){
                    j=k;
                }
                else{
                    break;
                }
            }
            cur = calGini(node,dim[i],j,sort_data);

            if(cur<col_min_gini){
                col_min_gini = cur;
                minj=j;
            }
            cur_value = node->data[j][dim[i]];
        }
        
        if (col_min_gini<min_gini){
            node->data = sort_data;
            min_gini = col_min_gini;
            index[0] = dim[i];
            index[1] = minj;
        }
    }
    node->gini = min_gini;
    return index;
}

void RFT::split(RFTreeNode *node, int depth, int numDim){
    set_predict_num(node);
    vector<vector<float> > ldata,rdata;
    
    //split the vector and copy to ldata and rdata
    for (int i=0;i<=node->nkey[1];i++){
        ldata.push_back(node->data[i]);
    }
    for (unsigned int i=node->nkey[1]+1;i<node->data.size();i++){
        rdata.push_back(node->data[i]);
    }
    
    
    RFTreeNode *lnode = new RFTreeNode(ldata);
    RFTreeNode *rnode = new RFTreeNode(rdata);
    
    node->leftchild=lnode;
    node->rightchild=rnode;
    
    
    if (ldata.size()<MINSIZE){
        if (ldata.size()==0){
            node->leftchild=NULL;
            return;
        }
                else{
                    node->leftchild=lnode;
                    lnode->parent=node;
                    lnode->leftchild=NULL;
                    lnode->rightchild=NULL;
                    lnode->nkey=NULL;
                    set_predict_num(lnode);
                    return;
                }
        
    }
    if (rdata.size()<MINSIZE){
        if (rdata.size()==0){
            node->rightchild=NULL;
            return;
        }
                else{
                    node->rightchild=rnode;
                    rnode->parent=node;
                    rnode->leftchild=NULL;
                    rnode->rightchild=NULL;
                    rnode->nkey=NULL;
                    set_predict_num(rnode);
                    return;
                }
    }
    lnode->nkey = test_split(lnode,numDim);
    lnode->parent = node;
    if (lnode->gini<MINGINI){
        node->leftchild=lnode;
        lnode->parent = node;
        set_predict_num(lnode);
        lnode->nkey=NULL;
        lnode->leftchild = NULL;
        lnode->rightchild = NULL;
        return;
    }
          
    else{
        lnode->parent = node;
        lnode->leftchild = NULL;
        lnode->rightchild = NULL;
        node->leftchild = lnode;
        split(lnode,depth+1,numDim);
    }
       
    
    rnode->nkey = test_split(rnode,numDim);
    rnode->parent = node;
    
    if (rnode->gini<MINGINI){
        node->rightchild=rnode;
        rnode->parent = node;
        set_predict_num(rnode);
        rnode->nkey=NULL;
        rnode->leftchild = NULL;
        rnode->rightchild = NULL;
        return;
    }
        
    else{
        rnode->parent = node;
        rnode->leftchild = NULL;
        rnode->rightchild = NULL;
        node->rightchild = rnode;
        split(rnode,depth+1,numDim);
    }
     
}


float RFT::traverse(RFTreeNode *r,const vector<float> &d){
    if (r->nkey==NULL){
        return r->pred_num;
    }
    if (d[r->nkey[0]]<=r->data[r->nkey[1]][r->nkey[0]]){
        if (r->leftchild)
            return traverse(r->leftchild, d);
        else
            return r->pred_num;
    }
    else {
        if (r->rightchild)
            return traverse(r->rightchild, d);
        else
            return r->pred_num;
    }
}

void RFT::set_predict_num(RFTreeNode *node){
    int num_0 = 0,num_1 = 0;
    for (int i=0;i<node->data.size();i++){
        if (node->data[i][node->data[0].size()-1]==0)
            num_0++;
        else
            num_1++;
    }
    node->pred_num = num_0>num_1?0.0:1.0;
}

float RFT::get_predict_num(const vector<float> &d){
    return traverse(root,d);
}

float RFT::calGini(RFTreeNode *node,int index1,int index2,vector<vector<float> > &sdata){
    float gini;
    float instances = sdata.size();

    float leftins = index2+1,rightins = instances-leftins;
    if (rightins==0)
        return 1;
    float left0 = 0;
    for (int i=0;i<=index2;i++){
        if (sdata[i][sdata[0].size()-1]==0)
            left0++;
    }
    float left1 = leftins-left0;
    
    float right0 = 0;
    //node->data.size()-1 ,don't have to check the last one
    for (unsigned int i=index2+1;i<sdata.size()-1;i++){
        if (sdata[i][sdata[0].size()-1]==0)
            right0++;
    }
    float right1 = rightins-right0;
    
    
    gini = (leftins/instances)*( 1-(left0/leftins)*(left0/leftins)-(left1/leftins)*(left1/leftins) ) + (rightins/instances)*( 1-(right0/rightins)*(right0/rightins)-(right1/rightins)*(right1/rightins) );
    
    return gini;
}

