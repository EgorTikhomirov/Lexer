#include <iostream>
#include <fstream>
#include <vector>
#include "Lexer.cpp"
#include "token.h"
#include "hash-table.cpp"


using namespace std;

int main() {
   
    ifstream fin("task.txt");
    ofstream fout("result.txt");
    Lexer lex;
    vector<token> v;
    token m("sdd", "dsd");
    string s = lex.lexems(fin);
    hash_table hash;
    while (s != "") {
        if (lex.getState(s) == -1) {
            fout << s << " error\n";
        }
        else if (lex.getState(s) == 10 || lex.getState(s) == 9) {
            hash.insert(token(s, "int_num"));
            
        }
        else if (lex.getState(s) == 6 || lex.getState(s) == 7) {
            hash.insert(token(s, "operator"));
            
        }
        else if (lex.getState(s) == 5) {
            hash.insert(token(s, "separator"));
            
        }
        else if (lex.getState(s) == 21 || lex.getState(s) == 18 || lex.getState(s) == 16) {
            hash.insert(token(s, "key_word"));
            
        }
        else if (lex.getState(s) == 14) {
            hash.insert(token(s, "char_const"));
            
        }
        else if (lex.getState(s) == 1 || lex.getState(s) == 2 || lex.getState(s) == 3 || lex.getState(s) == 4 || lex.getState(s) == 11 || lex.getState(s) == 12 || lex.getState(s) == 13 || lex.getState(s) == 15 || lex.getState(s) == 17 || lex.getState(s) == 19 || lex.getState(s) == 20) {
            hash.insert(token(s, "id_name"));
            
        }
        else {
            fout << s << " error\n";
        }
        s = lex.lexems(fin);
    }
    hash.print(fout);

    return 0;
}
