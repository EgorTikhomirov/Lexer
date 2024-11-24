#include <string>
#include "token.h"
using namespace std;



token::token(){}
token::token( string lex,  string type) {
	this->lex = lex;
	this->type = type;
}


	
