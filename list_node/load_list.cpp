/**
 * @brief
 * @author Dmitriy Gertsog <dmyger@gmail.com>
 *
 */
#include <iostream>
#include "list_node.h"

using namespace std;

const char *ListDataFile = "listnode.bin";

int main(int argc, char *argv[])
{
	List list;
	FILE *file = fopen(ListDataFile, "rb");
	if(file) {
		list.Desirialize(file);
		fclose(file);
		cout << list;
	} else {
		cout << "Error: can't open file '" << ListDataFile << '\"' << endl;
	}
	return 0;
}
