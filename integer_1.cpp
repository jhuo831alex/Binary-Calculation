#include <iostream>
#include <vector>
#include <algorithm>
#include "integer.h"

Integer::Integer(int a){
	int rem;	
	do // convert decimal to binary
	{
		// convert from the first digit (the left most digit)
		rem = a % 2; 
		integer_.push_back(rem); // add to the end of the vector
		a = a / 2; 
	} while (a > 0);
	std::reverse(integer_.begin(), integer_.end()); // the vector is reversed so that it is in the right order
	if (a >= 0){
		sign_ = { 0 }; // 0 for positive integer
	}
	else{
		sign_ = { 1 }; // 1 for negative integer
	}
}

Integer::Integer(std::vector<bool> v){
	integer_ = v;
}

// Convert the binary vector to a string
std::string Integer::binary_string(){	
	std::string print= "(";
	for (int i = 0; i < integer_.size(); ++i){
		if (integer_[i] == true){
			print += "1"; // true 1
		}
		else{
			print += "0"; // flase 0
		}
	}
	print += ")_2";
	return print;
}

Integer& Integer::operator += (const Integer& rhs){
	int sum, carry = 0;
	std::vector<bool>temp = rhs.integer_;//create a copy of the vector rhs.integer_ for resizing

	if (integer_.size() <= temp.size()){
		// since doing addition is from the right most digit to the left most one 
		// the vectors need to be reverse
		std::reverse(integer_.begin(), integer_.end()); 
		std::reverse(temp.begin(), temp.end());
		// expand vector integer_ to the size of temp
		// so as to add zeros to the end of vector integer_ for easy computation 
		integer_.resize(temp.size()); 
	}
	else{
		// same idea as above
		std::reverse(temp.begin(), temp.end());
		std::reverse(integer_.begin(), integer_.end());
		temp.resize(integer_.size());
	}

	// different cases for binary addition
	// when 1+1, carry = 1
	// carry 0: 1+0=1; 1+1=0; 0+0=0
	// carry 1: 1+0=0; 1+1=1; 0+0=1
	for (int i = 0; i < integer_.size(); ++i){
		if (integer_[i] == true && temp[i] == true && carry == 0){
			sum = 0;
			carry = 1;
			integer_[i] = sum;
		}
		else if (integer_[i] == true && temp[i] == false && carry == 0){
			sum = 1;
			carry = 0;
			integer_[i] = sum;
		}
		else if (integer_[i] == false && temp[i] == false && carry == 0){
			sum = 0;
			carry = 0;
			integer_[i] = sum;
		}
		else if (integer_[i] == false && temp[i] == true && carry == 0){
			sum = 1;
			carry = 0;
			integer_[i] = sum;
		}
		else if (integer_[i] == true && temp[i] == true && carry == 1){
			sum = 1;
			carry = 1;
			integer_[i] = sum;
		}
		else if (integer_[i] == true && temp[i] == false && carry == 1){
			sum = 0;
			carry = 1;
			integer_[i] = sum;
		}
		else if (integer_[i] == false && temp[i] == false && carry == 1){
			sum = 1;
			carry = 0;
			integer_[i] = sum;
		}
		else if (integer_[i] == false && temp[i] == true && carry == 1){
			sum = 0;
			carry = 1;
			integer_[i] = sum;
		}
	}

	// when carry = 1 means that a 1 should be added to the vector
	// exampe: 11+11=110
	if (carry == 1){
		integer_.push_back(1);
		std::reverse(integer_.begin(), integer_.end()); // reverse the vector back 
	}
	else{
		std::reverse(integer_.begin(), integer_.end());
	}
	return *this;
}
		
Integer& Integer::operator -= (const Integer& rhs){
	int sub, carry = 0;
	std::vector<bool>temp = rhs.integer_;//create a copy of the vector rhs.integer_ for resizing
	// assumption when a-=b, a>=b
	if (integer_.size() >= rhs.integer_.size()){
		// same as doing addition 
		// doing subtraction is also from the right most digit to the left most one 
		// the vectors need to be reverse
		std::reverse(temp.begin(), temp.end()); 
		std::reverse(integer_.begin(), integer_.end());
		// expand vector integer_ to the size of temp
		// so as to add zeros to the end of vector integer_ for easy computation 
		temp.resize(integer_.size());

		// when 0-1, carry = -1
		// carry = 0: 1-1=0; 1-0=1; 0-1=1
		// carry = -1: 1-1=1; 1-0=0; 0-1=0
		for (int i = 0; i < integer_.size(); ++i){
			if (integer_[i] == true && temp[i] == true && carry == 0){
				sub = 0;
				carry = 0;
				integer_[i] = sub;
			}
			else if (integer_[i] == true && temp[i] == false && carry == 0){
				sub = 1;
				carry = 0;
				integer_[i] = sub;
			}
			else if (integer_[i] == false && temp[i] == false && carry == 0){
				sub = 0;
				carry = 0;
				integer_[i] = sub;
			}
			else if (integer_[i] == false && temp[i] == true && carry == 0){
				sub = 1;
				carry = -1;
				integer_[i] = sub;
			}
			else if (integer_[i] == true && temp[i] == true && carry == -1){
				sub = 1;
				carry = -1;
				integer_[i] = sub;
			}
			else if (integer_[i] == true && temp[i] == false && carry == -1){
				sub = 0;
				carry = 0;
				integer_[i] = sub;
			}
			else if (integer_[i] == false && temp[i] == false && carry == -1){
				sub = 1;
				carry = -1;
				integer_[i] = sub;
			}
			else if (integer_[i] == false && temp[i] == true && carry == -1){
				sub = 0;
				carry = -1;
				integer_[i] = sub;
			}
		}

		// detect the redundant zeros and delete them 
		for (int i = integer_.size()-1; i >= 0; --i){
			if (integer_[i] == 0){
				integer_.pop_back();
			}
			else if (integer_[i] == 1){
				break;
			}
		}

		// reverse the vector back so that it is in the right order
		std::reverse(integer_.begin(), integer_.end()); 
	}
	return *this;
}

Integer& Integer:: operator *= (const Integer& rhs){
	std::vector<bool>temp = rhs.integer_; //create a copy of vector rhs.integer_
	Integer a; 
	// since the index of a vector and the index of binary string has reverse order
	// the vector is reversed
	std::reverse(integer_.begin(), integer_.end()); 
	for (int i = 0; i < integer_.size(); ++i){
		if (integer_[i] == 1){
			// shift rhs by i and add them together
			a += shift(temp, i);
		}
	}
	integer_ = a.boolvector(); 

	return *this;
}

Integer& Integer::operator ++(){
	Integer a = Integer(integer_) + Integer(1);
	integer_ = a.boolvector();
	return *this;
}

Integer operator + (Integer lhs, const Integer& rhs){
	lhs += rhs;
	return lhs;
}

Integer operator - (Integer lhs, const Integer& rhs){
	lhs -= rhs;
	return lhs;
}

Integer operator * (Integer lhs, const Integer& rhs){
	lhs *= rhs;
	return lhs;
}

Integer shift(std::vector<bool> v, int a){
	v.resize(v.size() + a); // expand the vector v by a index, the added elements are all zeros
	return Integer(v);
}