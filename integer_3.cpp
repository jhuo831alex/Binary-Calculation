////////////////////////////////////////////////////////////////////////////////
// @file integer.cpp
// @author Will	
// @date 2016-03-31
//
// Description: This is the implementation file for integer.h
//
// Known bugs: Several bugs in output operator for SignedInteger
//             please refer to the comment for the operator
////////////////////////////////////////////////////////////////////////////////

#include "integer.h"
#include <iostream>

Integer::Integer() {
	bits_.push_back(false);
}

Integer::Integer(unsigned int val) {

	// If val is 0, bits_ should be {false}
	if (val == 0) {
		bits_.push_back(false);
		return;
	}

	// Read the bits of val by repeatedly dividing by 2 and checking the
	// remainder % 2. Push bits to bits_.
	while (val != 0) {
		bits_.push_back(val % 2 == 1);
		val /= 2;
	}
}

std::string Integer::binary_string() const {

	std::string str_bits = ""; // the bits stored as a string

	// str_bits starts with (
	str_bits = '(';


	char bit; // a single bit to append to str_bits

	// append the bits of bits_ to str_bits... notice that the
	// bits_[0] is the least signifcant bit, so it should be
	// printed last, bits_[1] should be second-to-last, etc.
	for (int i = bits_.size() - 1; i >= 0; --i) {
		bit = bits_[i] ? '1' : '0';
		str_bits += bit;
	}

	// str_bits ends with )_2
	str_bits += ")_2";

	return str_bits;
}

std::string Integer::decimal_string() const {

	if (is_zero()) {
		return "0";
	}

	Integer copy = *this;
	Integer ten(10);

	std::string str = "";
	Integer cur_digit;

	while (!copy.is_zero()) {
		cur_digit = copy % ten;
		str += ('0' + cur_digit.get_int_value());
		copy /= 10;
	}

	reverse(str);

	return str;
}

bool Integer::get_bit(unsigned int index) const {
	if (index < bits_.size())
		return bits_[index];

	return false;
}

void Integer::set_bit(unsigned int index, bool value) {

	if (index < bits_.size()) {

		bits_[index] = value;

	}
	else {

		bits_.push_back(value);

	}
}

void Integer::left_shift() {

	// push "false" to the first position of bits_
	bits_.insert(bits_.begin(), false);

}

void Integer::right_shift() {
	bits_.erase(bits_.begin());
}

bool Integer::is_zero() const {
	return ((bits_.size() == 1) && !bits_[0]);
}

unsigned int Integer::get_int_value() const {
	unsigned int i_value = 0;
	unsigned int bit_value = 1;
	int size = bits_.size();

	for (int i = 0; i < size; ++i) {
		if (bits_[i]) {
			i_value += bit_value;
		}

		bit_value *= 2;
	}

	return i_value;
}

// "set_value" could be made more efficient by
// updating values of bits_ rather than always deleting values,
// then pushing new values.
void Integer::set_value(const Integer& val) {
	// the size of this instance
	int max_index = size() - 1;

	// remove everything from bits_
	for (int i = 0; i <= max_index; ++i) {
		bits_.pop_back();
	}

	max_index = val.size() - 1;

	// add the bits from val to bits_
	for (int i = 0; i <= max_index; ++i) {
		bits_.push_back(val.get_bit(i));
	}
}

void Integer::complement() {
	int size = bits_.size();

	// Flip each bit of bits_
	for (int i = 0; i < size; ++i) {
		bits_[i] = !bits_[i];
	}

	// Make sure there aren't any trailing zeros.
	remove_trailing_zeros();
}

Integer& Integer::operator+=(const Integer& rhs) {
	int lhs_max_index = bits_.size() - 1;
	int rhs_max_index = rhs.size() - 1;

	// max_index is the larger of the maximum indices of this instance and rhs
	int max_index = (lhs_max_index > rhs_max_index) ? lhs_max_index : rhs_max_index;

	// the carry bit from adding bits
	bool carry = false;

	// the current bits from lhs and rhs, respectively
	bool cur_lhs_bit, cur_rhs_bit;


	// Loop through the bit values of this instance and rhs and add bits
	// in each position, along with the carry bit from adding at the previous
	// index. 
	for (int i = 0; i <= max_index; ++i) {
		cur_lhs_bit = get_bit(i);
		cur_rhs_bit = rhs.get_bit(i);

		add_with_carry(cur_lhs_bit, cur_rhs_bit, carry);

		set_bit(i, cur_lhs_bit);
	}

	// If there is a carry bit after adding all bits, append it to the end of bits_
	if (carry) {
		bits_.push_back(carry);
	}

	return *this;
}

// The operator-= works as follows. Suppose we want to compute
// lhs -= rhs. We will assume that lhs and rhs have the same number of bits, and call
// this number k. That is k = lhs.size() which is equal to rhs.size(). The *complement*
// of rhs is the Integer whose bits are the opposite of rhs. Observe that the value of
// rhs's complement, which we'll denote by comp(rhs), is 2^(k+1) - rhs - 1. Thus we have
//   lhs - rhs = lhs + comp(rhs) - 2^(k+1) + 1.
// Therefore, in order to subtract rhs from lhs, it suffices to *add* comp(rhs) to lhs
// and subtract 2^(k+1) and add 1. Subtracting 2^(k+1) is easy because its binary
// representation is just 1 followed by k 0's.
//
// We can use the observation above in order to implement operator-= using addition!
// The idea is to add lhs+=(comp(rhs) + 1). If the finaly carry bit is 1, then the result
// is at least 2^(k+1), so lhs - rhs is non-negative, and we simply throw away the carry
// bit to compute lhs - rhs = lhs + comp(rhs) + 1 - 2^(k+1). On the other hand, if the
// final carry bit is 0, then we must have had lhs - rhs < 0. In this case, taking the
// complement of lhs + comp(rhs) gives rhs - lhs. So the function actually assigns the
// value |lhs - rhs| to lhs!!
Integer& Integer::operator-=(const Integer& rhs) {

	//std::cout << "Subtracting: " << binary_string() << " - " << rhs.binary_string() << " = ";

	// if this and rhs have the same value, set this one to 0
	if ((*this) == rhs) {
		Integer zero(0);
		set_value(zero);

		//std::cout << binary_string();

		return *this;
	}

	int lhs_max_index = bits_.size() - 1;
	int rhs_max_index = rhs.size() - 1;

	// max_index is the larger of the maximum indices of this instance and rhs
	int max_index = (lhs_max_index > rhs_max_index) ? lhs_max_index : rhs_max_index;

	// the carry bit from adding bits
	bool carry = false;

	// the current bits from lhs and rhs, respectively
	bool cur_lhs_bit, cur_rhs_bit;


	// Loop through the bit values of this instance and rhs and add bits
	// in each position, along with the carry bit from adding at the previous
	// index. 
	for (int i = 0; i <= max_index; ++i) {
		cur_lhs_bit = get_bit(i);
		cur_rhs_bit = rhs.get_bit(i);

		// Note that we add the complement to cur_rhs_bit!
		add_with_carry(cur_lhs_bit, !cur_rhs_bit, carry);

		set_bit(i, cur_lhs_bit);
	}

	// If there is a carry bit after adding all bits, ignore it, and increment 
	if (carry) {

		// This code gets called if the difference is positive
		// Note that we need to implement the Integer version of ++, not
		// the SignedInteger version here!
		Integer::operator++();

		remove_trailing_zeros();

	}
	else {

		// This code gets called if the difference is negative
		complement();

	}

	//std::cout << binary_string() << std::endl;

	return *this;
}

Integer& Integer::operator*=(Integer rhs) {

	// If rhs is 0, set the value to 0 (= rhs)
	if (rhs.size() == 1 && rhs.get_bit(0) == false) {
		set_value(rhs);
	}


	Integer new_value(0); // the new value (after multiplication)

	int max_index = bits_.size() - 1;

	// Loop through each bit of this instance and add the corresponding
	// "shifts" of rhs to new_value. For example, if we see a 1 in the k-th
	// bit of this instance, add the k-th shift of rhs to new_value.
	for (int i = 0; i <= max_index; ++i) {

		if (bits_[i]) {
			new_value += rhs;
		}

		rhs.left_shift();
	}

	// Assign this instance the value of new_value.
	set_value(new_value);

	return *this;
}

Integer& Integer::operator/=(Integer rhs) {
	int rhs_size = rhs.size();
	int lhs_size = size();

	// Check for division by 0!
	if ((rhs_size == 1) && (!rhs.get_bit(0))) {
		std::cout << "Division by 0 error!" << std::endl;
		return *this;
	}

	for (int i = rhs_size; i < lhs_size; ++i) {
		rhs.left_shift();
	}

	Integer quotient(0);

	for (int i = lhs_size; i >= rhs_size; --i) {
		if (less_than_eq(rhs, *this)) {
			quotient.set_bit(0, true);
			(*this) -= rhs;
		}
		quotient.left_shift();
		rhs.right_shift();
	}

	quotient.right_shift();

	set_value(quotient);

	remove_trailing_zeros();

	return *this;
}

Integer& Integer::operator%=(Integer rhs) {
	int rhs_size = rhs.size();
	int lhs_size = size();

	// Check for modulus by 0!
	if ((rhs_size == 1) && (!rhs.get_bit(0))) {
		std::cout << "Modulus by 0 error!" << std::endl;
		return *this;
	}

	for (int i = rhs_size; i < lhs_size; ++i) {
		rhs.left_shift();
	}

	for (int i = lhs_size; i >= rhs_size; --i) {
		if (less_than_eq(rhs, *this)) {
			(*this) -= rhs;
		}

		rhs.right_shift();
	}

	return *this;
}

Integer& Integer::operator++() {
	// Create an Integer "one" with value 1
	Integer one(1);

	// Add "one" to this instance.
	operator+=(one);

	return *this;
}

Integer Integer::operator++(int unused) {
	Integer a(*this);
	++*this;
	return a;
}

void Integer::remove_trailing_zeros() {

	int i = bits_.size() - 1;

	// Starting from the largest index in bits_, pop_back values until
	// we see the first 1 ("true")
	while ((i > 0) && !bits_[i]) {
		bits_.pop_back();
		--i;
	}

	// If bits_ is empty, push a 0 ("false") onto bits_
	if (bits_.size() == 0) {
		bits_.push_back(false);
	}
}

void add_with_carry(bool& b1, bool b2, bool& carry) {

	if (b1 && b2 && carry) {

		// If all bits are 1, then new bit and carry
		// bit are 1 so we don't need to do anything
		return;

	}
	else if ((b1 && b2) || (b1 && carry) || (b2 && carry)) {

		// If two bits are 1, then carry bit is 1
		// and new bit is 0
		b1 = false;
		carry = true;

		return;

	}
	else if (b1 || b2 || carry) {
		// If one of the three bits is 1 then new bit is 1
		// and carry bit is 0
		b1 = true;
		carry = false;
		return;
	}

	// If all 3 are false, new bit and carry bit stay 0
}

Integer operator+(Integer lhs, const Integer& rhs) {
	lhs += rhs;
	return lhs;
}

Integer operator-(Integer lhs, const Integer& rhs) {
	lhs -= rhs;
	return lhs;
}

Integer operator*(Integer lhs, const Integer& rhs) {
	lhs *= rhs;
	return lhs;
}

Integer operator/(Integer lhs, const Integer& rhs) {
	lhs /= rhs;
	return lhs;
}

Integer operator%(Integer lhs, const Integer& rhs) {
	lhs %= rhs;
	return lhs;
}

bool operator==(const Integer& lhs, const Integer& rhs) {
	int size = lhs.size();

	// If lhs and rhs have different sizes, they are not equal
	if (size != rhs.size()) {
		return false;
	}

	// Check that the bits of lhs and rhs are the same. If they ever differ,
	// return false.
	for (int i = 0; i < size; ++i) {
		if (lhs.get_bit(i) != rhs.get_bit(i)) {
			return false;
		}
	}

	// All of the bits of lhs and rhs are the same, so they store the same value.
	return true;
}

bool less_than_eq(const Integer& lhs, const Integer& rhs) {
	int size = lhs.size();
	int rhs_size = rhs.size();

	if (size < rhs_size) {

		return true;

	}
	else if (size > rhs_size) {

		return false;
	}

	for (int i = size - 1; i >= 0; --i) {

		if (lhs.get_bit(i) && !rhs.get_bit(i)) {

			return false;

		}
		else if (!lhs.get_bit(i) && rhs.get_bit(i)) {

			return true;

		}
	}

	// If we get here, lhs == rhs!
	return true;
}

void reverse(std::string& str) {
	int i = 0;
	int j = str.size() - 1;

	char ch;

	while (i < j) {
		ch = str[i];
		str[i] = str[j];
		str[j] = ch;

		++i;
		--j;
	}
}



SignedInteger::SignedInteger() : Integer() {
	neg_ = false;
}

SignedInteger::SignedInteger(int val) : Integer(absolute_value(val)) {
	neg_ = (val < 0);
}

SignedInteger::SignedInteger(Integer& val) : Integer(val) {
	neg_ = false;
}

std::string SignedInteger::binary_string() const {
	std::string str = neg_ ? "-" : "+";
	str += Integer::binary_string();

	return str;
}

std::string SignedInteger::decimal_string() const {
	std::string str = neg_ ? "-" : "+";
	str += Integer::decimal_string();

	return str;
}

SignedInteger& SignedInteger::operator+=(const SignedInteger& rhs) {

	// If this and rhs have the same sign, just do Integer addition
	if (neg_ == rhs.is_negative()) {
		Integer::operator+=(rhs);
		return *this;
	}

	// Otherwise, if they have opposite signs, neg_ will change sign
	// if rhs is larger than this SignedInteger
	if (less_than_eq(*this, rhs)) {
		negate();
	}

	Integer::operator-=(rhs);

	// 0 is not negative!
	if (is_zero()) {
		neg_ = false;
	}

	return *this;
}

SignedInteger& SignedInteger::operator-=(const SignedInteger& rhs) {

	// Subtraction is just adding the opposite!
	SignedInteger negative_rhs = rhs;
	negative_rhs.negate();

	operator+=(negative_rhs);


	return *this;
}

SignedInteger& SignedInteger::operator*=(const SignedInteger& rhs) {

	Integer::operator*=(rhs);

	if (rhs.is_negative()) {
		neg_ = !neg_;
	}

	return *this;
}

SignedInteger& SignedInteger::operator/=(const SignedInteger& rhs) {
	Integer::operator/=(rhs);

	if (rhs.is_negative()) {
		neg_ = !neg_;
	}

	return *this;
}

SignedInteger& SignedInteger::operator%=(const SignedInteger& rhs) {
	Integer::operator%=(rhs);
	return *this;
}

SignedInteger& SignedInteger::operator++() {
	SignedInteger one(1);
	return operator+=(one);
}

SignedInteger SignedInteger::operator++(int unused) {
	SignedInteger a(*this);
	++*this;
	return a;
}

SignedInteger operator+(SignedInteger lhs, const SignedInteger& rhs) {
	lhs += rhs;
	return lhs;
}

SignedInteger operator-(SignedInteger lhs, const SignedInteger& rhs) {
	lhs -= rhs;
	return lhs;
}

SignedInteger operator*(SignedInteger lhs, const SignedInteger& rhs) {
	lhs *= rhs;
	return lhs;
}

SignedInteger operator/(SignedInteger lhs, const SignedInteger& rhs) {
	lhs /= rhs;
	return lhs;
}

SignedInteger operator%(SignedInteger lhs, const SignedInteger& rhs) {
	lhs %= rhs;
	return lhs;
}

unsigned int absolute_value(int n) {
	if (n >= 0) {
		return n;
	}

	return -n;
}

std::ostream& operator << (std::ostream& os, const Integer& val){
	os << val.Integer::decimal_string();
	return os;
}

std::ostream& operator << (std::ostream& os, const SignedInteger& val){
	os << val.SignedInteger::decimal_string();
	return os;
}

std::istream& operator >> (std::istream& is, Integer& val){

	val = Integer(0);
	Integer ten(10);
	char ch;
	is >> ch;
	while (ch >= '0' && ch <= '9'){
		Integer digit(ch - '0');
		val *= ten; // if there is another digit after the first digit, times the first digit by 10
		val += digit; // and plus the second digit
		is.get();
		ch = is.peek();
	}

	return is;
}

std::istream& operator >> (std::istream& is, SignedInteger& val){
	
	val = SignedInteger(0);
	SignedInteger ten(10);
	char ch;
	bool sign = true; // the default sign for SignedInteger is positive

	is >> ch; // ignore the whitespaces before signs
	if (ch == '+'){
		sign = true; // if the sign is +, then the SignedInteger is positive
	}
	else if (ch == '-'){
		sign = false; // if the sign is -, then the SignedInteger is negative
	}

	// if there is no sign before the number, then this SignedInteger must be positive
	// unget this digit 
	else if (ch >= '0' && ch <= '9'){
		sign = true;
		is.unget();
	}

	is >> ch; // ignore whitespaces before digits

	// read in digits as chars and stores them into the SignedInteger val
	while (ch >= '0' && ch <= '9'){
		SignedInteger digit(ch - '0');
		val *= ten;
		val += digit;
		ch = is.peek();
		if (ch != EOF){ 
			is.get(); // if ch has not reached EOF, keep reading in digits
		}
	}

	// if the sign is negative, then the value of SignedInteger is multiplied by -1
	if (!sign){
		val *= SignedInteger(-1);
	}

	return is;
}


