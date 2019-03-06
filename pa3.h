/*
 * pa3.h
 *
 *  Created on: Apr 7, 2018
 *      Author: CarlRiley
 */

#ifndef PA3_H_
#define PA3_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Stacks {

public:
	Stacks();
	string pop();
	void push(string word);
	void print();
	void clear();
	bool exists(string word);
	bool isEmpty();

private:
	vector<string> program;
};

Stacks::Stacks() {
}

string Stacks::pop() {
	string wordToReturn;
	wordToReturn = program[0];
	program.erase(program.begin());
	return wordToReturn;
}
void Stacks::push(string word) {
	program.insert(program.begin(), word);	//Adds the value to the beginning of the vector.
}

void Stacks::print() {
	for (vector<string>::iterator it = program.begin(); it != program.end(); it++) {
		cout << *it << " ";
	}
}

void Stacks::clear() {
	program.clear();
}

bool Stacks::exists(string word) {
	for (unsigned int i = 0; i < program.size(); i++) {
		if (word.compare(program[i]) == 0) {
			return true;
		}
	}
	return false;
}

bool Stacks::isEmpty() {
	if (program.empty()) {
		return true;
	}
	return false;
}

#endif /* PA3_H_ */
