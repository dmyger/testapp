/**
 * @brief Test task remove duplicate symbols in string data: "AAA BBB CCC" -> "A B C"
 *
 * @author Dmitriy Gertsog <dmyger@gmail.com>
 * @data 2018-02-07
 *
 */
#include <iostream>
#include <string.h>
using namespace std;

void RemoveDups(char *pStr)
{
	if(!pStr) return;
	char *p_src = pStr;
	char *p_dest = pStr;
	while(*p_src) {
		if(*p_dest != *p_src) {
			++p_dest;
			*p_dest = *p_src;
		}
		++p_src;
	}
	++p_dest;
	*p_dest = *p_src;
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		cout << "Require string to process" << endl;
		return 1;
	}
	int data_size = strlen(argv[1]) + 1;
	char data[data_size];
	strncpy(data, argv[1], data_size);

	RemoveDups(data);
	cout << '"' << data << '"' << endl;

	return 0;
}
