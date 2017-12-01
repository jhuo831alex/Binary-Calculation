////////////////////////////////////////////////////////////////////////////////
// @file integer.h
// @author Jiahao Huo 
// @date 2016-04-20
//
// Description: This is the header file for the Integer class and its derived
// class SignedInteger. 
// The Integer class performs basic integer arithmetic. Unlike the fundamental 
// datatype int, an Integer dynamically rescales its size to accomodate much larger
// values than ints.
// Currently, an Integer only supports non-negative numbers, and the operators
// +, *, -, and their corresponding assignment operators (+=, etc.).
//
// The SignedInteger class performs basic integer arithmetic just like Integer 
// class, but it supports both negative and positive numbers. 
//
// Known bugs: Division is sometimes off by 1.
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
    Integer(int val);
    
	// Constructor assigns a bool vecter to bits_
    Integer(std::vector<bool> v);
    
    // Produce a string which contains the bits of
    // the Integer (stored in bits_)
    // @return an std::string in the form (...)_2 where "..." denotes the bits
    // of the Integer
    virtual std::string binary_string() const;

	// Produce a string which represents of the decimal form of this Integer
	// @return the string representation of the decimal value of this Integer
	virtual std::string decimal_string() const;

	// Convert a binary to a decimal
	// @return the decimal value of this Integer
	int convert_to_decimal() const;
    
    // Return the number of bits stored in bits_.
    // @return an int containing the number of its in bits_.
    inline int size() const { return bits_.size(); }

	// return the boolean vector of an Integer
	std::vector<bool> boolvector() const { return bits_; }
    
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
    
	// Divide this Integer by the Integer on the right hand side.
	// @ param rhs is a pointer to the Integer to divide
	// @ return a reference to this integer after division
    Integer& operator/= (Integer rhs);

	// Mod this Integer by the Integer on the right hand side
	// @ param rhs is a pointer to the Integer to mod
	// @ return a reference to this integer after mod
	Integer& operator%= (Integer rhs);
    
    // Increment this integer by one (prefix)
    // @return a reference to this integer (after incrementation).
    virtual Integer& operator++();
    
    // Increment this integer by one (postfix)
    // @return a reference which stores the value before incrementation.
    virtual Integer operator++(int unused);
    
private:
    // The bits that store the value of an Integer. Bits are stored
    // from least to most significant.
    std::vector<bool> bits_;
    
    // The "borrow" subroutine used in subtraction. Finds the next position
    // to the "left" of index in bits_ which has a value of 1, decrements this
    // bit, and increments all bits between that position and index. The function
    // returns a bool which tells if the borrowing operation was successful.
    // Note: this function is obsolete because of a new (better) implementation of
    // operator-= and operator-.
    // @param index is the index at which to borrow
    // @retun "true" if the borrow was successful (i.e., there is a 1 to the left
    // of index) and "false" if the borrow was unsuccessful.
    bool borrow(int index);
    
    // Remove the trailing zeros from an Integer. For example,
    // (00010110)_2 should be written as (10110)_2.
    void remove_trailing_zeros();
};

////////////////////////////////////////////////////////////////////////////////
// @class SignedInteger
// @brief Derived class for Integer. Supports basic arithmetic with integers with signs.
//
// This SignedInteger class supports basic arithemetic: addition, subtraction, and
// multiplication. It can operate on both positive and negative Integers. 
////////////////////////////////////////////////////////////////////////////////

class SignedInteger : public Integer{
public:

	// Default Constructor, default value is +0
	SignedInteger();

	// Construct an Integer with + or - sign
	SignedInteger(int val);

	// Produce a string which contains the bits of
	// the SignedInteger (stored in bits_) with + or - sign
	// @return an std::string in the form + or - (...)_2 where "..." denotes the bits
	// of the SignedInteger
	virtual std::string binary_string() const;

	// Produce a string which represents of the decimal form of this SignedInteger and its sign (+ or -)
	// @return the string representation of the decimal value of this SignedInteger and its sign (+ or -)
	virtual std::string decimal_string() const;
	
	// Adds the SignedInteger on the right side to the current SignedInteger.
	// @param rhs is a pointer to the SignedInteger to be added
	// @return a reference to this SignedInteger after addition
	SignedInteger& operator += (const SignedInteger& rhs);

	// Subtracts the SignedInteger on the right side from the current SignedInteger. 
	// @param rhs is a pointer to the SignedInteger to be subtracted
	// @return a reference to this SignedInteger after subtraction
	SignedInteger& operator -= (const SignedInteger& rhs);

	// Multiplies this SignedInteger by the SignedInteger on the right hand side.
	// @param rhs is a pointer to the SignedInteger to multiply
	// @return a reference to this SignedInteger after multiplication
	SignedInteger& operator *= (SignedInteger rhs);

	// Divide this SignedInteger by the SignedInteger on the right hand side.
	// @ param rhs is a pointer to the SignedInteger to divide
	// @ return a reference to this SignedInteger after division
	SignedInteger& operator /= (SignedInteger rhs);

	// Mod this SignedInteger by the SignedInteger on the right hand side
	// @ param rhs is a pointer to the SignedInteger to mod
	// @ return a reference to this SignedInteger after mod
	SignedInteger& operator %= (SignedInteger rhs);

	// Increment this SignedInteger by one (prefix)
	// @return a reference to this SignedInteger (after incrementation).
	virtual SignedInteger& operator ++();

	// Decrement this SignedInteger by one (prefix)
	// @return a reference to this SignedInteger (after decrementation).
	virtual SignedInteger& operator --();


private:
	// store the sign of a SignedInteger
	// True for positive, False for negative
	std::vector<bool> sign_;
};

// remove zeros from a bool vector
// @param a boolean vector with redundant zeros ex. 000
// @return a boolean vector without redundant zeros ex. 0
std::vector<bool> remove_zeros(std::vector<bool> trim);

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

// Divide lhs by rhs and return the value of the result.
// @param the left Integer dividend
// @param the right Integer divisor (a reference)
// @return the quotient 
Integer operator/(Integer lhs, const Integer& rhs);

// Mod lhs by rhs and return the value of the result.
// @param the left Integer dividend
// @param the right Integer divisor (a reference)
// @return the remainder
Integer operator%(Integer lhs, const Integer& rhs);

// Check if two integers have the same value.
// @param the left Integer
// @param the right Integer
// @return a bool which is true if and only if lhs and rhs store the same value
bool operator==(const Integer& lhs, const Integer& rhs);

// Check if lhs is smaller than rhs
// @param the left Integer
// @param the right Integer
// @return a bool which is true if and only if lhs is smaller than rhs
bool operator < (const Integer& lhs,  const Integer& rhs);

// Add two integers and return the value of the result.
// @param the left Integer to be added
// @param the right Integer to be added (a reference)
// @return the sum of lhs and rhs.
SignedInteger operator+(SignedInteger lhs, const SignedInteger& rhs);

// Subtract one Integer from another and return the value of the result.
// This function assumes that result is non-negative. If the result is negative,
// the output of the function is unspecified.
// @param the left Integer
// @param the right Integer to be subtracted (a reference)
// @return the difference of lhs and rhs
SignedInteger operator-(SignedInteger lhs, const SignedInteger& rhs);

// Multiply two integers and return the value of the result.
// @param the left Integer to be multiplied
// @param the right Integer to be multiplied (a reference)
// @return the product of lhs and rhs.
SignedInteger operator*(SignedInteger lhs, const SignedInteger& rhs);

// Divide lhs by rhs and return the value of the result.
// @param the left Integer dividend
// @param the right Integer divisor (a reference)
// @return the quotient 
SignedInteger operator/(SignedInteger lhs, const SignedInteger& rhs);

// Mod lhs by rhs and return the value of the result.
// @param the left Integer dividend
// @param the right Integer divisor (a reference)
// @return the remainder
SignedInteger operator%(SignedInteger lhs, const SignedInteger& rhs);


#endif

