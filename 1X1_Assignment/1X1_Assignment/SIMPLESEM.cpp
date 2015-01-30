/***************************************
* SIMPLESEM.cpp
*
* Student Name: Harry Pham
* Student ID: 79422112
* UCI Net ID: harrynp
**************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <cstdio> // for EOF
#include <cstdlib>
#include <algorithm>
#include <vector>

class SIMPLESEM {
public:
	SIMPLESEM(std::string sourceFile);
	void parseProgram();

private:
	int nextChar; //contains the character (or EOF)
	int number;

	std::ifstream inFile;
	std::ofstream outFile;

	void printRule(std::string rule) {
		outFile << rule << std::endl;
	}

	void throw_error(const std::string &str);
	void parseStatement(std::string str);
	void parseExpr(const std::vector<std::string> &tokens);
	void parseTerm(const std::vector<std::string> &tokens, unsigned int &index);
	void parseFactor(const std::string &str);
	bool checkNumber(const std::string &str);
	bool contains(const char &c);
	bool is_symbol(const std::string &str);
	std::vector<std::string> split_by_comma(const std::string &str);
	std::vector<std::string> split_by_bool(const std::string &str);
	std::vector<std::string> split_by_symbol(const std::string &str);
};


SIMPLESEM::SIMPLESEM(std::string sourceFile) {
	inFile.open(sourceFile.c_str(), std::fstream::in);
	if (inFile.fail()) {
		std::cerr << "init: Errors accessing source file "
			<< sourceFile << std::endl;
		exit(-2);
	}

	//nextChar = inFile.get();
	outFile.open((sourceFile + ".out").c_str(), std::fstream::out);
	if (outFile.fail()) {
		std::cerr << "init: Errors accessing output file "
			<< sourceFile + ".out" << std::endl;
		exit(-2);
	}
}

void SIMPLESEM::parseProgram() {
	printRule("Program");
	//parseStatement();
	//while (nextChar != EOF) {
	//	parseStatement();
	//}
	std::string line;
	while (getline(inFile, line))
	{
		parseStatement(line);
	}
}

void SIMPLESEM::parseStatement(std::string str) {
	printRule("Statement");
	if (str.substr(0, 3) == "set")
	{
		std::cout << str << std::endl;
		printRule("Set");
		str.erase(0, 3);
		std::cout << str << std::endl;
		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
		std::cout << str << std::endl;
		std::vector<std::string> tokens = split_by_comma(str);
		if (tokens.size() == 0)
		{
			throw_error("Error parsing file. Exiting.");
		}
		if (tokens[0] != "write")
		{
			parseExpr(split_by_symbol(tokens[0]));
		}
		if (tokens.size() < 2)
		{
			throw_error("Error parsing file. Exiting.");
		}
		if (tokens[1] != "read")
		{
			parseExpr(split_by_symbol(tokens[1]));
		}
		if (tokens.size() > 2)
		{
			throw_error("Error parsing file. Exiting.");
		}
	}
	else if (str.substr(0, 5) == "jumpt")
	{
		printRule("Jumpt");
		str.erase(0, 5);
		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
		std::vector<std::string> tokens = split_by_comma(str);
		if (tokens.size() == 0)
		{
			throw_error("Error parsing file. Exiting.");
		}
		parseExpr(split_by_symbol(tokens[0]));
		if (tokens.size() < 2)
		{
			throw_error("Error parsing file. Exiting.");
		}
		std::vector<std::string> bool_tokens = split_by_bool(tokens[1]);
		if (bool_tokens.size() == 0)
		{
			throw_error("Error parsing file. Exiting.");
		}
		parseExpr(split_by_symbol(bool_tokens[0]));
		if (tokens.size() < 2)
		{
			throw_error("Error parsing file. Exiting.");
		}
		parseExpr(split_by_symbol(bool_tokens[1]));
		if (tokens.size() > 2 || bool_tokens.size() > 2)
		{
			throw_error("Error parsing file. Exiting.");
		}
	}
	else if (str.substr(0, 4) == "jump")
	{
		printRule("Jump");
		str.erase(0, 4);
		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
		std::vector<std::string> tokens = split_by_symbol(str);
		parseExpr(tokens);
	}
	else if (str.substr(0, 4) == "halt" && str.length() == 4)
	{
		return;
	}
	else
	{
		throw_error("Error parsing file. Exiting.");
	}
}

void SIMPLESEM::parseExpr(const std::vector<std::string> &tokens)
{
	printRule("Expr");
	unsigned int start = 0;
	parseTerm(tokens, start);
	++start;
	for (unsigned int i = start; i<tokens.size(); i++){
		if (tokens[i] == "+" || tokens[i] == "-"){
			if (i + 1 == tokens.size() && (tokens[i] == "+" || tokens[i] == "-")){
				throw_error("parseExpr: Missing Term");
			}
			else if (is_symbol(tokens[i + 1])){
				throw_error("parseExpr: Invalid Term");
			}
		}
		else{
			if (i % 2 == 1){
				throw_error("parseExpr: Invalid Term");
			}
			parseTerm(tokens, i);
		}
	}
}

void SIMPLESEM::parseTerm(const std::vector<std::string> &tokens, unsigned int &index)
{
	printRule("Term");
	parseFactor(tokens[index]);
	++index;
	for (index; index < tokens.size(); index++){
		if (tokens[index] == "*" || tokens[index] == "/" || tokens[index] == "%"){
			if (index + 1 == tokens.size() && (tokens[index] == "*" || tokens[index] == "/" || tokens[index] == "%")){
				throw_error("parseExpr: Missing Term");
			}
			else if (is_symbol(tokens[index + 1])){
				throw_error("parseExpr: Invalid Term");
			}
		}
		else if (tokens[index] == "+" || tokens[index] == "-")
		{
			--index;
			break;
		}
		else{
			if (index % 2 == 1){
				throw_error("parseExpr: Invalid Term");
			}
			parseFactor(tokens[index]);
		}
	}
}

void SIMPLESEM::parseFactor(const std::string &str)
{
	printRule("Factor");
	if (str[0] == 'D' && str[1] == '['){
		parseExpr(split_by_symbol(str.substr(2, str.size() - 3)));
	}
	else if (str[0] == '('){
		parseExpr(split_by_symbol(str.substr(1, str.size() - 2)));
	}
	else if (checkNumber(str)){
		printRule("Number");
	}
	else{
		throw_error("parseExpr: Invalid Factor");
	}
}

std::vector<std::string> SIMPLESEM::split_by_symbol(const std::string &str){
	std::vector<std::string> result;
	std::string temp;
	char end_sym;
	char start_sym;
	int counter = 0;

	for (unsigned int x = 0; x < str.size(); x++){
		if (counter == 0){
			if (contains(str[x])){
				result.push_back(temp);
				result.push_back(std::string(1, str[x]));
				temp.clear();
			}
			else{
				temp += str[x];
				if (x + 1 == str.size()){
					result.push_back(temp);
				}
				else if (temp == "D["){
					start_sym = '[';
					end_sym = ']';
					counter++;
				}
				else if (temp == "("){
					start_sym = '(';
					end_sym = ')';
					counter++;
				}
			}
		}
		else{
			temp += str[x];
			if (str[x] == end_sym){
				counter--;
				if (x + 1 == str.size()){
					result.push_back(temp);
				}
			}
			else if (str[x] == start_sym){
				counter++;
			}
		}
	}
	return result;
}

bool SIMPLESEM::is_symbol(const std::string &str){
	return str == "+" || str == "-" || str == "*" || str == "/" || str == "%";
}

bool SIMPLESEM::contains(const char &c){
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}


std::vector<std::string> SIMPLESEM::split_by_comma(const std::string &str)
{
	std::vector<std::string> tokens;
	std::size_t found = str.find(",");
	if (found != std::string::npos)
	{
		tokens.push_back(str.substr(0, found));
		tokens.push_back(str.substr(found + 1, str.length()));
	}
	return tokens;
}

std::vector<std::string> SIMPLESEM::split_by_bool(const std::string &str)
{
	std::vector<std::string> tokens;
	std::size_t found = str.find_first_of("<>!=");
	if (found != std::string::npos)
	{
		tokens.push_back(str.substr(0, found));
		if (str[found] == '>' || str[found] == '<')
		{
			if (str[found + 1] == '=')
			{
				tokens.push_back(str.substr(found + 2, str.length()));
			}
			else
			{
				tokens.push_back(str.substr(found + 1, str.length()));
			}
		}
		else if (str[found] == '!' || str[found] == '=')
		{
			tokens.push_back(str.substr(found + 2, str.length()));
		}
	}
	return tokens;
}

void SIMPLESEM::throw_error(const std::string &str)
{
	std::cerr << str << std::endl;
	exit(-2);
}

bool SIMPLESEM::checkNumber(const std::string &str)
{
	if (str.find_first_not_of("0123456789") == std::string::npos)
	{
		return true;
	}
	return false;
}


int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "init: This program requires an input "
			<< "filename as a parameter " << std::endl;
		exit(-2);
	}
	SIMPLESEM s(argv[1]);
	s.parseProgram();
}
