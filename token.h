#pragma once
#include <string>

using namespace std;

struct token {
	string lex = "";
	string type;
	token();
	token( string lex,  string type);
};