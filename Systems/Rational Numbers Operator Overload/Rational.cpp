/* Rational.cpp -- Andrew Han, Nathaniel Bryant */

#include "Rational.h"

using std::abs;

Rational::Rational(){
	numerator = 0;
	denominator = 1;
}

Rational::Rational(const Rational &a){
	numerator = a.numerator;
	denominator = a.denominator;
}

Rational::Rational(const int wholeNumber){
	numerator = wholeNumber;
	denominator = 1;
}

Rational::Rational(const int num, const int denom){
	numerator = num;
	denominator = denom;
}

Rational::~Rational(){

}

void Rational::normalize(){
	int greater;
	int lesser;
	int gcd;
	int rcurrent;
	int rprev;
	int sign = 1;
	if(numerator < 0)
		sign = sign*-1;
	if(denominator < 0)
		sign = sign*-1;
	if(abs(numerator) > abs(denominator)){
		greater = abs(numerator);
		lesser = abs(denominator);
	}
	else if(abs(denominator) > abs(numerator)){
		greater = abs(denominator);
		lesser = abs(numerator);
	}
	else{
		numerator = 1*sign;
		denominator = 1;
		return;
	}
	while(rcurrent > 0){
		rprev = rcurrent;
		rcurrent = greater % lesser;
		greater = lesser;
		lesser = rcurrent;
	}
	gcd = rprev;
	numerator = abs(numerator)*sign/gcd;
	denominator = abs(denominator)/gcd;
}

ifstream &operator>>(ifstream& input, Rational &rational){
	char a;
	input >> rational.numerator;
	input >> a;
	if(a == ' '){
		input.putback(' ');
		rational.denominator = 1;
		return input;
	}
	else{
		input >> rational.denominator;
		return input;
	}
}

ostream &operator<<(ostream& output, const Rational &rational){
	if(rational.denominator == 1)
		output << rational.numerator;
	else
		output << rational.numerator << "/" << rational.denominator;
	return output;
}

bool Rational::operator==(Rational rational){
	double a = float(numerator)/float(denominator);
	double b = float(rational.numerator)/float(rational.denominator);
	return (a == b);
}

bool Rational::operator!=(Rational rational){
	double a = float(numerator)/float(denominator);
	double b = float(rational.numerator)/float(rational.denominator);
	return (a != b);
}

bool Rational::operator<(Rational rational){
	double a = float(numerator)/float(denominator);
	double b = float(rational.numerator)/float(rational.denominator);
	return (a < b);
}

bool Rational::operator<=(Rational rational){
	double a = float(numerator)/float(denominator);
	double b = float(rational.numerator)/float(rational.denominator);
	return (a <= b);
}

bool Rational::operator>(Rational rational){
	double a = float(numerator)/float(denominator);
	double b = float(rational.numerator)/float(rational.denominator);
	return (a > b);
}

bool Rational::operator>=(Rational rational){
	double a = float(numerator)/float(denominator);
	double b = float(rational.numerator)/float(rational.denominator);
	return (a >= b);
}

Rational Rational::operator+(const Rational &rational){
	long long int resultNum = (numerator * rational.denominator) + (rational.numerator * denominator);
	long long int resultDenom = denominator * rational.denominator;
	return *(normalizeLong(resultNum, resultDenom));
}

Rational Rational::operator-(const Rational &rational){
	long long int resultNum = (numerator * rational.denominator) - (rational.numerator * denominator);
	long long int resultDenom = denominator * rational.denominator;
	return *(normalizeLong(resultNum, resultDenom));
}

Rational Rational::operator*(const Rational &rational){
	long long int resultNum = numerator * rational.numerator;
	long long int resultDenom = denominator * rational.denominator;
	return *(normalizeLong(resultNum, resultDenom));
}

Rational Rational::operator/(const Rational &rational){
	long long int resultNum = numerator * rational.denominator;
	long long int resultDenom = denominator * rational.numerator;
	return *(normalizeLong(resultNum, resultDenom));
}

Rational* Rational::normalizeLong(long long int a, long long int b){
	int greater;
	int lesser;
	int gcd;
	int rprev;
	int sign = 1;
	if(a < 0)
		sign = sign*-1;
	if(b < 0)
		sign = sign*-1;
	if(abs(a) > abs(b)){
		greater = abs(a);
		lesser = abs(b);
	}
	else if(abs(b) > abs(a)){
		greater = abs(b);
		lesser = abs(a);
	}
	else{
		a = 1*sign;
		b = 1;
		return new Rational(a, b);
	}
	int rcurrent = lesser;
	while(rcurrent > 0){
		rprev = rcurrent;
		rcurrent = greater % lesser;
		greater = lesser;
		lesser = rcurrent;
	}
	gcd = rprev;
	a = abs(a)*sign/gcd;
	b = abs(b)/gcd;
	return new Rational(a, b);
}

double Rational::toDouble(){
	return double(numerator)/double(denominator);
}


int Rational::getNum(){
	return numerator;
}

int Rational::getDen(){
	return denominator;
}
