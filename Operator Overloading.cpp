#include <iostream>    
#include <iomanip>      
#include <string>
#include <math.h>
#include <vector>


class Integer
{
public:
	Integer(int a);
	void print_as_int();
	void print_as_bits();
	Integer& operator+=(const Integer& rhs);
	Integer& operator*=(const Integer& rhs);
	void operator&=(const Integer& rhs);
	void operator|=(const Integer& rhs);
	Integer& operator++();                 // prefix ++
	Integer operator++(int unused);   // postfix ++
	bool operator<(const Integer& rhs) const;
	bool operator==(const Integer& rhs) const;
	Integer& operator<<=(unsigned int a);
	Integer twos_complement(Integer temp);
	Integer twos_complement2 (const Integer& temp) const;
	void twos_complement3();
private:
	std::vector<bool> bit;
	bool sign = 0;
	void clean_up();
};
Integer operator+(Integer a, const Integer& b);
Integer operator*(Integer a, const Integer& b);
Integer operator&(Integer a, const Integer& b);
Integer operator|(Integer a, const Integer& b);
bool operator!= (const Integer& lhs, const Integer& rhs);
bool operator>  (const Integer& lhs, const Integer& rhs);
bool operator<=(const Integer& lhs, const Integer& rhs);
bool operator>=(const Integer& lhs, const Integer& rhs);


int main()
{
	Integer a = -30;
	Integer b = -100;
	std::cout << "The base-2 representation of a is: "; a.print_as_bits(); std::cout << std::endl;
	std::cout << "The decimal representation of a is: "; a.print_as_int(); std::cout << std::endl;
	std::cout << "The decimal representation of b is: "; b.print_as_int(); std::cout << std::endl;
	std::cout << "The base-2 representation of b is: "; b.print_as_bits(); std::cout << std::endl;
	Integer c = a + b;
	std::cout << "The base-2 representation of c is: "; c.print_as_bits(); std::cout << std::endl;
	std::cout << "The decimal representation of c is: "; c.print_as_int(); std::cout << std::endl;
	Integer d = a*b;
	std::cout << "The decimal representation of d is: "; d.print_as_int(); std::cout << std::endl;
	std::cout << "The base-2 representation of d is: "; d.print_as_bits(); std::cout << std::endl;
	if (a < b)	std::cout << "a<b" << std::endl;
	if (a <= b)	std::cout << "a<=b" << std::endl;
	if (a == b)	std::cout << "a==b" << std::endl;
	if (a != b)	std::cout << "a!=b" << std::endl;
	if (a >= b)	std::cout << "a>=b" << std::endl;
	if (a > b)	std::cout << "a>b" << std::endl;
	Integer the_and = a&b; the_and.print_as_bits(); std::cout << std::endl;
	Integer the_or = a|b; the_or.print_as_bits(); std::cout << std::endl;
	++a; a.print_as_bits(); std::cout << std::endl;
	a++; a.print_as_bits(); std::cout << std::endl;
	Integer shiftupequals = a; shiftupequals <<= 2;
	shiftupequals.print_as_bits(); std::cout << std::endl;
	return 0;
}

Integer::Integer(int a) {
	if (a == 0) bit.push_back(0);
	if (a < 0) sign = 1;
	a = abs(a);
	while (a > 0) {
		bit.push_back(a % 2);
		a /= 2;
	}

}

Integer& Integer::operator+=(const Integer& rhs) {
	bool carry = 0;
	Integer temp = rhs;
	unsigned int m = 0;
	if (bit.size() > temp.bit.size())	temp.bit.resize(bit.size());
	if (bit.size() < temp.bit.size())	bit.resize(temp.bit.size());
	if (sign != temp.sign) {												//one positive, one negative
		//if (sign == 1)		*this = twos_complement(*this);					//converting to 2s complement
		//if (temp.sign == 1)		temp = twos_complement(temp);				//can't pass address of temp!!! local variable}
		if (sign == 1)		twos_complement3();				
		if (temp.sign == 1)		temp.twos_complement3();
	}
	//temp.print_as_bits();
	//print_as_bits();
	for (size_t i = 0, n = bit.size(); i < n; i++) { //adding two vectors
		m = bit[i] + temp.bit[i] + carry;
		if (m == 0) {
			bit[i] = 0; carry = 0;
		}
		if (m == 1) {
			bit[i] = 1; carry = 0;
		}
		if (m == 2) {
			bit[i] = 0; carry = 1;
		}
		if (m == 3) {
			bit[i] = 1; carry = 1;
		}
	}
	if (sign == temp.sign) {
		if (carry == 1)  bit.push_back(1);  //highest digit at the end of vector
	}
	else {							//if sign != temp.sign
		if (carry == 1)	sign = 0;	//positive result
		else {						//negative result, converting back
			//*this = twos_complement(*this);
			twos_complement3();
			sign = 1;
		}
	}
	return *this;
}
Integer& Integer::operator*=(const Integer& rhs) {
	bool temp_sign = sign ^ rhs.sign;	//sign of the result
	sign = 0;							//make sign positive for +=
	Integer temp = 0;
	temp.bit = bit;    // creating a new object to store the temp number to be added to bit
	std::vector<bool> rhss = rhs.bit;
	std::fill(bit.begin(), bit.end(), false);  //empty bit to vector of 0
	for (size_t i = 0, n = rhss.size(); i < n; i++) {
		if (rhss[i] == 1)  *this += temp;
		temp.bit.insert(temp.bit.begin(), 0);   //left-shift by 1, double
	}
	sign = temp_sign;   //not temp.sign
	return *this;
}
Integer Integer::twos_complement(Integer temp) {
	for (size_t i = 0, n = temp.bit.size(); i < n; i++) {	//flipping all digits
		temp.bit[i] = !temp.bit[i];
	}
	for (size_t i = 0, n = temp.bit.size(); i<n; ++i) {		//adding 1
		if (temp.bit[i])	
			temp.bit[i] = 0;
		else{
			temp.bit[i] = 1;
			break;
		}
	}
	return temp;
}
Integer Integer::twos_complement2(const Integer& rhs) const {
	Integer temp = rhs;
	for (size_t i = 0, n = temp.bit.size(); i < n; i++) {	//flipping all digits
		temp.bit[i] = !temp.bit[i];
	}
	for (size_t i = 0, n = temp.bit.size(); i<n; ++i) {		//adding 1
		if (temp.bit[i])
			temp.bit[i] = 0;
		else {
			temp.bit[i] = 1;
			break;
		}
	}
	return temp;
}
void Integer::twos_complement3() {
	for (size_t i = 0, n = bit.size(); i < n; ++i) {	//flipping all digits
		bit[i] = !bit[i];
	}
	for (size_t i = 0, n = bit.size(); i < n; ++i) {		//adding 1
		if (bit[i])
			bit[i] = 0;
		else{
			bit[i] = 1;
			break;
		}
	}
}
void Integer::operator&=(const Integer& rhs) {
	std::vector<bool> temp = rhs.bit;  //copy vector bit
	if (bit.size() > temp.size())	temp.resize(bit.size());  //swap?
	if (bit.size() < temp.size())	bit.resize(temp.size());
	for (size_t i = 0, n = bit.size(); i < n; i++) {
		bit[i] = bit[i] & temp[i];
	}
	clean_up();
}
void Integer::operator|=(const Integer& rhs) {
	std::vector<bool> temp = rhs.bit;  //copy vector bit
	if (bit.size() > temp.size())	temp.resize(bit.size());
	if (bit.size() < temp.size())	bit.resize(temp.size());
	for (size_t i = 0, n = bit.size(); i < n; i++) {
		bit[i] = bit[i] | temp[i];
	}
}
Integer& Integer::operator++() {
	Integer temp = 1;
	return *this += temp;
}
Integer Integer::operator++(int unused) {
	Integer temp = 1;
	return unused + temp;
}
bool Integer::operator<(const Integer& rhs) const {
	if (sign < rhs.sign)	return 0;
	else if (sign > rhs.sign)	return 1;
	else if (sign==0){                                //both positive
		if (bit.size() < rhs.bit.size())	return 1;
		if (bit.size() > rhs.bit.size())	return 0;
		for (int i = bit.size() - 1; i >= 0; i--) {
			if (bit[i] < rhs.bit[i])	return 1;
			if (bit[i] > rhs.bit[i])	return 0;
		}
		return 0;
	}
	else{											//both negative
		if (bit.size() < rhs.bit.size())	return 0;
		if (bit.size() > rhs.bit.size())	return 1;
		for (int i = bit.size() - 1; i >= 0; i--) {
			if (bit[i] < rhs.bit[i])	return 0;
			if (bit[i] > rhs.bit[i])	return 1;
		}
		return 0;
	}
}
bool Integer::operator==(const Integer& rhs) const {
	if (bit == rhs.bit && sign == rhs.sign) return 1;
	return 0;
}
Integer& Integer::operator<<=(unsigned int a) {
	while (a > 0) {
		bit.insert(bit.begin(), 0);
		a--;
	}
	return *this;
}
void Integer::print_as_bits() {
	//clean_up();
	if (sign == 0)	std::cout << " (";
	if (sign == 1)	std::cout << "-(";
	for (int i = bit.size() - 1; i >= 0; i--) {
		std::cout << bit[i];
	}
	std::cout << ")_2";
}
void Integer::print_as_int() {
	unsigned long int product = 0;
	if (sign == 1)	std::cout << "-";
	else	std::cout << " ";
	for (size_t i = 0; i < bit.size(); i++) {
		product += bit[i] * int(pow(2, i));
	}
	std::cout << product;
}
void Integer::clean_up() {
	while (bit[bit.size() - 1] == 0 && bit.size() > 1) //&& bit.size() > 1
		bit.pop_back();
}

Integer operator+(Integer a, const Integer& b) {
	return a += b;
}
Integer operator*(Integer a, const Integer& b) {
	return a *= b;
}
Integer operator&(Integer a, const Integer& b) {
	a &= b;
	return a;
}
Integer operator|(Integer a, const Integer& b) {
	a |= b;
	return a;
}
bool operator!= (const Integer& lhs, const Integer& rhs) { return !(lhs == rhs); }
bool operator>  (const Integer& lhs, const Integer& rhs) { return rhs < lhs; }
bool operator<=(const Integer& lhs, const Integer& rhs) { return !(lhs > rhs); }
bool operator>=(const Integer& lhs, const Integer& rhs) { return !(lhs < rhs); }

//temp.print_as_bits();
//std::cout << std::endl;