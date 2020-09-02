#include "getdata.hpp"

float stringtofloat(string word){
	int i=0;
	float re=0.0,k=1.0;
	bool k_flag = 0;
	while(1){
		if(word[i]=='.')
			k_flag = 1;
		else if(word[i]=='\0')
			break;
		else{
			re = re*10.0 + int(word[i])-48;
			if(k_flag)
				k*=0.1;
		}
		i++;
	}
	//cout << "rk" << re*k << " ";
	return re*k;
}
vector<float> linetoarray(string line){
	//cout << "ltoa";
	vector<float> fdata;
	int i=0;
	string word="\0";
	while(1){
		if(line[i]==','){
			//cout << "w" << word << " ";
			fdata.push_back(stringtofloat(word));
			word = "\0";
		}
		else if(line[i]=='\0'){
			fdata.push_back(stringtofloat(word));
			break;
		}
		else
			word = word + line[i];		
		i++;
	}
	//cout << fdata[0] << fdata[1];
	return fdata;
}
vector<vector<float> > getdata(char* filename){
    
    
	ifstream inStream;
	inStream.open(filename);
	vector<vector<float> > data;
	vector<float> line;
	string gl;
	
	int type_flag = 0; //0 for number, 1 for letter
	while(getline(inStream,gl,'\n')){
		line = linetoarray(gl);
		data.push_back(line);
	}
    
	
	return data;
} 
