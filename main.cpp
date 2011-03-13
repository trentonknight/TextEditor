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
int countLines(NODES *mainNODE);
NODES *moveTo(NODES *mainNODE,int current,int where);
void displayCommands(NODES *mainNODE);
NODES *deleteLines(NODES *mainNODE,int from,int till);

int main(int argc, char *argv[])
{ 
  NODES *mainNODE;
  mainNODE = new NODES;
  ifstream grabFile;

  if(openFILE(grabFile,argv[1])){
    mainNODE = preLoad(grabFile,argv[1]);
  }
  displayCommands(mainNODE);
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
  first = new NODES;
  NODES *newNODE = 0;
  string lines;
  int noLine = 0;
  
  file.open(newfile.c_str(),ios::in);
  
  while(!file.eof()){
    noLine = 0;
    getline(file,lines);
    noLine = lines.size();
    if(noLine > 1){
    newNODE = new NODES;
    newNODE->line = lines;
    newNODE->front = 0;
    newNODE->back = 0;
  
    newNODE->back = first->back;
    first->back = newNODE;
    newNODE->front = first;
    if(newNODE->back != NULL){
      newNODE->back->front = newNODE;
    } 
  
    if(file.eof()){
      file.close();
    }
    }
  } 
  
  return first;
}
void displayCommands(NODES *mainNODE){
  char input[10];
  bool quit = true;
  int num = 0;
  int numTwo = 0; 
  int where = 0;

  while(quit){
    where = countLines(mainNODE);
    cout << mainNODE->line << endl;
    cout << "LINE[" << where <<"]";
    cout << " >> " << endl;
    cin >> input; 
    num = input[1] - 48;
    numTwo = input[2] - 48;

    if(input[0] == 'h' || input[0] == 'H'){
      cout << "h: display commands." << endl;
      cout << "t: display total lines in linked list." << endl;
      cout << "m #: move to requested line number." << endl;
      cout << "d #-#: delete line numbers (EXAMPLE: d 2-4)" << endl;
      cout << "a #: insert a new line of text after line number." << endl;
      cout << "b #: insert a new line of text before line number." << endl;
      cout << "q: quit and save lines." << endl;
    }
    if(input[0] == 'm' || input[0] == 'M'){
      mainNODE = moveTo(mainNODE,num,where);
    }
    if(input[0] == 'd' || input[0] == 'D'){
      mainNODE = deleteLines(mainNODE,num,numTwo);
    }
    if(input[0] == 'q' || input[0] == 'Q'){
      quit = false;
    }
  }

}
int countLines(NODES *mainNODE){
  int count = 0;
  while(mainNODE != NULL){
    count++;
    mainNODE = mainNODE->back;
  }
  return count;
}
NODES *moveTo(NODES *mainNODE,int current,int where){
  if(where > current){
    current = where - current;
    while(current != 0){
      mainNODE = mainNODE->back;
      current--;
    }
  }
  else{
    current = current - where;
    while(current != 0){
      mainNODE = mainNODE->front;
      current--;
    }
  }
  cout << mainNODE->line << endl;
  return mainNODE;
}
NODES *deleteLines(NODES *mainNODE,int from,int till){
  

  return mainNODE;
}
