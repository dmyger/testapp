/**
 * @brief Test task print integer in binary format
 *
 * @author Dmitriy Gertsog <dmyger@gmail.com>
 * @date 2018-02-06
 *
 */
#include <iostream>
#include <stdlib.h>
using namespace std;

void print_binary(int number)
{
	unsigned int single_bit = ~(~0u >> 1);
	while(single_bit) {
		char bit = number & single_bit ? '1' : '0';
		cout << bit;
		single_bit >>= 1;
	}
	cout << endl;
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		cout << "Require integer number" << endl;
		return 1;
	}
	print_binary(atoi(argv[1]));
	return 0;
}
