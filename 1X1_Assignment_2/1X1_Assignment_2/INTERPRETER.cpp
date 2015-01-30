/***************************************
 * INTERPRETER.cpp
 *
 * Student Name: Harry Pham
 * Student ID: 79422112
 * UCI Net ID: harrynp
 **************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cstring>

#define DATA_SEG_SIZE 100

class INTERPRETER {
public:
  INTERPRETER(char *sourceFile);
  void runProgram();

private:
  std::ifstream codeIn;
  std::ifstream inputIn;
  std::ofstream outFile;

  std::vector<std::string> C;
	int D[DATA_SEG_SIZE];
	int PC;
  std::string IR;
	bool run_bit;

  unsigned curIRIndex;

  void printDataSeg();

  void fetch();
  void incrementPC();
  void execute();

  // Output: used in the case of: set write, source
  void write(int source);

	// Input: used in the case of: set destination, read
  int read();

  /**
   * Checks and returns if the character c is found at the current
   * position in IR. If c is found, advance to the next
   * (non-whitespace) character.
   */
  bool accept(char c);

  /**
   * Checks and returns if the string s is found at the current
   * position in IR. If s is found, advance to the next
   * (non-whitespace) character.
   */
  bool accept(const char *s);

  /**
   * Checks if the character c is found at the current position in
   * IR. Throws a syntax error if c is not found at the current
   * position.
   */
  void expect(char c);

  /**
   * Checks if the string s is found at the current position in
   * IR. Throws a syntax error if s is not found at the current
   * position.
   */
  void expect(const char *s);

  void skipWhitespace();

  void parseStatement();
  void parseSet();
  void parseJump();
  void parseJumpt();
  int parseExpr();
  int parseTerm();
  int parseFactor();
  int parseNumber();
  char foundChar;
  std::string foundStr;

  void syntaxError();
};


INTERPRETER::INTERPRETER(char *sourceFile) {
  codeIn.open(sourceFile, std::fstream::in);
  if (codeIn.fail()) {
    std::cerr << "init: Errors accessing source file "
              << sourceFile << std::endl;
    exit(-2);
  }

  inputIn.open("input.txt", std::fstream::in);
  if (inputIn.fail()) {
    std::cerr << "init: Errors accessing input file input.txt" << std::endl;
    exit(-2);
  }

  outFile.open((std::string(sourceFile) + ".out").c_str(), std::fstream::out);
  if (outFile.fail()) {
    std::cerr << "init: Errors accessing output file "
              << std::string(sourceFile) + ".out" << std::endl;
    exit(-2);
  }

  // Initialize the SIMPLESEM processor state
  // Initialize the Code segment
  while (codeIn.good()) {
    std::string line;
    std::getline(codeIn, line);
    C.push_back(line);
  }

  // Initialize the Data segment
  for (int i=0; i<DATA_SEG_SIZE ;i++) {
    D[i]=0;
  }
  PC = 0; // Every SIMPLESEM program begins at instruction 0
  run_bit = true; // Enable the processor
}

void INTERPRETER::runProgram() {

  // TODO FETCH-INCREMENT-EXECUTE CYCLE
	while (run_bit)
	{
		//std::cout << "PC = " << PC << std::endl;
		curIRIndex = 0;
		fetch();
		incrementPC();
		execute();
	}
  printDataSeg();
}

void INTERPRETER::printDataSeg() {
  outFile << "Data Segment Contents" << std::endl;
  for(int i=0; i < DATA_SEG_SIZE; i++){
    outFile << i << ": " << D[i] << std::endl;
  }
}

void INTERPRETER::fetch() {
  // TODO
	IR = C[PC];
}

void INTERPRETER::incrementPC() {
  // TODO
	++PC;
}

void INTERPRETER::execute() {
  // TODO
	//std::cout << IR << std::endl;
	if (accept("halt"))
	{
		run_bit = false;
	}
	else if (accept("set"))
	{
		parseSet();
	}
	else if (accept("jumpt"))
	{
		parseJumpt();
	}
	else if (accept("jump"))
	{
		parseJump();
	}
}

//Output: used in the case of: set write, source
void INTERPRETER::write(int source){
  outFile << source << std::endl;
}

//Input: used in the case of: set destination, read
int INTERPRETER::read() {
  int value;
  inputIn >> value;
  std::cout << value << std::endl;
  return value;
}

/**
 * Checks and returns if the character c is found at the current
 * position in IR. If c is found, advance to the next
 * (non-whitespace) character.
 */
bool INTERPRETER::accept(char c) {
  if (curIRIndex >= IR.length())
    return false;

  if (IR[curIRIndex] == c) {
	  foundChar = c;
    curIRIndex++;
    skipWhitespace();
    return true;
  } else {
    return false;
  }
}

/**
 * Checks and returns if the string s is found at the current
 * position in IR. If s is found, advance to the next
 * (non-whitespace) character.
 */
bool INTERPRETER::accept(const char *s) {
  unsigned s_len = strlen(s);

  if (curIRIndex+s_len > IR.length())
    return false;

  for (unsigned i = 0; i < s_len; ++i) {
    if (IR[curIRIndex+i] != s[i]) {
      return false;
    }
  }
  foundStr = s;
  curIRIndex += s_len;
  skipWhitespace();
  return true;
}

/**
 * Checks if the character c is found at the current position in
 * IR. Throws a syntax error if c is not found at the current
 * position.
 */
void INTERPRETER::expect(char c) {
  if (!accept(c))
    syntaxError();
}

/**
 * Checks if the string s is found at the current position in
 * IR. Throws a syntax error if s is not found at the current
 * position.
 */
void INTERPRETER::expect(const char *s) {
  if (!accept(s))
    syntaxError();
}

void INTERPRETER::skipWhitespace() {
  while (curIRIndex < IR.length() && isspace(IR[curIRIndex]))
    curIRIndex++;
}

void INTERPRETER::parseStatement() {
  //std::cerr << "Statement" << std::endl;

  if (accept("halt"))
    return;
  else if(accept("set"))
    parseSet();
  else if(accept("jumpt"))
    parseJumpt();
  else if(accept("jump"))
    parseJump();
}

void INTERPRETER::parseSet() {
  //std::cerr << "Set" << std::endl;
  bool can_write = false, can_read = false;
  int lvalue = 0, rvalue = 0;
  if (!accept("write"))
  {
	  lvalue = parseExpr();
  }
  else
  {
	  can_write = true;
  }
  expect(',');

  if (!accept("read"))
  {
	  rvalue = parseExpr();
  }
  else
  {
	  can_read = true;
  }
  if (can_write)
  {
	  write(rvalue);
  }
  else if (can_read)
  {
	  D[lvalue] = read();
  }
  else
  {
	  D[lvalue] = rvalue;
  }
}

int INTERPRETER::parseExpr() {
  //std::cerr << "Expr" << std::endl;

  int value = parseTerm();

  while (accept('+') || accept('-'))
  {
	  if (foundChar == '+')
	  {
		  value += parseTerm();
	  }
	  else
	  {
		  value -= parseTerm();
	  }
  }
  return value;
}

int INTERPRETER::parseTerm() {
  //std::cerr << "Term" << std::endl;

  int value = parseFactor();

  while (accept('*') || accept('/') || accept('%'))
  {
	  if (foundChar == '*')
	  {
		  value *= parseFactor();
	  }
	  else if (foundChar == '/')
	  {
		  value /= parseFactor();
	  }
	  else
	  {
		  value %= parseFactor();
	  }
  }
  return value;
}

int INTERPRETER::parseFactor() {
  //std::cerr << "Factor" << std::endl;

  int value = 0;

  if (accept("D[")) {
    value = parseExpr();

    expect(']');
	return D[value];
  } else if (accept('(')) {
    value = parseExpr();

    expect(')');
  } else {
    value = parseNumber();
  }
  return value;
}

int INTERPRETER::parseNumber() {
  //std::cerr << "Number" << std::endl;
  std::string str = "";
  if (curIRIndex >= IR.length())
    syntaxError();

  if (IR[curIRIndex] == '0') {
    curIRIndex++;
    skipWhitespace();
    return 0;
  } else if (isdigit(IR[curIRIndex])) {
    while (curIRIndex < IR.length() &&
           isdigit(IR[curIRIndex])) {
		str += IR[curIRIndex];
      curIRIndex++;
    }
    skipWhitespace();
	return std::stoi(str);
  } else {
    syntaxError();
  }
}

void INTERPRETER::parseJump() {
  //std::cerr << "Jump" << std::endl;
  int value = parseExpr();
  PC = value;
}

//<Jumpt>-> jumpt <Expr>, <Expr> ( != | == | > | < | >= | <= ) <Expr>
void INTERPRETER::parseJumpt() {
  //std::cerr << "Jumpt" << std::endl;
  int lvalue = 0, rvalue = 0;
  bool isTrue = false;

  lvalue = parseExpr();

  expect(',');

  rvalue = parseExpr();

  if (accept("!=") || accept("==") || accept(">=") || accept("<="))
  {
	  if (foundStr == "!=")
	  {
		  isTrue = rvalue != parseExpr();
	  }
	  else if (foundStr == "==")
	  {
		  isTrue = rvalue == parseExpr();
	  }
	  else if (foundStr == ">=")
	  {
		  isTrue = rvalue >= parseExpr();
	  }
	  else
	  {
		  isTrue = rvalue <= parseExpr();
	  }
  }
  else if (accept('>') || accept('<'))
  {
	  if (foundChar == '>')
	  {
		  isTrue = rvalue > parseExpr();
	  }
	  else
	  {
		  isTrue = rvalue < parseExpr();
	  }
  }
  else {
    syntaxError();
  }
  if (isTrue)
  {
	  PC = lvalue;
  }
}

void INTERPRETER::syntaxError() {
  std::cerr << "Syntax Error!" << std::endl;
  exit(-1);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "init: This program requires an input "
              << "filename as a parameter " << std::endl;
    exit(-2);
  }
  INTERPRETER i(argv[1]);
  i.runProgram();
}
