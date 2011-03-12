#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;


struct NODES{
  string line;
  NODES* back;
  NODES* front;
};
bool openFILE(ifstream& file,string filename);
NODES *preLoad(ifstream& file,string newfile);

int main(int argc, char *argv[])
{ 
  NODES *mainNODE;
  mainNODE = new NODES;
  ifstream grabFile;

  if(openFILE(grabFile,argv[1])){
    mainNODE = preLoad(grabFile,argv[1]);
  }
  cout << "finished" << endl;
}
bool openFILE(ifstream& file,string filename){
  bool check = false;
  file.open(filename.c_str(), ios::binary);
  if(file){
    check = true;
  }
  file.close();
  return check;
}
NODES *preLoad(ifstream& file,string newfile){
  NODES *first = 0;
  NODES *last = 0;
  NODES *newNODE = 0;
  string lines;
  
  file.open(newfile.c_str(),ios::in);
  
  while(!file.eof()){

    getline(file,lines);
    newNODE = new NODES;
    newNODE->line = lines;
    newNODE->front = 0;
    newNODE->back = 0;

    if(first == NULL){
      first = newNODE;
      last = newNODE;
    }
    else{  
      while(last != 0){
        first = last;
	last = last->back;
      }
      first->back = newNODE;     
      last = newNODE;
      last->front = first;
    }  
    if(file.eof()){
      file.close();
    }
  } 
  return last;
}
