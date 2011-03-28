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
void driveCommands(NODES *mainNODE,NODES *top,NODES *bottom);
void display();
NODES *deleteLines(NODES *mainNODE,int from,NODES *top,NODES *bottom);
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
  ///make node for keeping top node NULL
  NODES *top;
  top = new NODES;
  ///make node for keeping bottom node NULL
  NODES *bottom;
  bottom = new NODES;

  ifstream grabFile;
  ///used for verifing file exists
  bool goodFile = false;
  string filename;
 
  ///make one node and nullify top and bottom 
  mainNODE = newLine(mainNODE);
  top = mainNODE->front;
  bottom = mainNODE;
  
  if(argv[1] != 0x0){
  
    //check if file exists, I wont use this function next time
    //as advised.
    goodFile = openFILE(grabFile,argv[1]);
    
    //if file exists then preload the entire file into linked list
    if(goodFile){
      mainNODE = preLoad(grabFile,argv[1]);
      //make string filename equal CLI 
      filename = argv[1];
    }
  }
  ///driver function that runs all other functions except
  ///for the final write to file
  driveCommands(mainNODE,top,bottom);
  ///request new filename since CLI was not used of filename was not valid
  if(!goodFile){
    cout << "Enter filename you wish to save as: " << endl;
    cin >> filename;
  }
  //function which writes to file
  writeToFile(filename,mainNODE);
  system("PAUSE");//windows pause
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
  //if file exists return bool true
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
  ///simple function opens file and writes list
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
  //simple doubly linked list
  newNODE->line = "\0";
  newNODE->front = 0;
  newNODE->back = 0;
 
  newNODE->back = 0;
  newNODE->front = mainNODE;
  mainNODE = newNODE;
  //once again a struggle to keep nodes
  //null
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
  //keeping ends null
  first->back = 0;
  first->front = 0;
  //while loop loads each
  //line into a node
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
      if(newNODE->back != 0){
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
void driveCommands(NODES *mainNODE,NODES *top,NODES *bottom){
  string input;
  bool quit = true;
  bool move = true;
  int total = 0;
  int num = 0; 
  int numTwo = 0; 
  
  mainNODE = moveTo(mainNODE,1);
  
  while(quit){
    //this works with GCC 4.5.1
    //but not with GCC 3.4.2 (mingw-special)
    //it does work fine with the latest version of
    //mingw and its more modern compiler but not the supplied
    //gcc for Dev-C++
    //top and bottom should still be on correct address for top and
    //bottom of list and NULL therefore allowing all traversing loops to
    //do their job, namely locatePosition called in the deleteLines function
    top->front = 0;
    bottom->back = 0;
    //printout to user
    cout << mainNODE->line << endl;
    ///call locatePosition which simply traverses and counts
    cout << "LINE[" << locatePosition(mainNODE) <<"]";
    cout << " >> " << endl;
    //get user selection
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
          //safeguard to over user attempting to delete last node
          cout << "ERROR: no line here yet." << endl;
          driveCommands(mainNODE,top,bottom);
	}
        else{
	  mainNODE = deleteLines(mainNODE,num,top,bottom);
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
      //double check position
      numTwo = locatePosition(mainNODE);
      //move to user selection
      mainNODE = moveTo(mainNODE,num);
      cout << "ENTER: to edit"<< endl;
      cin.get();//pause before user input
      //call append function
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
      //same as append above except calls 
      //append to rear
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
      //just calls function countTotal
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
  ///this never reaches its end due to address being lost when compiled with
  //Dev-C++ version on GCC. Without proper addresses top and bottom nodes cannot be
  //made null. When delete is used this function goes into infinite loop.
  while(mainNODE != 0){
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
  //traverses up and down list to get total
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
  //moves until users chosen current is reached
  //if has safe guard to ensure user is not trying to reach something
  //unavailable
  if(current > 0 && current < total)
    {
      if(where > current){//get amount needed to traverse
	current = where - current;
	while(current != 0){
	  mainNODE = mainNODE->back;
	  current--;
	}
      }
      else{
	current = current - where;
	while(current != 0){//get amount needed to traverse
	  mainNODE = mainNODE->front;
	  current--;
	}
      }     
    }
  else{//check unavailable
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
NODES *deleteLines(NODES *mainNODE,int from,NODES *top,NODES *bottom){
  int where = 0;
  mainNODE = moveTo(mainNODE,from);
  NODES *pDEL;
  pDEL = mainNODE;

  if(pDEL == 0){
    //should never actual use this if
    //due to previous safeguard 
    cout << "ERROR: last line" << endl;
    driveCommands(mainNODE,top,bottom);
  }
  if(pDEL->back != 0){
    mainNODE->back = pDEL->back;
    mainNODE->back->front = pDEL->front;
  }
  else{
    mainNODE = pDEL->front;
  }
  if(pDEL->front != 0){
    mainNODE->front = pDEL->front;
    mainNODE->front->back = pDEL->back;
  }
  else{
  mainNODE->back = pDEL->back;
  }
  delete pDEL;
  top->front = 0;
  bottom->back = 0;
  ///this is where the delete function fails due to an endless amount of nodes
  ///top and bottom should in theory be null but this does not happen on GCC
  ///3.4.2 mingw-special for Dev-C++
  ///However it works wonderfully using the latest real version of GCC and should look like
  ///(gdb) print top->front
  ///$6 = (NODES *) 0x0
  ///(gdb) print bottom->back
  ///$7 = (NODES *) 0x0
  ///additionaly top and bottom should have retained their original addresses given in main
  ///this is not happening in the Dev-C++ flavor of GCC
  where = locatePosition(mainNODE);
  if(from != 1){
    //move to safe node
    mainNODE = moveTo(mainNODE,from -1);
  }
  else{
    //move to safe node
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
  ///get users new line and create new node
  newNode->line = newFront;
  newNode->back = 0;
  newNode->front = 0;
  ///put new node in front of previous
  newNode->front = mainNODE->front;
  newNode->back = mainNODE;
  if(mainNODE->front != 0){
    mainNODE->front->back = newNode;
  }
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
  ///get user input for new line
  newNode->line = newFront;
  newNode->back = 0;
  newNode->front = 0;
  //append behind chosen node
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
