////////////////////////////////////////////////////////////////////////////////
// @file hw-02.tex
// @mainpage
// @author Joe Bruin	
// @date 2016-03-31
//
// Description: testing file for integer.h and integer.cpp
//
// Known bugs: Division is sometimes off by 1.
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "integer.h"

int main() {

	int a, b;
	SignedInteger a_int, b_int, sum, diff, prod, quot, mod;


	do {
		std::cout << "Provide an integer: a = ";
		std::cin >> a;

		std::cout << "Provide another integer (0 to stop): b = ";
		std::cin >> b;

		std::cout << std::endl;

		a_int = SignedInteger(a);
		b_int = SignedInteger(b);

		sum = a_int + b_int;

		std::cout << "a + b = " << sum.decimal_string() << " = "
			<< sum.binary_string() << std::endl;

		diff = a_int - b_int;

		std::cout << "a - b = " << diff.decimal_string() << " = "
			<< diff.binary_string() << std::endl;

		prod = a_int * b_int;

		std::cout << "a * b = " << prod.decimal_string() << " = "
			<< prod.binary_string() << std::endl;

		quot = a_int / b_int;

		std::cout << "a / b = " << quot.decimal_string()
			<< " = " << quot.binary_string() << std::endl;

		mod = a_int % b_int;

		std::cout << "a % b = " << mod.decimal_string() << " = "
			<< mod.binary_string() << std::endl
			<< std::endl;

	} while (b != 0);

	std::vector<Integer*> values;
	Integer* a_ptr = nullptr;

	std::cout << "Enter some more integers! (0 to end): ";
	std::cin >> a;

	while (a != 0) {
		if (a < 0) {

			a_ptr = new SignedInteger(a);

		}
		else {

			a_ptr = new Integer(a);

		}

		values.push_back(a_ptr);

		std::cout << "Enter some more integers! (0 to end): ";
		std::cin >> a;
	}

	a_ptr = nullptr;

	int num_entries = values.size();

	std::cout << std::endl << "The values you entered (in decimal) were: " << std::endl;
	for (int i = 0; i < num_entries; ++i) {
		std::cout << values[i]->decimal_string();

		if (i < num_entries - 1) {
			std::cout << ", ";
		}
	}

	std::cout << std::endl << "The values you entered (in binary) were: " << std::endl;

	for (int i = 0; i < num_entries; ++i) {
		std::cout << values[i]->binary_string();

		if (i < num_entries - 1) {
			std::cout << ", ";
		}
	}

	std::cout << std::endl << std::endl;

	int num_inc;
	std::cout << "Enter a positive integer : ";
	std::cin >> num_inc;

	std::cout << std::endl << "Incrementing each number you entered "
		<< num_inc << " times..." << std::endl;

	for (int i = 0; i < num_entries; ++i) {
		for (int j = 0; j < num_inc; ++j) {
			++(*values[i]);
		}
	}

	std::cout << std::endl << "The incremented values are: " << std::endl;
	for (int i = 0; i < num_entries; ++i) {
		std::cout << values[i]->decimal_string();

		if (i < num_entries - 1) {
			std::cout << ", ";
		}
	}

	std::cout << std::endl << std::endl;

	return 0;
}