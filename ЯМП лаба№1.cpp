#include <iostream>
#include <fstream>
#include <vector>
#include "Lexer.cpp"
#include "token.h"
#include "hash-table.cpp"
#include "Parser.cpp"


using namespace std;

int main() {

    ifstream fin("task.txt");
    ofstream fout("result_lex.txt");
    ofstream fout2("result_sin.txt");
    ofstream fout3("result_sem.txt");
    Lexer lex;
    vector<token> v;
    token m("sdd", "dsd");
    vector<pair<string, int>> code;
    vector <string> lextype;
    string s = lex.lexems(fin);
    if (lex.f == 0) {
        code.push_back({ s, lex.line });
    }
    else {
        code.push_back({ s, lex.line - 1 });
    }
    hash_table hash;
    int flag = 1;//проверка на ошибки на уровне лексики
    while (s != "") {
        if (lex.getState(s) == -1) {
            fout << s << " error\n";
            flag = 0;
            fout2 << "bad lexem in line: " << code[code.size() - 1].second << '\n';
        }
        else if (lex.getState(s) == 10 || lex.getState(s) == 9) {
            hash.insert(token(s, "int_num"));
            lextype.push_back("int_num");

        }
        else if (lex.getState(s) == 6 || lex.getState(s) == 7) {
            hash.insert(token(s, "operator"));
            lextype.push_back("operator");
        }
        else if (lex.getState(s) == 5) {
            hash.insert(token(s, "separator"));
            lextype.push_back("separator");
        }
        else if (lex.getState(s) == 21 || lex.getState(s) == 18 || lex.getState(s) == 16) {
            hash.insert(token(s, "key_word"));
            lextype.push_back("key_word");
        }
        else if (lex.getState(s) == 14) {
            hash.insert(token(s, "char_const"));
            lextype.push_back("char_const");
        }
        else if (lex.getState(s) == 1 || lex.getState(s) == 2 || lex.getState(s) == 3 || lex.getState(s) == 4 || lex.getState(s) == 11 || lex.getState(s) == 12 || lex.getState(s) == 13 || lex.getState(s) == 15 || lex.getState(s) == 17 || lex.getState(s) == 19 || lex.getState(s) == 20) {
            hash.insert(token(s, "id_name"));
            lextype.push_back("id_name");
        }
        else {
            fout << s << " error\n";
            fout2 << "bad lexem in line: " << code[code.size() - 1].second << '\n';
        }
        s = lex.lexems(fin);
        if (lex.f == 0) {
            code.push_back({ s, lex.line });
        }
        else {
            code.push_back({ s, lex.line - 1 });
        }
    }
    code.pop_back();
    hash.print(fout);
    if (flag) {
        Parser par(code, lextype);
        try {

            par.parse();
            fout3 << par.get_res();
        }

        catch (exception e) {
            if (string(e.what()) == "Expected ; ") {
                fout3 << '\n';
                fout3 << e.what() << " line: " << par.code[par.i].second - 1;
            }
            else {
                if (par.i < code.size()) {
                    fout3 << '\n';
                    fout3 << e.what() << " line: " << par.code[par.i].second;
                }
                else {
                    if (par.code[par.i - 1].first == ";") {
                        fout3 << '\n';
                        fout3 << "Error in line: " << par.code[par.i - 1].second + 1;
                    }
                    else {
                        if (e.what()[0] == 'W') {
                            fout3 << '\n';
                            fout3 << "Error in line: " << par.code[par.i - 1].second;
                        }
                        else {
                            fout3 << e.what();
                        }
                    }
                }

            }
        }

    }
    return 0;
}
