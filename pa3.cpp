/*
 * pa3.cpp
 *
 *  Created on: Apr 6, 2018
 *      Author: CarlRiley
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "pa3.h"

using namespace std;

//How to open and read files: https://www.youtube.com/watch?v=6vEf4n6NQtY
//How to open and read files: https://www.youtube.com/watch?v=Iho2EdJgusQ
//Vectors in C++: https://www.youtube.com/watch?v=Cq1h1KPoGBU
//Resetting to the beginning of the file after reading all of it: https://stackoverflow.com/questions/5343173/returning-to-beginning-of-file-after-getline

Stacks keywords;
Stacks identifiers;
Stacks constants;
Stacks operators;
Stacks delimiters;
Stacks errors;
Stacks nestedLoops;
Stacks parentheses;
string fileName;
ifstream myFile;

bool compareKeywords(string word) {
	if (word.compare("FOR") == 0 || word.compare("BEGIN") == 0 || word.compare("END") == 0) {
		return true;
	}
	return false;
}

void checkKeywords(string word) {
	if (compareKeywords(word) && !(keywords.exists(word))) {
		keywords.push(word);
	}
	else if (!compareKeywords(word) && !(errors.exists(word))) {
		errors.push(word);
	}
}

void checkIdentifiers(string word) {
	if (!identifiers.exists(word)) {
		identifiers.push(word);
	}
}

void checkConstants(string word) {
	if (!constants.exists(word)) {
		constants.push(word);
	}
}

void checkOperators(string word) {
	if (!operators.exists(word)) {
		operators.push(word);
	}
}

void checkDelimiters(string word) {
	if (!delimiters.exists(word)) {
		delimiters.push(word);
	}
}

int main() {
	while (true) {
		cout << "Input the name of the file (type 'quit' to quit the program)" << endl;
		cin >> fileName;
		if (fileName.compare("quit") == 0) {
			break;
		}
		myFile.open(fileName, std::fstream::in | std::fstream::out | std::fstream::app);

		if (myFile.fail()) {
			cout << "File Does Not Exist!" << endl;
			return 0;
		}

		char word[50];
		string valueToBeAdded = word;	//converts the series of characters in the word array into a string.
		int pos = 0;	//this position cycles through the string until it reaches the end of the desired word
		int initialPos = 0;		//initial position finds the beginning of the desired word.
		while (myFile >> word) {
			while (true) {
				//If the word is a keyword (it begins with a capital letter)
				if (word[pos] >= 65 && word[pos] <= 90) {
					while (word[pos] >= 65 && word[pos] <= 90) {
						pos++;
					}
					checkKeywords(word);
				}
				//If the word is an identifier (it begins with a lower case letter)
				else if (word[pos] >= 97 && word[pos] <= 122) {
					initialPos = pos;
					valueToBeAdded = word;
					//This while loop finds the identifier in the string
					while (word[pos] >= 97 && word[pos] <= 122) {
						pos++;
					}
					valueToBeAdded = valueToBeAdded.substr(initialPos, pos - initialPos);
					checkIdentifiers(valueToBeAdded);
				}
				//If the word is a number.
				else if (word[pos] >= 48 && word[pos] <= 57) {
					initialPos = pos;
					valueToBeAdded = word;
					//This while loop finds the number in the string.
					while (word[pos] >= 48 && word[pos] <= 57) {
						pos++;
					}
					valueToBeAdded = valueToBeAdded.substr(initialPos, pos - initialPos);
					checkConstants(valueToBeAdded);
				}
				//If the word is an operator
				else if (word[pos] == '*' || word[pos] == '+' || word[pos] == '-' || word[pos] == '/' || word[pos] == '=') {
					initialPos = pos;
					valueToBeAdded = word;
					//this while loop finds the operator in the string.
					while (word[pos] == '*' || word[pos] == '+' || word[pos] == '-' || word[pos] =='/' || word[pos] == '=') {
						pos++;
					}
					valueToBeAdded = valueToBeAdded.substr(initialPos, pos - initialPos);
					checkOperators(valueToBeAdded);
				}
				//If the word is a delimiter
				else if (word[pos] == ',' || word[pos] == ';') {
					valueToBeAdded = word[pos];
					checkDelimiters(valueToBeAdded);
					pos++;
				}
				//If the word is a parenthesis
				else if (word[pos] == '(' || word[pos] == ')') {
					if (word[pos] == '(') {
						valueToBeAdded = word[pos];
						parentheses.push(valueToBeAdded);
					}
					//pops the open parenthesis if it encounters the closing parenthesis
					else {
						parentheses.pop();
					}
					pos++;
				}
				//if the program has reached the end of the line and needs to move on to the next line in the file.
				if (word[pos] == '\0') {
					pos = 0;
					break;
				}
				else {
					continue;
				}
			}
		}

		myFile.clear();
		myFile.seekg(0, ios::beg);	//Returns the file back to the beginning.

		myFile >> word;
		pos = 0;
		//Checks to see if the parentheses are all good.
		while (myFile.good()) {
			if (word[pos] == '(') {
				valueToBeAdded = "(";
				if (parentheses.exists(valueToBeAdded) && !errors.exists(valueToBeAdded)) {
					errors.push(valueToBeAdded);
				}
				else {
					parentheses.push(valueToBeAdded);
				}
			}
			else if (word[pos] == ')') {
				parentheses.pop();
			}
			pos++;
			if (word[pos] == '\0') {
				myFile >> word;
				pos = 0;
			}
		}
		myFile.clear();
		myFile.seekg(0, ios::beg);	//Returns the file back to the beginning.
		pos = 0;
		int numOfNestedLoops = 0;
		bool checkForBegin = false;
		//Counts the depth of nested loops.
		while (myFile >> word) {
			valueToBeAdded = word;
			if (checkForBegin) {
				if (valueToBeAdded.compare("BEGIN") != 0 && !errors.exists("BEGIN")) {
					errors.push("BEGIN");
				}
				checkForBegin = false;
			}
			if (valueToBeAdded.compare("FOR") == 0) {
				numOfNestedLoops++;
				nestedLoops.push(valueToBeAdded);
			}
			if (valueToBeAdded.compare("BEGAN") == 0 && nestedLoops.exists("FOR")) {
				numOfNestedLoops--;
			}
			if (valueToBeAdded.compare("END") == 0 && nestedLoops.exists("FOR")) {
				nestedLoops.pop();
			}
			if (valueToBeAdded.find(")") != string::npos) {
				checkForBegin = true;
			}
		}
		if (nestedLoops.exists("FOR")) {
			errors.push("END");
		}

		cout << "The depth of the nested loop(s) is " << numOfNestedLoops - 1 << endl;
		cout << "Keywords: ";
		keywords.print();
		cout << endl << "Identifiers: ";
		identifiers.print();
		cout << endl << "Constants: ";
		constants.print();
		cout << endl << "Operators: ";
		operators.print();
		cout << endl << "Delimiters: ";
		delimiters.print();
		if (errors.isEmpty()) {
			errors.push("NA");
		}
		cout << endl << "Syntax Error(s): ";
		errors.print();
		cout << endl;

		myFile.close();
		keywords.clear();
		identifiers.clear();
		constants.clear();
		operators.clear();
		delimiters.clear();
		errors.clear();
		nestedLoops.clear();
		parentheses.clear();
	}
	return 0;
}


