#ifndef __LIST_NODE_H__
#define __LIST_NODE_H__
#include <string>
#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct ListNode
{
	ListNode(const char *data)
	    : prev()
	    , next()
	    , rand()
	    , data(data)
	{}

	ListNode(const char *data, unsigned data_size, unsigned rand_index)
	    : prev()
	    , next()
	    , rand(reinterpret_cast<ListNode *>(rand_index))
	    , data(data, data_size)
	{}

	ListNode *prev;
	ListNode *next;
	ListNode *rand;
	std::string data;
};

class List
{
public:
	List()
	    : head()
	    , tail()
	    , count()
	{}

	~List()
	{
		while(ListNode *node = tail) {
			tail = node->prev;
			delete node;
		}
	}

	/// Store ListNodes in file
	void Serialize(FILE *file)
	{
		typedef std::map<ListNode *, size_t> RandMapType;
		RandMapType rand_map;
		// Save 'rand' pointers to map as a key
		ListNode *node = head;
		while(node) {
			rand_map[node->rand] = 0;
			node = node->next;
		}
		// Set '1' based index for 'rand' pointers
		node = head;
		size_t i = 0;
		while(node) {
			++i;
			RandMapType::iterator it = rand_map.find(node);
			if(it != rand_map.end()) it->second = i;
			node = node->next;
		}
		// Save Nodes in file
		node = head;
		while(node) {
			FileRecord record;
			RandMapType::iterator it = rand_map.find(node->rand);
			record.rand_index = it != rand_map.end() ? it->second : 0;
			record.data_size = node->data.size();
			fwrite(&record, sizeof(record), 1, file);
			fwrite(node->data.c_str(), record.data_size, 1, file);
			node = node->next;
		}
	}

	/// Load ListNodes from file
	void Desirialize(FILE *file)
	{
		typedef std::map<size_t, ListNode *> RandMapType;
		RandMapType rand_map;
		FileRecord record;
		char *buffer = NULL;
		size_t buffer_size = 0;
		// Read raw records from file in List
		while(fread(&record, 1, sizeof(record), file) == sizeof(record)) {
			if(record.data_size > buffer_size) {
				buffer_size = record.data_size;
				delete[] buffer;
				buffer = new char[buffer_size];
			}
			if(!buffer) {
				std::cerr << "Out of memmory" << std::endl;
				break;
			}
			if(fread(buffer, 1, record.data_size, file) != record.data_size) break;
			*this += new ListNode(buffer, record.data_size, record.rand_index);
			// Collect all indexes of 'rand' field
			rand_map[record.rand_index] = NULL;
		}
		// Set accordance index to Node's pointer
		ListNode *node = head;
		size_t i = 0;
		while(node) {
			++i;
			RandMapType::iterator it = rand_map.find(i);
			if(it != rand_map.end()) it->second = node;
			node = node->next;
		}
		// Restore pointer from map
		node = head;
		while(node) {
			RandMapType::iterator it = rand_map.find(reinterpret_cast<size_t>(node->rand));
			if(it != rand_map.end()) node->rand = it->second;
			node = node->next;
		}
	}

	/// Add node to List
	List &operator+=(ListNode *node)
	{
		if(!head)
			head = node;
		else {
			node->prev = tail;
			tail->next = node;
		}
		tail = node;
		++count;
		return *this;
	}

	/// Get node by index
	ListNode *operator[](int index)
	{
		if(index < 0 || index >= count) return NULL;
		ListNode *node = head;
		while(node && index > 0) {
			node = node->next;
			--index;
		}
		return node;
	}

	/// This method only to build list for test, because it works very slowly O(N^2) due to seek nodes with the operator []
	void randomize()
	{
		srand(time(NULL));
		ListNode *node = head;
		while(node) {
			size_t rand_idx = rand();
			if(count > RAND_MAX / 2) rand_idx *= rand();
			rand_idx %= count * 2;  // *2 to get ~50% of 'rand' as NULL
			node->rand = (*this)[rand_idx];
			node = node->next;
		}
	}

	/// Write dump of List to stream
	friend std::ostream &operator<<(std::ostream &output, List const &list)
	{
		ListNode *node = list.head;
		while(node) {
			output << node->data;
			if(node->rand) output << ", rand=>" << node->rand->data;
			output << std::endl;
			node = node->next;
		}
		return output;
	}

private:
	/// Structure used to save/restore data from the binary file.
	/// Each Node stored in following format:
	/// 4 bytes - '1' based index for 'rand' field
	/// 4 byte - size of data string
	/// data string according its size with no zero ending
	struct FileRecord
	{
		unsigned rand_index;  ///< '1' based index
		unsigned data_size;   ///< specify size of following data block
	};

	ListNode *head;
	ListNode *tail;
	int count;
};

#endif  // __LIST_NODE_H__