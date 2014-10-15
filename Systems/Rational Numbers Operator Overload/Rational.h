/* Rational.h -- Andrew Han, Nathaniel Bryant */

#include <cmath>
#include <iostream>
#include <fstream>

using std::ifstream;
using std::ostream;

class Rational{
	friend ifstream &operator>>(ifstream& input, Rational &);
	friend ostream &operator<<(ostream& output, const Rational &);
public:
	void normalize();
	Rational();
	Rational(const Rational &a);
	Rational(const int wholeNumber);
	Rational(const int num, const int denom);
	~Rational();
	Rational* normalizeLong(long long int a, long long int b);
	bool operator==(Rational rational);
	bool operator!=(Rational rational);
	bool operator<(Rational rational);
	bool operator<=(Rational rational);
	bool operator>(Rational rational);
	bool operator>=(Rational rational);
	Rational operator+(const Rational &rational);
	Rational operator-(const Rational &rational);
	Rational operator*(const Rational &rational);
	Rational operator/(const Rational &rational);
	double toDouble();
	int getNum();
	int getDen();
private:
	int numerator;
	int denominator;

};
