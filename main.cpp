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


int main(int argc, char *argv[])
{
  ifstream grabFile;
  //string doc = argv[1];
  //cout << doc << endl;
  openFILE(grabFile,argv[1]);
}
bool openFILE(ifstream& file,string filename){
  bool check = false;
  file.open(filename.c_str(), ios::binary);
  if(file){
    check = true;
  }
  return check;
}

