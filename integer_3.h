////////////////////////////////////////////////////////////////////////////////
// @file integer.h
// @author Will	
// @date 2016-03-31
//
// Description: This is the header file for the Integer class. The Integer class
// performs basic integer arithmetic. Unlike the fundamental datatype int, an
// Integer dynamically rescales its size to accomodate much larger values than
// ints.
//
// An Integer only supports non-negative numbers, and the operators
// +, *, -, /, %, and their corresponding assignment operators (+=, etc.).
//
// To represent positive and negative integers, we have the derived class
// SignedInteger. SignedIntegers have all of the same functionalities as
// Integers, but they addtionally perform arithmetic on negative as well as
// positive numbers.
//
// Known bugs:
////////////////////////////////////////////////////////////////////////////////

#ifndef INTEGER_H
#define INTEGER_H

#include <string>
#include <vector>
#include <iostream>


////////////////////////////////////////////////////////////////////////////////
// @class Integer
// @brief Stores an Integer as a vector of bools. Supports basic arithmetic.
//
// This Integer class supports basic arithemetic: addition, subtraction, and
// multiplication. Currently it only allows for non-negative values. For example
// if an Integer b is larger than a, then the expression a - b may have
// arbitrary output. 
////////////////////////////////////////////////////////////////////////////////

class Integer{

public:
	// Default constructor--default value is 0
	Integer();

	// Constructor assigns a value val to bits_
	Integer(unsigned int val);

	// Produce a string which contains the bits of
	// the Integer (stored in bits_)
	// @return an std::string in the form (...)_2 where "..." denotes the bits
	// of the Integer
	virtual std::string binary_string() const;

	// Produce a string which contains the digits of
	// the Integer (stored in bits_)
	// @return an std::string giving the decimal representation of the
	// calling Integer.
	virtual std::string decimal_string() const;

	// Return the number of bits stored in bits_.
	// @return an int containing the number of its in bits_.
	inline int size() const { return bits_.size(); }

	// Return the value of the bit stored at a particular index. If index
	// exceeds the number of bits, get_bit returns "false"
	// @param index is the desired index (an unsigned int)
	// @return the boolean value of bits_[index] if index < bits_.size(), and
	// "false" otherwise.
	bool get_bit(unsigned int index) const;

	// Sets the bit at index in bits_ to value. If value exceeds bits_.size(),
	// then value is pushed onto the back of bits_.
	// @param index an unsigned int indicating the desired location
	// @param value is the value to be assigned to bits_[index]
	void set_bit(unsigned int index, bool value);

	// Shifts the bits of the Integer one to the left. Equivalently, this
	// operation can be interpreted as multiplying the Integer by 2.
	void left_shift();

	// Shifts the bits of the Integer one to the right. Equivalently, this
	// operation can be interpreted as dividing the Integer by 2.
	void right_shift();

	// Tells if an Integer is 0.
	// @return a bool indicating if the calling Integer is zero.
	bool is_zero() const;

	// Returns the value of the calling Integer as an unsigned int. Note that if
	// the calling Integer is larger than the maximum value that can be stored as
	// an unsigned int, then the value returned will not be equal to the value
	// stored in the calling Integer. This function is thus intended to be used
	// only when the value of the Integer is known to be relatively small.
	// @return an unsigned int whose value is equal to the value of the calling
	// Integer.
	unsigned int get_int_value() const;

	// Assigns the value stored in val to this instance of Integer.
	// @param val a reference to the Integer which stores the desired new
	// value
	void set_value(const Integer& val);

	// Computes the complement of this Integer by replacing each of its
	// bits with the bits complement. That is, each "1" gets flipped to "0"
	// and vice versa.
	void complement();

	// Adds the Integer on the right side to the current integer.
	// @param rhs is a pointer to the Integer to be added
	// @return a reference to this integer after addition
	Integer& operator+=(const Integer& rhs);

	// Subtracts the Integer on the right side from the current integer. If the
	// value of rhs is larger than this instance, the new value will be the absolute
	// value of the difference. In the following example
	//   Integer a(3);
	//   Integer b(5);
	//   a -= b;
	// the new value of a will be |3 - 5| = 2.
	// @param rhs is a pointer to the Integer to be subtracted
	// @return a reference to this integer after subtraction
	Integer& operator-=(const Integer& rhs);

	// Multiplies this Integer by the Integer on the right hand side.
	// @param rhs is a pointer to the Integer to multiply
	// @return a reference to this integer after multiplication
	Integer& operator*=(Integer rhs);

	// Increment this integer by one (prefix)
	// @return a reference to this integer (after incrementation).
	virtual Integer& operator++();

	// Increment this integer by one (postfix)
	// @return previous value (before incrementation).
	Integer operator++(int unused);

	// Divide this Integer by an Integer, rhs. Note that in Integer division,
	// the result is an Integer, and there is no remainder. For example
	//   Integer a(5);
	//   Integer b(2);
	//   a /= 2; // a will now have the value 2.
	// @param rhs an Integer which will divide the instance which calls /=
	// @return a reference to the calling integer.  
	Integer& operator/=(Integer rhs);

	// Compute the modulus/remainder (%) of the calling Integer upon division
	// by the Integer rhs.
	//   Integer a(5);
	//   Integer b(2);
	//   a %= 2; // a will now have the value 1.
	// @param rhs an Integer which will divide the instance which calls %=
	// @return a reference to the calling integer (with its new value)
	Integer& operator%=(Integer rhs);

private:
	// The bits that store the value of an Integer. Bits are stored
	// from least to most significant.
	std::vector<bool> bits_;

	// Remove the trailing zeros from an Integer. For example,
	// (00010110)_2 should be written as (10110)_2.
	void remove_trailing_zeros();

};

// Add two bits b1 and b2, along with a carry bit, for example:
// 0 + 0 + 0 = 0 carry 0 (value is 0)
// 1 + 0 + 0 = 1 carry 0 (value is 1)
// 1 + 1 + 0 = 0 carry 1 (value is 10, must carry the 1)
// 1 + 1 + 1 = 1 carry 1 (value is 11, must carry the 1)
// @param b1 the first bit to be added (stored as a refernce to a  bool)
// @param b2 the second bit to be added
// @param carry the carry bit (a reference to a bool)
void add_with_carry(bool& b1, bool b2, bool& carry);

// Add two integers and return the value of the result.
// @param the left Integer to be added
// @param the right Integer to be added (a reference)
// @return the sum of lhs and rhs.
Integer operator+(Integer lhs, const Integer& rhs);

// Subtract one Integer from another and return the value of the result.
// This function assumes that result is non-negative. If the result is negative,
// the output of the function is unspecified.
// @param the left Integer
// @param the right Integer to be subtracted (a reference)
// @return the difference of lhs and rhs
Integer operator-(Integer lhs, const Integer& rhs);

// Multiply two integers and return the value of the result.
// @param the left Integer to be multiplied
// @param the right Integer to be multiplied (a reference)
// @return the product of lhs and rhs.
Integer operator*(Integer lhs, const Integer& rhs);

// Divide two integers and return the value of the result.
// @param the dividend (Integer to be divided)
// @param the divisor (Integer which is dividing the dividend)
// @return the (Integer) quotient of lhs divided by rhs
Integer operator/(Integer lhs, const Integer& rhs);

// Divide two integers and return the remainder/modulus of the result.
// @param the dividend (Integer to be divided)
// @param the divisor (Integer which is dividing the dividend)
// @return the (Integer) modulus/remainder of the division
Integer operator%(Integer lhs, const Integer& rhs);

// Check if two integers have the same value.
// @param the left Integer
// @param the right Integer
// @return a bool which is true if and only if lhs and rhs store the same value
bool operator==(const Integer& lhs, const Integer& rhs);

// Checks if lhs is less than or equal to rhs for unsigned integers. Note that
// this checks if |lhs| <= |rhs|
bool less_than_eq(const Integer& lhs, const Integer& rhs);

// Reverse the characters in a std::string
// @param str, a reference to the string to be reversed.
void reverse(std::string& str);


////////////////////////////////////////////////////////////////////////////////
// @class SignedInteger
// @brief An Integer which can be positive or negative.
//
// Longer description
////////////////////////////////////////////////////////////////////////////////

class SignedInteger : public Integer {
public:
	SignedInteger();
	SignedInteger(int val);
	SignedInteger(Integer& val);

	inline bool is_negative() const { return neg_; }
	inline void negate() { neg_ = !neg_; }

	virtual std::string binary_string() const;
	virtual std::string decimal_string() const;

	SignedInteger& operator+=(const SignedInteger& rhs);
	SignedInteger& operator-=(const SignedInteger& rhs);
	SignedInteger& operator*=(const SignedInteger& rhs);
	SignedInteger& operator/=(const SignedInteger& rhs);
	SignedInteger& operator%=(const SignedInteger& rhs);
	virtual SignedInteger& operator++();
	SignedInteger operator++(int unused);

private:

	// A bool indicating if the value stored in the SignedInteger is negative.
	// Specicially, neg_ is "true" if and only if the stored value is less than
	// zero.
	bool neg_;
};

SignedInteger operator+(SignedInteger lhs, const SignedInteger& rhs);
SignedInteger operator-(SignedInteger lhs, const SignedInteger& rhs);
SignedInteger operator*(SignedInteger lhs, const SignedInteger& rhs);
SignedInteger operator/(SignedInteger lhs, const SignedInteger& rhs);
SignedInteger operator%(SignedInteger lhs, const SignedInteger& rhs);

unsigned int absolute_value(int n);

// the output operator for Integer
// write Integer val to stream
std::ostream& operator << (std::ostream& os, const Integer& val);

// the input operator for Integer
// read Integer val to stream
std::istream& operator >> (std::istream& is, Integer& val);

// the output operator for SignedInteger
// write SignedInteger val to stream
std::ostream& operator << (std::ostream& os, const SignedInteger& val);

// the input operator for SignedInteger
// write SignedInteger val to stream
std::istream& operator >> (std::istream& is, SignedInteger& val);


#endif

