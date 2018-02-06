/**
 * @brief Testing task of implement a BigNumber class with support only arithmetic addition for integers.
 * @author Dmitriy Gertsog <dmyger@gmail.com>
 * @date 2018-01-28
 *
 * @details
 * To compile use: g++ -std=gnu++11 big_numbers.cpp -o big_numbers
 *
 * To execute: big_numbers <some_large_decimal_number> <another_large_decimal_number>
 *
 * To run the test run without any parameters.
 *
 * @remark Only positive numbers is processed.
 *
 */
#include <algorithm>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <string.h>

class BigNumber
{
public:
	BigNumber(int _digits = 0, char const *value = nullptr)
	    : in_use()
	    , is_negative()
	    , digits(packed_size(_digits))
	    , number(nullptr)
	{
		if(digits) number = new two_digits[digits];
		*this = value;
	}

	BigNumber(BigNumber const &other)
	    : in_use(other.in_use)
	    , is_negative(other.is_negative)
	    , digits(other.digits)
	    , number(nullptr)
	{
		number = new two_digits[digits];
		if(number) memcpy(number, other.number, digits);
	}

	~BigNumber()
	{
		if(number) delete[] number;
		// Not required, but just to ensure
		number = nullptr;
		in_use = digits = 0;
	}

	/// Set value with another BigNumber
	BigNumber &operator=(BigNumber const &other)
	{
		if(this == &other) return *this;

		if(digits < other.digits) {
			digits = other.digits;
			if(number) delete[] number;
			number = new two_digits[digits];
		}
		if(number) {
			in_use = other.in_use;
			memcpy(number, other.number, other.digits);
		} else
			in_use = 0;
		return *this;
	}

	/// Set value with string
	BigNumber &operator=(char const *value)
	{
		clear();
		if(!(value && *value)) return *this;
		unsigned len = strlen(value);
		if(!is_index(len)) increase(len);
		if(!number) return *this;

		is_negative = *value == '-';
		if(is_negative) ++value;

		char const *p_ch = value + len - 1;
		while(p_ch >= value) {
			unsigned char ch = *p_ch--;
			if('0' <= ch && ch <= '9')
				push_back(ch - '0');
			else {
				clear();
				break;
			}
		}
		return *this;
	}

	friend BigNumber operator+(BigNumber const &left, BigNumber const &right)
	{
		BigNumber result(left);
		return result += right;
	}

	BigNumber &operator+=(BigNumber const &right)
	{
		BigNumber const &large = in_use > right.in_use ? *this : right;
		BigNumber const &small = in_use > right.in_use ? right : *this;
		unsigned large_in_use = large.in_use;
		unsigned small_in_use = small.in_use;
		in_use = 0;

		unsigned rest = 0;
		for(unsigned i = 0; i < large_in_use; i++) {
			// TODO: обязательно нужна еще проверка на отрицательные числа
			unsigned sum = i < small_in_use ? small[i] : 0;
			unsigned char d = large[i];
			if(d > 9) break;  // Should not happen, but just to ensure
			sum += d + rest;
			push_back(sum % 10);
			rest = sum / 10;
		}
		if(rest) push_back(rest);

		return *this;
	}

	/// return digit from specified place by index and 'WrongDigit' if index out of range
	unsigned char operator[](unsigned index) const
	{
		if(is_index(index)) {
			two_digits const &td = number[index / 2];
			return index & 1 ? td.h : td.l;
		} else
			return WrongDigit;
	}

	/// Convert big number to string value
	std::string to_string() const
	{
		std::string value;
		value.reserve(in_use);
		if(is_negative) value.push_back('-');
		for(int i = in_use - 1; i >= 0; i--) {
			unsigned char d = (*this)[i];
			if(d <= 9)
				value.push_back(d + '0');
			else
				break;
		}
		return value;
	}

	/// Write big number in stream as string value
	friend std::ostream &operator<<(std::ostream &output, BigNumber const &bn)
	{
		output << bn.to_string();
		return output;
	}

	/// Fill number array with WrongDigit sign
	void clear()
	{
		if(number) memset(number, two_digits(), digits);
		in_use = 0;
		is_negative = false;
	}

protected:
	unsigned char static const WrongDigit = 0xF;  ///< Indicate wrong digit in number

	/// structure to keep two decimal values packed in one byte
	struct two_digits
	{
		two_digits()
		    : h(WrongDigit)
		    , l(WrongDigit)
		{
		}
		operator int() const { return h << 4 | l; }
		unsigned char h : 4, l : 4;
	};

	/// Append digit to the end of array
	void push_back(unsigned char digit)
	{
		if(!is_index(in_use + 1)) increase(in_use + 1);
		two_digits &d = number[in_use / 2];
		digit &= 0xF;
		if(in_use & 1)
			d.h = digit;
		else
			d.l = digit;
		++in_use;
	}

	/// Increase size of array, with keep old values
	bool increase(unsigned dec_digits)
	{
		unsigned new_size = packed_size(dec_digits);
		if(new_size <= digits) return true;
		two_digits *new_number = new two_digits[new_size];
		if(new_number) {
			memcpy(new_number, number, digits);
			delete[] number;
			number = new_number;
			memset(number + digits, two_digits(), new_size - digits);
			digits = new_size;
		}
		return new_number;
	}

	/// Check is required index valid
	bool is_index(unsigned index) const { return (index / 2) <= digits; }

	/// return count of packed strustures required to keep specified decimal digits
	unsigned packed_size(unsigned digits) const { return (digits / 2) + (digits & 1); }

	unsigned in_use;     ///< Actual used decimal digits
	bool is_negative;    ///< True if number is negative
	unsigned digits;     ///< Number of packed 'two digits' structures; Size of array 'number'
	two_digits *number;  ///< Keep packed decimal digits in the reverse order
};

/// Structure for testing data
struct TestData
{
	bool success;      // 'true' if test should be successful
	unsigned digits;   // amount of reserved decimal digits
	char const *num1;  // first number
	char const *num2;  // second number
	char const *sum;   // result: num1 + num2
};

std::vector<TestData> const test_data = {
	{ true, 10, "9403122312080358940358998430329430580543434", "9876543210123456789012345678901234567890123", "19279665522203815729371344109230665148433557" },
	{ true, 100, "9403122312080358940358998430329430580543434", "100000000000000000000000000000000000000000000000", "100009403122312080358940358998430329430580543434" },
	{ true, 43, "9403122312080358940358998430329430580543434", "0", "9403122312080358940358998430329430580543434" },
	{ true, 43, "9403122312080358940358998430329430580543434", "1", "9403122312080358940358998430329430580543435" },
	{ true, 50, "1", "99999999999999999999999999999999999999999999999999", "100000000000000000000000000000000000000000000000000" },
	{ true, 1, "99999999999999999999999999999999999999999999999999", "99999999999999999999999999999999999999999999999999", "199999999999999999999999999999999999999999999999998" },
	{ true, 0, "11111111111111111111111111111111111111111111111111", "99999999999999999999999999999999999999999999999999", "111111111111111111111111111111111111111111111111110" },
	{ true, 43, "9403122312080358940358998430329430580543434", nullptr, "9403122312080358940358998430329430580543434" },
	{ true, 0, nullptr, nullptr, "" },
	{ false, 43, "9403122312080358940358998430329430580543434", "0", "1" }
};

/// simple unit test to check work of BigNumber implemention
bool do_test(TestData const &data)
{
	BigNumber x(data.digits, data.num1);
	BigNumber y;
	y = x + BigNumber(data.digits, data.num2);
	bool res = (y.to_string() == data.sum) == data.success;
	if(res)
		std::cout << "OK";
	else
		std::cout << "Error: " << y << " != " << data.sum;
	std::cout << std::endl;
	return res;
}

int main(int argc, char const *argv[])
{
	if(argc < 3) {
		std::cout << "Note: required two numbers as arguments" << std::endl;
		bool tests_ok = true;
		int i = 0;
		std::for_each(test_data.begin(), test_data.end(), [&tests_ok, &i](TestData const &data) {
			std::cout << "Run test " << i++ << ": ";
			tests_ok &= do_test(data);
		});
		return tests_ok ? 0 : 1;
	}

	int digits = std::max(strlen(argv[1]), strlen(argv[2]));
	std::cout << "Max number of decimal digits=" << digits << std::endl;
	int bytes = ceil(digits * log2(10) / 8);
	std::cout << "To store in binary format required at least " << bytes << " bytes" << std::endl;

	BigNumber a(digits, argv[1]);
	BigNumber b(digits, argv[2]);
	std::cout << "BigNumber A='" << a << "'" << std::endl;
	std::cout << "BigNumber B='" << b << "'" << std::endl;
	std::cout << "BigNumber A+B='" << a + b << "'" << std::endl;

	return 0;
}
