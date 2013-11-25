/* C++ program that simulated a Turing Machine.

The TM should be stored on a file called TM.txt with the following format:

	First line is the number of states.

	Second line is the start state.

	Third line is the accept state.

	Fourth line is the symbols in the tape alphabet (apart from blank)

	The rest of the lines are the transitions in the form: 
		F T RWM, where
		the transition goes from F to T, and 
		R = readSymbol,
		W = writeSymbol (where D stands for blank symbol), and 
		M = headMoveChar (L for left, R for right, S for stationary).

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

struct transition {
	string currentState;
	string futureState;
	char readSymbol;
	char writeSymbol;
	char headMoveChar;
};

class Turing {
  string tape;
  int head;
  int size;
  string state;
  string alphabet;
  string start;
  string accept;
  vector< vector<transition> > transitions;
  int inAlphabet(char);
  int validMove(char);
  int validTape();
  void readTransitions(ifstream &);
  void myLittleHelper(transition);
public:
  Turing(string);
  void setTape(string);
  void printTransitions();
  void processTape();
};

Turing::Turing(string f) {

	char *arr = new char[f.length()+1];
  strcpy (arr, f.c_str());

  ifstream is(arr);
  if (!is.good()) {
  	cerr << "Error opening the file! Exiting." << endl;
  	exit(-1);
  }
  string tmp;
  int t;
  
  getline(is, tmp); // read first line
  stringstream(tmp) >> t;
  transitions.resize(t-1); // t-1 since there are no transitions from the accept state
  getline(is, start); // read second line
  getline(is, accept); // read third line
  getline(is, alphabet); // read fourth line

  // now read all of the transitions
  readTransitions(is);
  is.close();

  head = 0;
  size = 0;
  state = start;
}

void Turing::setTape(string str) {
	tape = str;
	size = tape.length();

	if (!validTape())
		exit(-1);
}

int Turing::inAlphabet(char c) {

	if (c == 'D')
		return 1;

	for (int i = 0; i < alphabet.length(); i++)
		if (c == alphabet[i])
			return 1;

	return 0;
}

int Turing::validMove(char c) {

	if (c != 'L' && c != 'R' && c != 'S')
		return 0;

	return 1;
}

int Turing::validTape() {
  if (size == 0) {
    cout << "Rejected." << endl;
    return 0;
  }
  for (int i = 0; i < size; i++) {
    if (!inAlphabet(tape[i])) {
    	cout << tape[i] << " is not in the alphabet! Rejected." << endl;
    	return 0;
    }
  }

  return 1;
}

void Turing::readTransitions(ifstream &is) {

	char from[10];
  char to[10];
  char read;
  char write;
  char move;
  char buff; // buffer for skipping spaces
  while(is.good()) {
  	is.get(from, 10, ' '); // read from state until we reach a space
		buff = is.get(); // skip the space
  	is.get(to, 10, ' ');
  	buff = is.get();
  	read = is.get();
  	if (!inAlphabet(read)) {
  		cerr << "Error! " << read << " is not in the alphabet!" << endl;
  		exit(-1);
  	}
  	write = is.get();
  	if (!inAlphabet(write)) {
  		cerr << "Error! " << write << " is not in the alphabet!" << endl;
  		exit(-1);
  	}
  	move = is.get();
  	if (!validMove(move)) {
  		cerr << "Error! " << move << " is not a valid move!" << endl;
  		exit(-1);
  	}
  	buff = is.get(); // move to next line

  	transition t;
  	t.currentState = from;
  	t.futureState = to;
  	t.readSymbol = read;
  	t.writeSymbol = write;
  	t.headMoveChar = move;

  	int index;
  	for (int i = 0; i < transitions.size(); i++) {
  		if (transitions[i].empty()) {
  			index = i;
  			break;
  		}
  		else if (transitions[i][0].currentState.compare(t.currentState) == 0) {
  			index = i;
  			break;
  		}
  	}

  	transitions[index].push_back(t);
  }
}

void Turing::printTransitions() {

	for (int i = 0; i < transitions.size(); i++) {
		for (int j = 0; j < transitions[i].size(); j++) {
			cout << transitions[i][j].currentState << " " << transitions[i][j].futureState << " ";
			cout << transitions[i][j].readSymbol << transitions[i][j].writeSymbol;
			cout << transitions[i][j].headMoveChar << endl;
		}
	}
}

void Turing::myLittleHelper(transition t) {

	state = t.futureState;
	tape[head] = t.writeSymbol;
	if (t.headMoveChar == 'R')
		head++;
	else if (t.headMoveChar == 'L')
		head--;
}

void Turing::processTape() {
	// get past the blank cells
	while (tape[head] == 'D')
		head++;

  while (state.compare(accept) != 0 && state.compare("reject") != 0) {
  	bool found = false;
  	for (int i = 0; i < transitions.size(); i++) {
  		for (int j = 0; j < transitions[i].size(); j++) {
  			if (transitions[i][j].currentState.compare(state) == 0) {
  				if (transitions[i][j].readSymbol == tape[head]) {
  					myLittleHelper(transitions[i][j]);
  					found = true;
  					break;
  				}
  			}
  			else
  				break;
  		}
  		if (found)
  			break;
  	}

  	if (!found)
  		state = "reject";

  }

  if (state.compare(accept) == 0)
    cout << "Accepted!" << endl;
  else
    cout << "Rejected." << endl;
}
    

int main() {

	string file;
	cout << "Welcome to Tommy's Turing Machine simulator!\nEnter your file name here: ";
	getline(cin, file);
  Turing TM(file);

  string input;
  cout << "Your Turing Machine has successfully been simulated!" << endl << endl;
  cout << "(Your tape must start and end with 'D')" << endl;
  cout << "Insert your tape here >> ";
  getline(cin, input);
  TM.setTape(input);
  TM.processTape();

  return 0;
}