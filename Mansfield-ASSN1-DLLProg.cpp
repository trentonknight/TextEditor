////////////////////////////////////////////////////////////////////////
///CODE FILENAME: Mansfield-ASSN1-DLLProg
///DESCRIPTION:   A Simplisitic Line Editor (Like VI just nowhere near as good.)   //*** km: vi -- my favorite.  No wasted time w/
///               writes and saves documents via CLI or user choice.               //        a silly mouse.
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

struct NODES{      //*** km: Traditional to reserve all caps words for constants.
  string line;
  NODES* back;
  NODES* front;
};

bool openFILE(ifstream& file,string filename);
void writeToFile(string filename,NODES *mainNODE);
NODES *preLoad(ifstream& file,string newfile);
NODES *newLine(NODES *mainNODE);
int locatePosition(NODES *mainNODE);
int countTotal(NODES *mainNODE);
NODES *moveTo(NODES *mainNODE,int current);
void driveCommands(NODES *mainNODE);
void display();
NODES *deleteLines(NODES *mainNODE,int from);
NODES *appendToFront(NODES *mainNODE,bool here);
NODES *appendToRear(NODES *mainNODE,bool here);

//*** km: Leave some blank lines for readability.

///////////////////////////////////////////////////////////////////////////
///  FUNCTION:	main
///  DESCRIPTION: takes CLI input from user and cycles through 
///               functions.
///  INPUT:
///  	Parameters: argv[1] recieves any user input for filename
///  	File:  	if verified true user chooses which file to load
///  OUTPUT:   
///  	Return Val: Linked List with multiple lines of character strings    //*** km: Return Val is 0, meaning program completed
///  	Parameters: filename or argv[1]                                     //        successfully.
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
         //*** km: How about a comment here on what you're doing w/ this if..else?
         
  // //*** km:
  // cout << "Before initial if\n";
  
  mainNODE = newLine(mainNODE);
  
  if(argv[1] != 0x0){
             
    // //*** km:
    // cout << "In initial if\n";
    // cin.get();
    
    goodFile = openFILE(grabFile,argv[1]);
    
    // //*** km:
    // cout << "After goodFile: " << goodFile << "\n";
    // cin.get();
    
    if(goodFile){
      mainNODE = preLoad(grabFile,argv[1]);
      
      // //*** km:
      // cout << "After preLoad.  mainNode = " << mainNODE << endl;
      // cin.get();
      
      filename = argv[1];
    }
  }
 
  
  // //*** km: 
  // cout << "Before driveCommands()\n";
  // cin.get();
  
  driveCommands(mainNODE);
  if(!goodFile){
    cout << "Enter filename you wish to save as: " << endl;
    cin >> filename;
  }
  writeToFile(filename,mainNODE);
  cin.get();//PAUSE
  return 0;
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
///  f(n) = n
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

//*** km: Above -- Looks as if it works but file open/closes are relatively expensive
//        operations especially if they're over the net.  Much better to do your
//        verify and then pass the file handle on to the working section of code.

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
///  f(n) = n
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
///  FUNCTION:	newLine
///  DESCRIPTION:  in the event user does not enter a filname via CLI when     //*** km: Write out cmd line interface rather than CLI
///                starting program noFile creates enough nodes for new strings to be
///                appended to list. This also automates any requests by user
///                which moves pointer past available line numbers.
///  INPUT:
///  	Parameters: empty mainNODE       //*** km: int current is an input?
///  OUTPUT:   
///  	Return Val: mainNODE with one new NODE
///  	Parameters: mainNODE->line = "\0"
///  CALLS TO: locatePosition 
///  f(n) = n   
//////////////////////////////////////////////////////////////////////////
NODES *newLine(NODES *mainNODE){
  NODES *newNODE = 0;
  newNODE = new NODES;

  newNODE->line = "\0";
  newNODE->front = 0;
  newNODE->back = 0;
 
  newNODE->back = 0;
  newNODE->front = mainNODE;
  mainNODE = newNODE;
  mainNODE->front->front = 0;
  mainNODE->front->back = 0;

  return mainNODE;  
}
///////////////////////////////////////////////////////////////////////////
///  FUNCTION:	preLoad
///  DESCRIPTION:  reads and parses ifstream data to doubly linked list
///  INPUT:
///  	Parameters: ifstream file is user approved newfile which to stream data.   //*** km: newfile?
///                 from. 
///  	File:  	any characters from text, no validation or filtering is done.
///  OUTPUT:   
///  	Return Val: doubly linked list is returned full from file data.
///  	Parameters: NODES *first
///  f(n) = n
//////////////////////////////////////////////////////////////////////////
NODES *preLoad(ifstream& file,string newfile){
  NODES *first = 0;
  first = new NODES;
  NODES *newNODE = 0;
  string lines;
  int noLine = 0;
  
  file.open(newfile.c_str(),ios::in);
  first->back = 0;
  first->front = 0;
  // //*** km:
  // cout << "In preLoad after file.open()\n";
  // cin.get();
  
  
  while(!file.eof()){
    noLine = 0;
    getline(file,lines);
    noLine = lines.size();
    
    //*** km: You definitely need some comments here.  I have no idea what your noLine > 1 is testing for.
    if(noLine > 1){
      newNODE = new NODES;
      newNODE->line = lines;
      newNODE->front = 0;      //*** km: Be consistent.  Either use NULL or 0 for null ptr but not both!
      newNODE->back = 0;
  
      newNODE->back = first->back;    //*** km: Need some comments here too.
      first->back = newNODE;
      newNODE->front = first;
      if(newNODE->back != NULL){
	newNODE->back->front = newNODE;
      }     
      if(file.eof()){      //*** km: Here and above -- proper indenting.
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
///  f(n) = n(log2n)
//////////////////////////////////////////////////////////////////////////
void driveCommands(NODES *mainNODE){
  string input;
  bool quit = true;
  bool move = true;
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
	num = locatePosition(mainNODE);
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
        if(mainNODE->back == 0 && mainNODE->front->front == 0){
          cout << "ERROR: no line here yet." << endl;
          driveCommands(mainNODE);
	}
        else{
	mainNODE = deleteLines(mainNODE,num);
	}
	num++;
      }
    }
    if(input == "a" || input == "A"){
      num = cin.get();
      if(num == '\n'){
	num = locatePosition(mainNODE);
        move = false;
      } 
      else{
	cin >> num;
      }
      numTwo = locatePosition(mainNODE);
      mainNODE = moveTo(mainNODE,num);
      cout << "ENTER: to edit"<< endl;
      cin.get();//pause before user input
      mainNODE = appendToFront(mainNODE,move);
      mainNODE = moveTo(mainNODE,numTwo);
    }
    if(input == "b" || input == "B"){
      num = cin.get();
      if(num == '\n'){
	num = locatePosition(mainNODE);
        move = false;
      } 
      else{
	cin >> num;
      }
      numTwo = locatePosition(mainNODE);
      mainNODE = moveTo(mainNODE,num -1);
      cout << "ENTER: to edit" << endl;
      cin.get();//pause before user input
      mainNODE = appendToRear(mainNODE,move);
      mainNODE = moveTo(mainNODE,numTwo);
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

//*** km: Above -- Jason, I don't know you since this is the first class you're taking with me.  But your
//        code is ATTROCIOUS!  It is unreadable.  You must put some comments, especially to explain branches.
//        You must put in some blank lines and be more careful w/ the indenting.  I don't have a clue as to
//        what your code is trying to do.  For this first lab 1, I
//        won't take points off for this, but next time -- watch out!

///////////////////////////////////////////////////////////////////////////
///  FUNCTION: locatePosition
///  DESCRIPTION:  finds end of current list
///  INPUT:
///  	Parameters: mainNODE     //*** km: Need a brief comment on what it is.
///  OUTPUT:   
///  	Return Val: count of traversals till NULL is found
///  	Parameters: returns int count of traversals           //*** km: count is not returned as a parameter, but as a return value.
///  f(n) = n
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
///  f(n) = n
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
///  f(n) = n
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
    cout << "ERROR: line does not exist." << endl;
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
///  f(n) = n
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
///  constant
//////////////////////////////////////////////////////////////////////////
NODES *appendToFront(NODES *mainNODE,bool here){
  NODES *newNode;
  newNode = new NODES;
  char newFront[20];
  
  cout << "$ " << endl;
  cin.getline(newFront,20);

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
///  constant
//////////////////////////////////////////////////////////////////////////
NODES *appendToRear(NODES *mainNODE,bool here){
  NODES *newNode;
  newNode = new NODES;

  char newFront[20];
  
  cout << "$ " << endl;
  cin.getline(newFront,20);

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
