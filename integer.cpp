////////////////////////////////////////////////////////////////////////////////
// @file integer.cpp
// @author Jiahao Huo
// @date 2016-04-20
//
// Description: This is the implementation file for integer.h
//
// Known bugs: Division is sometimes off by 1.
////////////////////////////////////////////////////////////////////////////////

#include "integer.h"
#include <algorithm>
#include <cmath>
//#include <iostream>

Integer::Integer() {
    bits_.push_back(false);
}

Integer::Integer(int val) {

	// doesnt work
	if (val < 0) { val = -val; }


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

Integer::Integer(std::vector<bool> v){
    bits_ = v;
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

std::string Integer::decimal_string() const{
	int decimal = convert_to_decimal();
	std::string decimal_string = std::to_string(decimal);
	return decimal_string;
}

int Integer::convert_to_decimal() const{
	int decimal = 0;
	for (int i = 0; i < bits_.size(); ++i){
		if (bits_[i]){
			decimal += pow(2, i);
		}
	}
	return decimal;
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

// This new version of operator-= works as follows. Suppose we want to compute
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
    
	// if this and rhs have the same value, set this one to 0
	if ((*this) == rhs) {
		Integer zero(0);
		set_value(zero);

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
		//++(*this);
		Integer::operator++();

		remove_trailing_zeros();

	}
	else {

		// This code gets called if the difference is negative
		complement();

	}

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

Integer& Integer::operator/=(Integer rhs){

    std::vector<bool> quotient;
    std::vector<bool> tempD;
    Integer tempDiv; // temporary dividend
    
	// if rhs is 0, error
	int intrhs = rhs.convert_to_decimal();
	if (intrhs == 0){
		std::cout << "Division by 0 error!" << std::endl;
		bits_ = { 0 };
		return *this;
	}

	for (int iter = (int)bits_.size() - 1; iter >= 0; --iter) {
		// so tempDiv is equal to the first digit of the dividend
        tempD.push_back(bits_[iter]);
        std::reverse(tempD.begin(), tempD.end());
        tempDiv = Integer(tempD);
        std::reverse(tempD.begin(), tempD.end());
        
        if (tempDiv < rhs) {
            quotient.push_back(false);
        } else {
            quotient.push_back(true);
            tempDiv -= rhs;
			// delete what is in the tempD and store the value of tempDiv into tempD
            tempD.clear();
			for (int i = 0; i < tempDiv.size();++i) {
                tempD.push_back(tempDiv.get_bit(i));
            }
            std::reverse(tempD.begin(), tempD.end());
        }
    }
    std::reverse(quotient.begin(), quotient.end());
    set_value(quotient);
    remove_trailing_zeros();
    return *this;
}

Integer& Integer::operator%=(Integer rhs){
	Integer quotient, product, remainder;

	// if rhs is 0, error
	int intrhs = rhs.convert_to_decimal();
	if (intrhs == 0){
		std::cout << "Modulus by 0 error!" << std::endl;
		bits_ = { 0 };
		return *this;
	}

	if (*this < rhs){
		set_value(rhs);
	}
	else{
		quotient = *this / rhs;
		product = quotient * rhs;
		remainder = *this - product;
	}
	set_value(remainder);
	return *this;
}

Integer& Integer::operator++() {
	// Create an Integer "one" with value 1
	Integer one(1);

	// Add "one" to this instance.
	Integer::operator+=(one);

	return *this;
}

Integer Integer::operator++(int unused) {
    // Make a copy of this Integer
    Integer copy(*this);
    
    // Increment this version
    ++(*this);
    
    // Return the copy containing the previous version
    return copy;
}

bool Integer::borrow(int index) {
    int i = index + 1;
    int max_index = bits_.size() - 1;
    
    // Find the next value 1 ("true") in bits_ larger than index
    while (i <= max_index && !bits_[i]) {
        ++i;
    }
    
    // If on such value 1 exists, return false
    if (i > max_index)
        return false;
    
    // Otherwise set the value at i to false...
    bits_[i] = false;
    
    
    // ...and set all values of bits_[j] with j between index and i
    // to 1 ("true")
    --i;
    
    for (; i >= index; --i) {
        bits_[i] = true;
    }
    
    return true;
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


SignedInteger::SignedInteger() :Integer(){
	sign_ = { true };
}

// cannot handle negative number, how to fix?????????
// Integer(val) can only handle positive numbers
// minus sign needs to get rid of
SignedInteger::SignedInteger(int val):Integer(val){
	
	if (val >= 0) {
		sign_ = { true };
	}
	else{
		sign_ = { false };
	}
}

std::string SignedInteger::binary_string() const{
	std::string s;
	if (sign_[0] == true){
		s = "+";
	}
	else if (sign_[0] == false){
		s = "-";
	}
	s += Integer::binary_string();
	return s;
}

std::string SignedInteger::decimal_string() const{
	std::string s;
	if (sign_[0] == true){
		s = "+";
	}
	else if (sign_[0] == false){
		s = "-";
	}
	s += Integer::decimal_string();
	return s;
}

SignedInteger& SignedInteger::operator+=(const SignedInteger& rhs){

	if (sign_[0] == true && rhs.sign_[0] == true){
		Integer::operator+=(rhs);
		sign_ = { true };
	}
	else if (sign_[0] == false && rhs.sign_[0] == false){
		Integer::operator+=(rhs);
		sign_ = { false };
	}
	else if (sign_[0] == true && rhs.sign_[0] == false){
		Integer::operator-=(rhs);
		if (*this < rhs){
			sign_ = { false };
		}
		else if (rhs < *this){
			sign_ = { true };
		}
		else if (rhs == *this){
			sign_ = { true };
			*this = 0;
			return *this;
		}
	}
	else if (sign_[0] == false && rhs.sign_[0] == true){
		if (*this < rhs){
			sign_ = { true };
		}
		else if (rhs < *this){
			sign_ = { false };
		}
		else if (rhs == *this){
			sign_ = { true };
			*this = 0;
			return *this;
		}
		Integer::operator-=(rhs);
	}
	return *this;
}

SignedInteger& SignedInteger::operator-=(const SignedInteger& rhs){

	if (sign_[0] == true && rhs.sign_[0] == true){
		if (*this < rhs){
			sign_ = { false };
		}
		else if (rhs < *this ){
			sign_ = { true };
		}
		else if (rhs == *this){
			sign_ = { true };
			*this = 0;
			return *this;
		}
		Integer::operator-=(rhs);
	}
	else if (sign_[0] == false && rhs.sign_[0] == false){
		if (*this < rhs){
			sign_ = { true };
		}
		else if (rhs < *this){
			sign_ = { false };
		}
		else if (rhs == *this){
			sign_ = { true };
			*this = 0;
			return *this;
		}
		Integer::operator-=(rhs);
	}
	else if (sign_[0] == true && rhs.sign_[0] == false){
		Integer::operator+=(rhs);
		sign_ = { true };
	}
	else{
		Integer::operator+=(rhs);
		sign_ = { false };
	}
	return *this;
}

SignedInteger& SignedInteger::operator*=(SignedInteger rhs){
	if ((sign_[0] == true && rhs.sign_[0] == true) || sign_[0] == false && rhs.sign_[0] == false){
		Integer::operator*=(rhs);
		sign_ = { true };
	}
	else if ((sign_[0] == true && rhs.sign_[0] == false) || (sign_[0] == false && rhs.sign_[0] == true)){
		Integer::operator*=(rhs);
		sign_ = { false };
	}
	return *this;
}

SignedInteger& SignedInteger::operator/=(SignedInteger rhs){
	if ((sign_[0] == true && rhs.sign_[0] == true) || sign_[0] == false && rhs.sign_[0] == false){
		Integer::operator/=(rhs);
		sign_ = { true };
	}
	else if ((sign_[0] == true && rhs.sign_[0] == false) || (sign_[0] == false && rhs.sign_[0] == true)){
		Integer::operator/=(rhs);
		sign_ = { false };
	}
	return *this;
}

SignedInteger& SignedInteger::operator%=(SignedInteger rhs){
	if (sign_[0] == true && rhs.sign_[0] == true){
		Integer::operator%=(rhs);
		sign_ = { true };
	}
	else if ((sign_[0] == true && rhs.sign_[0] == false) ||
		(sign_[0] == false && rhs.sign_[0] == true) ||
		(sign_[0] == false && rhs.sign_[0] == false)){
		Integer::operator%=(rhs);
		sign_ = { false };
	}
	return *this;
}

SignedInteger& SignedInteger::operator++(){

	SignedInteger one(1);
	SignedInteger::operator += (one);
	return *this;
}

SignedInteger& SignedInteger::operator--(){

	SignedInteger one(1);
	SignedInteger::operator -= (one);
	return *this;
}

std::vector<bool> remove_zeros(std::vector<bool> trim) {
    int i = (int) trim.size() - 1;
    while ((i > 0) && !trim[i]) {
        trim.pop_back();
        --i;
    }
    
    // If bits_ is empty, push a 0 ("false") onto bits_
    if (trim.size() == 0) {
        trim.push_back(false);
    }
    return trim;
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

bool operator < (const Integer& lhs, const Integer& rhs){
    
    std::vector<bool> templhs, temprhs;

	Integer tlhs, trhs;
    
	//copy the value of lhs into templhs
    for (int i=0; i < lhs.size(); i++) {
        templhs.push_back(lhs.get_bit(i));
    }
    templhs = remove_zeros(templhs); // remove redundant zeros
    tlhs= Integer(templhs);
    
	// copy the value of rhs into temrhs
    for (int i=0; i < rhs.size(); i++) {
        temprhs.push_back(rhs.get_bit(i));
    }
    temprhs = remove_zeros(temprhs); // remove redundant zeros
    trhs = Integer(temprhs);
    
    if (tlhs.size() < trhs.size()) { return true; }
    else if (tlhs.size() > trhs.size()){ return false; }
    else{
        for (int i = tlhs.size() - 1; i >= 0; --i) {
            if (tlhs.get_bit(i) < trhs.get_bit(i)) { return true; }
        }
    }
    return false;
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


