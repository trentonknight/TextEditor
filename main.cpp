////////////////////////////////////////////////////////////////////////
///CODE FILENAME: Mansfield-ASSN1-DLLProg
///DESCRIPTION:   A Simplisitic Line Editor (Like VI just nowhere near as good.)
///               writes and saves documents via CLI or user choice.
///  DATE:    	19MAR11
///  DESIGNER:	Jason N Mansfield
///  FUNCTIONS: 
///            openFILE(), writeToFile(),preLoad(),noFile(),locatePosition()
///            countTotal(), moveTo(), driveCommands(),display(), deleteLines()
///            appendToFront(), appendToRear().
////////////////////////////////////////////////////////////////////////
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
void writeToFile(string filename,NODES *mainNODE);
NODES *preLoad(ifstream& file,string newfile);
NODES *noFile(NODES *mainNODE);
int locatePosition(NODES *mainNODE);
int countTotal(NODES *mainNODE);
NODES *moveTo(NODES *mainNODE,int current);
void driveCommands(NODES *mainNODE);
void display();
NODES *deleteLines(NODES *mainNODE,int from);
NODES *appendToFront(NODES *mainNODE);
NODES *appendToRear(NODES *mainNODE);
///////////////////////////////////////////////////////////////////////////
///  FUNCTION:	main
///  DESCRIPTION: takes CLI input from user and cycles through 
///               functions.
///  INPUT:
///  	Parameters: argv[1] recieves any user input for filename
///  	File:  	if verified true user chooses which file to load
///  OUTPUT:   
///  	Return Val: Linked List with multiple lines of character strings
///  	Parameters: filename or argv[1]
///  	File:	strings of characters
///  CALLS TO:  openFILE, preLoad, noFile, driveCommands, writeToFile
//////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{ 
  NODES *mainNODE;
  mainNODE = new NODES;
  ifstream grabFile;
  bool goodFile = false;
  string filename;

  if(argv[1] != 0x0){
    goodFile = openFILE(grabFile,argv[1]);
    if(goodFile){
      mainNODE = preLoad(grabFile,argv[1]);
      filename = argv[1];
    }
  }
  else{
    mainNODE = noFile(mainNODE);
  }
  driveCommands(mainNODE);
  if(!goodFile){
    cout << "Enter filename you wish to save as: " << endl;
    cin >> filename;
  }
  writeToFile(filename,mainNODE);
}
///////////////////////////////////////////////////////////////////////////
///  FUNCTION:  openFILE
///  DESCRIPTION:  verifes file exists
///  INPUT:
///  	Parameters: ifstream and string name of file from argv[1]
///  	File:  	checks to see if correct CLI filename was given.
///  OUTPUT:   
///  	Return Val: bool value
///  	Parameters: if file is found true then bool true is returned.
///  	File:	user CLI input name
//////////////////////////////////////////////////////////////////////////
bool openFILE(ifstream& file,string filename){
  bool check = false;
  file.open(filename.c_str(), ios::binary);
  if(file){
    check = true;
  }
  file.close();
  return check;
}
///////////////////////////////////////////////////////////////////////////
///  FUNCTION:	writeToFile
///  DESCRIPTION: traverses list and writes to file
///  INPUT:
///  	Parameters: filename is the file which to write too.
///                 mainNODE is the list which is written to file
///  	File:  	strings of characters
///  OUTPUT:   
///  	Return Val: ofstream to file
///  	Parameters: traversed list is written to user selected file
///  	File:	strings of characters per line
///  CALLS TO:  moveTo
//////////////////////////////////////////////////////////////////////////
void writeToFile(string filename,NODES *mainNODE){
  ofstream outfile;
  mainNODE = moveTo(mainNODE,1);
  outfile.open(filename.c_str(), ios::binary);
  if(outfile){
    while(mainNODE->front != 0){
      outfile << mainNODE->line << endl;
      mainNODE = mainNODE->front;
    }
  }
  outfile.close();
}
///////////////////////////////////////////////////////////////////////////
///  FUNCTION:	noFile
///  DESCRIPTION:  in the event user does not enter a filname via CLI when
///                starting program noFile creates enough nodes for new strings to be
///                appended to list.
///  INPUT:
///  	Parameters: empty mainNODE
///  OUTPUT:   
///  	Return Val: mainNODE with one new NODE
///  	Parameters: mainNODE->line = "\0"
//////////////////////////////////////////////////////////////////////////
NODES *noFile(NODES *mainNODE){
  NODES *newNODE = 0;
  newNODE = new NODES;
  newNODE->line = "\0";
  newNODE->front = 0;
  newNODE->back = 0;

  newNODE->back = mainNODE->back;
  mainNODE->back = newNODE;
  newNODE->front = mainNODE;

  return mainNODE;  
}
///////////////////////////////////////////////////////////////////////////
///  FUNCTION:	preLoad
///  DESCRIPTION:  reads and parses ifstream data to doubly linked list
///  INPUT:
///  	Parameters: ifstream file is user approved newfile which to stream data.
///                 from. 
///  	File:  	any characters from text, no validation or filtering is done.
///  OUTPUT:   
///  	Return Val: doubly linked list is returned full from file data.
///  	Parameters: NODES *first
//////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////
///  FUNCTION:	driveCommands
///  DESCRIPTION:  core driver program that makes all calls while editing
///                doubly linked list.
///  INPUT:
///  	Parameters: mainNODE primary doubly linked list
///  OUTPUT:   
///  	Return Val: mainNODE is complete after user has finished all editing.
///  CALLS TO:  moveTo, locatePosition, display, deleteLines, appendToFront
///             appendToRear, countTotal.
//////////////////////////////////////////////////////////////////////////
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
      mainNODE = appendToFront(mainNODE);
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
      mainNODE = appendToRear(mainNODE);
    }
    if(input == "q" || input == "Q"){
      quit = false;
    }
    if(input == "t" || input == "T"){
      total = countTotal(mainNODE);
      cout << "TOTAL: " << total - 1 << endl; 
    }
  }
  
}
///////////////////////////////////////////////////////////////////////////
///  FUNCTION: locatePosition
///  DESCRIPTION:  finds end of current list
///  INPUT:
///  	Parameters: mainNODE
///  OUTPUT:   
///  	Return Val: count of traversals till NULL is found
///  	Parameters: returns int count of traversals
//////////////////////////////////////////////////////////////////////////
int locatePosition(NODES *mainNODE){
  int count = 0;
  while(mainNODE != NULL){
    count++;
    mainNODE = mainNODE->back;
  }
  return count;
}
///////////////////////////////////////////////////////////////////////////
///  FUNCTION:	countTotal
///  DESCRIPTION: counts both forwards and backwards in list to get
///               a total count of items in list.
///  INPUT:
///  	Parameters: mainNODE
///  OUTPUT:   
///  	Return Val: total count of list size.
///  	Parameters: int value of traversal count.
//////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////
///  FUNCTION: moveTo
///  DESCRIPTION:  using both function countTotal for size and locatePosition
///                moveTo is able to identify available locations and move to
///                or warn user that location does not exist.
///  INPUT:
///  	Parameters: current is the interger value which represents where the user wants 
///                 to access.
///  OUTPUT:   
///  	Return Val: if available mainNODE is returned with pointer moved to requested 
///                 location.
///  CALLS TO: countTotal,locatePosition,driveCommands
//////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////
///  FUNCTION:	deleteLines
///  DESCRIPTION: deletes one or many NODES as requested by user
///  INPUT:
///  	Parameters: mainNODE list
///                 from is the node location being deleted.
///  OUTPUT:   
///  	Return Val: mainNODE after select NODES are removed
///  CALLS TO: moveTo, locatePosition, 
//////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////
///  FUNCTION:	display
///  DESCRIPTION:  simple message for user.
//////////////////////////////////////////////////////////////////////////
void display(){
  cout << "h: display commands." << endl;
  cout << "t: display total lines in linked list." << endl;
  cout << "m #: move to requested line number." << endl;
  cout << "d #-#: delete line numbers (EXAMPLE: d 2-4)" << endl;
  cout << "a #: insert a new line of text after line number." << endl;
  cout << "b #: insert a new line of text before line number." << endl;
  cout << "q: quit and save lines." << endl;
}
///////////////////////////////////////////////////////////////////////////
///  FUNCTION:  appendToFront
///  DESCRIPTION:  appends new NODE with user input in front of select
///                NODE local.
///  INPUT:
///  	Parameters: mainNODE is the current list
///  OUTPUT:   
///  	Return Val: mainNODE is returned with appened NODE
///  CALLS TO:  List of programmer-written functions called (names only)
//////////////////////////////////////////////////////////////////////////
NODES *appendToFront(NODES *mainNODE){
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
///////////////////////////////////////////////////////////////////////////
///  FUNCTION:  appendToRear
///  DESCRIPTION: appends in front of NODE, more realitically it moves 
///               NODE forwards once and places new NODE in its previous local.
///  INPUT:
///  	Parameters: mainNODE is the current list
///  OUTPUT:   
///  	Return Val: appended mainNODE
///  CALLS TO:  List of programmer-written functions called (names only)
//////////////////////////////////////////////////////////////////////////
NODES *appendToRear(NODES *mainNODE){
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
