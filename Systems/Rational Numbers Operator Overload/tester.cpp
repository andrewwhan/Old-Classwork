/* tester.cpp -- Andrew Han, Nathaniel Bryant */

#include <iostream>
#include <fstream>
#include <string>
#include "Rational.h"

using std::ifstream;
using std::string;
using std::endl;
using std::cout;

int main(int argc, char* argv[]){
	for(int i = 1; i <= argc-1; i++){
		ifstream input;
		cout << argv[i] << endl;
		input.open(argv[i]);
		input >> std::noskipws;
		while(!input.eof()){
			Rational a = *(new Rational());
			Rational b = *(new Rational());
			char c;
			string op;
			input >> a;
			cout << a << " ";
			input.ignore(1);
			while(1){
				input >> b;
				cout << b << " ";
				input.ignore(1);
				input >> op;
				cout << op << " ";
				if(!(a.getDen()) || !(b.getDen())){
					cout << "ERROR" << endl;
					std::getline(input, op);
					break;
				}
				if(op == "+")
					a = a + b;
				else if(op == "-")
					a = a - b;
				else if(op == "*")
					a = a * b;
				else if(op == "/")
					a = a / b;
				else if(op == "=="){
					if(a == b)
						cout << "\t: true" << endl;
					else
						cout << "\t: false" << endl;
					input >> c;
					if(c != '\n'){
						cout << "ERROR" << endl;
						std::getline(input, op);
					}
					break;
				}
				else if(op == "!="){
					if(a != b)
						cout << "\t: true" << endl;
					else
						cout << "\t: false" << endl;
					input >> c;
					if(c != '\n'){
						cout << "ERROR" << endl;
						std::getline(input, op);
					}
					break;
				}
				else if(op == "<"){
					if(a < b)
						cout << "\t: true" << endl;
					else
						cout << "\t: false" << endl;
					input >> c;
					if(c != '\n'){
						cout << "ERROR" << endl;
						std::getline(input, op);
					}
					break;
				}
				else if(op == "<="){
					if(a <= b)
						cout << "\t: true" << endl;
					else
						cout << "\t: false" << endl;
					input >> c;
					if(c != '\n'){
						cout << "ERROR" << endl;
						std::getline(input, op);
					}
					break;
				}
				else if(op == ">"){
					if(a > b)
						cout << "\t: true" << endl;
					else
						cout << "\t: false" << endl;
					input >> c;
					if(c != '\n'){
						cout << "ERROR" << endl;
						std::getline(input, op);
					}
					break;
				}
				else if(op == ">="){
					if(a >= b)
						cout << "\t: true" << endl;
					else
						cout << "\t: false" << endl;
					input >> c;
					if(c != '\n'){
						cout << "ERROR" << endl;
						std::getline(input, op);
					}
					break;
				}
				else{
					cout << "ERROR" << endl;
					break;
				}
				input >> c;
				if(c == '\n' || input.eof()){
					cout << "\t: " << a << " (double " << a.toDouble() << ")" << endl;
					break;
				}
			}
		}
		input.close();
	}
}
