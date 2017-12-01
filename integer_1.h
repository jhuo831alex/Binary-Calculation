#include <vector>
#include <string>

#ifndef INTEGER_H
#define INTEGER_H

////////////////////////////////////////////////////////////////////////////////
// @file integer.h
// @author: Jiahao Huo
// @date April 6, 2016
//
// @class: Integer
// @description: This Integer class, unlike datatype Int, can store as many bits
//				needed to represent itself. In this class, integers are represented
//              by binary strings. These binary strings are stored as boolean vectors.
//              Operators +=, -=, *= and ++ are overloaded in this class.          
//              Note: this class can only be operated on positive integers.
//              Example Usage:
//              Integer a(3)   represented by 11
//              Integer b(4)   represented by 100
//              a+=b           now a is 111 
//              a.boolvector()   return a boolean vector 111
//              a.binary_string()   return a string "111"
// 
//              This file also includes binary addition, subtraction and multiplication,
//              and a function that can append zeros at the end of the binary strings. 
////////////////////////////////////////////////////////////////////////////////

class Integer {
public:
	Integer() : integer_(0){} // default constructor: value should be 0
	Integer(int a); // store an int into an Integer
	Integer(std::vector<bool>v); // store a boolean vector into an Integer
	std::string binary_string(); // convert a binary from a boolean vector to a string 
	std::vector<bool> boolvector() const { return integer_; } // return the boolean vector of an Integer
	Integer& operator += (const Integer& rhs); 
	Integer& operator -= (const Integer& rhs);
	Integer& operator *= (const Integer& rhs);
	Integer& operator ++(); // Prefix increment
private:
	std::vector <bool> integer_; //a boolean vector that represents the value of an integer
	std::vector <bool> sign_; // a boolean vector that represents the sign of an integer
};

//Overloads the operator + 
//@param lhs the first integer
//@param rhs the second integer
//return the sum of lhs and rhs
Integer operator + (Integer lhs, const Integer& rhs);

//Overloads the operator -
//@param lhs the minuend 
//@param rhs the subtrahend
//return the result of subtract rhs from lhs
Integer operator - (Integer lhs, const Integer& rhs);

//Overloads the operator * 
//@param lhs the first factor
//@param rhs the second factor
//return the product of lhs and rhs
Integer operator * (Integer lhs, const Integer& rhs);

//append zeros at the end of the boolean vector
//@param v a boolean vector consists only 1 and 0
//@param int a how many zeros are appended after the vector
//return a boolean vector with appended zeros
Integer shift (std::vector<bool> v, int a);

#endif