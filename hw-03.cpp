////////////////////////////////////////////////////////////////////////////////
// @file hw-03.cpp
// @mainpage
// @author Joe Bruin	
// @date 2016-03-31
//
// Description: [You supply the details!]
//
// Known bugs: None so far!
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "integer.h"

// Get SignedInteger values from is, and push them onto a vector
// of signed integers, values
// @param is an input stream
// @param values a reference to 
void get_values(std::istream& is, std::vector<SignedInteger>& values);

// Prints the values to os, followed by the sum and product of the values
// @param os an output stream to write to
// @param values a vector SignedInteger values to be printed
void send_values(std::ostream& os, std::vector<SignedInteger>& values);

int main() {

	std::vector<SignedInteger> values;
	SignedInteger entry;

	do {

		std::cout << "Enter a very large integer (0 to stop): ";
		std::cin >> entry;

		values.push_back(entry);

	} while (!entry.is_zero());

	// Forget the 0 that the user entered!
	values.pop_back();

	std::string file_name;
	std::cout << "File name to read values: ";
	std::cin >> file_name;

	std::ifstream is;
	is.open(file_name.c_str());

	get_values(is, values);

	// Close is since we are done with it.
	is.close();

	std::cout << "File name to store values: ";
	std::cin >> file_name;

	std::ofstream os;
	os.open(file_name.c_str());

	send_values(os, values);
	send_values(std::cout, values);

	// Close os since we are done with it.
	os.close();

	return 0;
}

void get_values(std::istream& is, std::vector<SignedInteger>& values) {
	SignedInteger val;
	//int val;
	while (is >> val) {
		values.push_back(val);
	}
}


void send_values(std::ostream& os, std::vector<SignedInteger>& values) {
	SignedInteger sum(0);
	SignedInteger product(1);

	for (int i = 0; i < values.size(); ++i) {
		os << values[i] << std::endl;
		sum += values[i];
		product *= values[i];
	}

	os << "Sum = " << sum << std::endl;
	os << "Product = " << product << std::endl;
}