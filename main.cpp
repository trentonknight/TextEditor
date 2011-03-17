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
int locatePosition(NODES *mainNODE);
int countTotal(NODES *mainNODE);
NODES *moveTo(NODES *mainNODE,int current);
void driveCommands(NODES *mainNODE);
void display();
NODES *deleteLines(NODES *mainNODE,int from);
NODES *appendToFront(NODES *mainNODE,int num);
NODES *appendToRear(NODES *mainNODE,int num);

int main(int argc, char *argv[])
{ 
  NODES *mainNODE;
  mainNODE = new NODES;
  ifstream grabFile;

  if(openFILE(grabFile,argv[1])){
    mainNODE = preLoad(grabFile,argv[1]);
  }
  driveCommands(mainNODE);
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
	first->front = 0;
	file.close();
      }
    }
  } 
  
  return first;
}
void driveCommands(NODES *mainNODE){
  string input;
  bool quit = true;
  int total = 0;
  int num = 0; 
  int numTwo = 0; 

  mainNODE = moveTo(mainNODE,1);

  while(quit){
    cout << mainNODE->line << endl;
    cout << "LINE[" << locatePosition(mainNODE) <<"]";
    cout << " >> " << endl;
    cin >> input;

    if(input == "h" || input == "H"){
      display();
    }
    if(input == "m" || input == "M"){
      num = cin.get();
      if(num == '\n'){
	num = locatePosition(mainNODE) + 1;
      } 
      else{
	cin >> num;
      }
      mainNODE = moveTo(mainNODE,num);
    }
    if(input == "d" || input == "D"){    
    
      num = cin.get();
      if(num == '\n'){
	num = locatePosition(mainNODE) + 1;
	numTwo = num + 1;
      } 
      else{
	cin >> num;
	numTwo = cin.get();
	if(numTwo != '\n'){
	  cin >> numTwo;
	}
	else{
	  numTwo = num + 1;
	}
      }
      while(num != numTwo){
	mainNODE = deleteLines(mainNODE,num);
	num++;
      }
    }
    if(input == "a" || input == "A"){
      num = cin.get();
      if(num == '\n'){
	num = locatePosition(mainNODE) + 1;
      } 
      else{
	cin >> num;
      }
      mainNODE = moveTo(mainNODE,num);
      mainNODE = appendToFront(mainNODE,num);
    }
    if(input == "b" || input == "B"){
      num = cin.get();
      if(num == '\n'){
	num = locatePosition(mainNODE) - 1;
      } 
      else{
	cin >> num;
      }
      mainNODE = moveTo(mainNODE,num);
      mainNODE = appendToRear(mainNODE,num);
    }
    if(input == "q" || input == "Q"){
      mainNODE = NULL;
      quit = false;
    }
    if(input == "t" || input == "T"){
      total = countTotal(mainNODE);
      cout << "TOTAL: " << total - 1 << endl; 
    }
  }
  
}
int locatePosition(NODES *mainNODE){
  int count = 0;
  while(mainNODE != NULL){
    count++;
    mainNODE = mainNODE->back;
  }
  return count;
}
int countTotal(NODES *mainNODE){
  int count = 0;
  while(mainNODE->back != NULL){
    mainNODE = mainNODE->back;
  }
  while(mainNODE != NULL){
    mainNODE = mainNODE->front;
    count++;
  }
  return count;
}
NODES *moveTo(NODES *mainNODE,int current){
  int total = countTotal(mainNODE);  
  int where = locatePosition(mainNODE);

  if(current > 0 && current < total)
    {
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
    }
  else{
    cout << "ERROR: invalid line: " << current << endl;
    driveCommands(mainNODE);
  }
  return mainNODE;
}
NODES *deleteLines(NODES *mainNODE,int from){
  int where = 0;
  where = locatePosition(mainNODE);
  mainNODE = moveTo(mainNODE,from);

  if(from == 1){
    mainNODE->front->back = mainNODE->back;
  }
  else if(mainNODE->front == NULL){
    mainNODE->back->front = mainNODE->front;
  }
  else{
    mainNODE->front->back = mainNODE->back;
    mainNODE->back->front = mainNODE->front;
  }
  delete mainNODE;
  where = locatePosition(mainNODE);
  if(from != 1){
    mainNODE = moveTo(mainNODE,from -1);
  }
  else{
    mainNODE = moveTo(mainNODE,from +1);
  }
  
  return mainNODE;
}
void display(){
  cout << "h: display commands." << endl;
  cout << "t: display total lines in linked list." << endl;
  cout << "m #: move to requested line number." << endl;
  cout << "d #-#: delete line numbers (EXAMPLE: d 2-4)" << endl;
  cout << "a #: insert a new line of text after line number." << endl;
  cout << "b #: insert a new line of text before line number." << endl;
  cout << "q: quit and save lines." << endl;
}
NODES *appendToFront(NODES *mainNODE,int num){
  NODES *newNode;
  newNode = new NODES;
  string newFront;

  cout << "$ " << endl;
  cin.ignore(25, '\n');
  getline(cin,newFront);
  
  newNode->line = newFront;
  newNode->back = 0;
  newNode->front = 0;

  newNode->back = mainNODE;
  newNode->front = mainNODE->front;
  mainNODE->front->back = newNode;
  mainNODE->front = newNode;
 
  return mainNODE;
}
NODES *appendToRear(NODES *mainNODE,int num){
  NODES *newNode;
  newNode = new NODES;
  string newFront;

  cout << "$ " << endl;
  cin.ignore(25, '\n');
  getline(cin,newFront);

  newNode->line = newFront;
  newNode->back = 0;
  newNode->front = 0;

  if(mainNODE->back == NULL){
    newNode->back = NULL;
    newNode->front = mainNODE;
    mainNODE->back = new NODES;
    mainNODE->back->front = newNode;
    mainNODE->back = newNode;
  } 
  else{ 
    newNode->front = mainNODE;
    newNode->back = mainNODE->back;
    mainNODE->back->front = newNode;
    mainNODE->back = newNode;
  }

  return mainNODE;
}
