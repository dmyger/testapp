/**
 * @brief Part of task 'ListNodes' used to load list from file.
 * @author Dmitriy Gertsog <dmyger@gmail.com>
 * @date: 2018-02-10
 *
 * @details Note: redirect dump of loaded list in file to compare results with original dump.
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
		list.Deserialize(file);
		fclose(file);
		cout << list;
	} else {
		cout << "Error: can't open file '" << ListDataFile << '\"' << endl;
	}
	return 0;
}
