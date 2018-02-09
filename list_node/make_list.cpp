/**
 * @brief Part of task 'ListNodes' used to create list. Accept one argument: number of Nodes in List.
 * @author Dmitriy Gertsog <dmyger@gmail.com>
 * @date: 2018-02-10
 *
 * @details Note: redirect dump of created list in file to compare results after load list.
 *
 */
#include <iostream>
#include "list_node.h"

using namespace std;

int main(int argc, char *argv[])
{
	int count = 10;
	if(argc >= 2) count = atoi(argv[1]);

	List list;
	for(int i = 0; i < count; i++) {
		char data[20];
		snprintf(data, sizeof(data), "Node %03d", i);
		list += new ListNode(data);
	}
	list.randomize();
	cout << list;

	FILE* file = fopen("listnode.bin", "wb");
	list.Serialize(file);
	fclose(file);
	return 0;
}
