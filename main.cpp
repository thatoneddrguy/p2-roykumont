#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

struct info1{
  int number;
  int start;
  int end;
  int block;
  std::vector<int> size1;
}info;


struct def_block{
 int start;
 int end;
 int program;
 int page;
 bool freeflg;
}block;
//creating the order of time
vector<int> execution_list(vector<info1> &prlist){
vector<int> exe_list;
bool temp_1,temp_2;

for(int i=0;i<prlist.size();i++){
  temp_1=temp_2=false;
  for(int k=0;k<exe_list.size();k++){

    if(prlist[i].start==exe_list[k]){
      temp_1=true;
    }
    if(prlist[i].start+prlist[i].end==exe_list[k]){
      temp_2=true;
    }
  }

  if(temp_1==false){
    exe_list.push_back(prlist[i].start);
  }
  if(temp_2==false){
    exe_list.push_back(prlist[i].start+prlist[i].end);
  }

}
std::sort (exe_list.begin(), exe_list.end());
return exe_list;
};
//Creating a memory block structure

vector<def_block> block_div(int mem_s, int page_s){
vector<def_block> mmb;

for(int k=0;k<(mem_s/page_s);k++){

  block.start=(k*page_s);
  block.end=((k+1)*page_s)-1;
  block.program=-1;
  block.page=-1;
  block.freeflg=true;
  mmb.push_back(block);
}
return mmb;

};





int main(){
int memorysize=2000;
int pagesize=0;
int memory=0,totalprocess=0;
vector<info1> prlist;
vector<int> exe_li;
vector<def_block> blk;
cout<<"Please enter page size where(1=100,2=200,3=300) > ";
cin>>pagesize;
if (pagesize==1){
  pagesize=100;
}else if(pagesize==2){
  pagesize=200;
}else if(pagesize==3){
  pagesize=300;
}else{
  cout<<"Invalid Value. Please enter correct value. "<<endl;
}
//Reading the input file and store into struct type vector
ifstream inputfile;
inputfile.open("in1.txt");
inputfile>>totalprocess;

while(totalprocess>0){

  inputfile>>info.number;
  inputfile>>info.start;
  inputfile>>info.end;
  inputfile>>info.block;
  int b1=info.block;
  while(b1>0){
    inputfile>>memory;
    info.size1.push_back(memory);
    b1--;
  }
  totalprocess--;
  prlist.push_back(info);
}
inputfile.close();
exe_li=execution_list(prlist);

cout<<"Time: "<<endl;
for(int k=0;k<exe_li.size();k++){
cout<<exe_li[k]<<endl;
}

cout<<"\n"<<"Memory Block: "<<endl;
blk=block_div(memorysize,pagesize);
for(int k=0;k<blk.size();k++){
cout<<blk[k].start<<" "<<blk[k].end<<endl;
}
return 0;
}
