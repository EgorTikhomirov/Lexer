#include <string>
#include <iostream>
#include "token.h"
using namespace std;



class hash_table {	
	int capacity = 1000;
	int size=0;
	token* tok = new token[1000];
public:
	int get_id(const token& token, int capacity) {
	int id = 0;

	for (char ch : token.lex) {
		id = (id * 31 + ch) % capacity;
	}
	return id;
	}
	hash_table() {};
	~hash_table() {
		delete[] tok;
	}

	void insert(token t) {
		
		int id = get_id(t, capacity);
		while (tok[id].lex != "" && tok[id].lex != t.lex) {
			id = (id + 1) % capacity;
		}
		if (tok[id].lex == "") {
			tok[id] = t;
			++size;
		}
	}
	void print(ostream& fout) {
		string str = "";
		for (int i = 0; i < capacity; i++) {
			if (tok[i].lex != "") {
				fout << tok[i].type << " | " << tok[i].lex << " | " << i << '\n';
			}
		}
		
	}
};