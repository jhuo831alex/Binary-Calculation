////////////////////////////////////////////////////////////////////////////////
// @file hw-01.tex
// @mainpage
// @author Jiahao Huo	
// @date 2016-03-31
//
// Description: the testing file for class Integer. Testing +, -, *, ++, and *= 
//              relating to class Integer
//
// Known bugs: NA
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "integer.h"

int main() {

	int a_int, b_int;

	std::cout << "Please provide a positive integer a = ";
	std::cin >> a_int;

	std::cout << "Please provide a larger positive integer b = ";
	std::cin >> b_int;


	Integer a(a_int);
	Integer b(b_int);

	Integer c = a + b;
	Integer d = a - b;
	Integer e = a * b;

	std::cout << "In binary: " << std::endl
		<< "a = " << a.binary_string() << std::endl
		<< "b = " << b.binary_string() << std::endl
		<< "a + b = " << c.binary_string() << std::endl
		<< "b - a = " << d.binary_string() << std::endl
		<< "a * b = " << e.binary_string() << std::endl
		<< std::endl;


	std::cout << "Incrementing a..." << std::endl;

	++a;

	std::cout << "a = " << a.binary_string() << std::endl << std::endl;

	int n;

	std::cout << "Please provide a positive integer less than 40, n = ";
	std::cin >> n;

	std::cout << "Computing " << n << "!..." << std::endl;

	c = Integer(1);
	d = Integer(1);

	for (int i = 1; i <= n; ++i) {
		c *= d;
		++d;
	}

	std::cout << n << "! = " << c.binary_string()
		<< std::endl << std::endl;

	return 0;
}