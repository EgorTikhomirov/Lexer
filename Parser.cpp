#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Node {
	string odwrotny_zapis_Polski = "";
	string type = "-1";
	int count = 0;
	string data;
	vector<Node> children;
	Node(string s) {
		data = s;
	}
	void addSon(string s) {
		Node n(s);
		children.push_back(n);
	}
	Node& getSon(int i) {
		return children[i];
	}
};

class Parser
{
private:
	map<string, string> vars;
	string global_type;
	string cur_type = "";
	string input, lexeme;
	string result;
	int otstup = 0;
	void Function(Node& n) {
		if (i < code.size() && (code[i].first == "int" || code[i].first == "char")) {
			global_type = code[i].first;
			cout << "Function" << '\n';
			n.addSon("Begin");
			Begin(n.getSon(0));
			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + ' ';
			n.addSon("Descriptions");
			Descriptions(n.getSon(1));
			n.odwrotny_zapis_Polski += n.getSon(1).odwrotny_zapis_Polski + ' ';

			//////
			n.addSon("Operators");
			Operators(n.getSon(2));
			n.odwrotny_zapis_Polski += n.getSon(2).odwrotny_zapis_Polski + ' ';
			n.addSon("End");
			End(n.getSon(3));
			n.odwrotny_zapis_Polski += n.getSon(3).odwrotny_zapis_Polski;
			result = n.odwrotny_zapis_Polski;
			//cout << '\n' << n.odwrotny_zapis_Polski;

		}
		else {
			throw exception("Wrong input");
		}
	}
	void Begin(Node& n) {
		++otstup;
		if (i < code.size() && (code[i].first == "int" || code[i].first == "char")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Begin" << '\n';
			n.addSon("Type");
			Type(n.getSon(0));
			n.addSon("FunctionName");
			FunctionName(n.getSon(1));

			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + ' ' + n.getSon(1).odwrotny_zapis_Polski + "DECL ";
		}
		else throw exception("Wrong input");
		--otstup;
	}
	void Descriptions(Node& n) {
		++otstup;
		if (i < code.size() && (code[i].first == "int" || code[i].first == "char")) {

			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Descriptions" << '\n';
			n.addSon("Descr");
			Descr(n.getSon(n.children.size() - 1));
			n.addSon("Descriptions1");
			Descriptions1(n.getSon(n.children.size() - 1));
			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + ' ' + n.getSon(n.children.size() - 1).odwrotny_zapis_Polski;
		}
		else {
			throw exception("Wrong input");
		}
		--otstup;
	}
	void Operators(Node& n) {
		++otstup;
		if (i < code.size() && (lextype[i] == "id_name")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Operators" << '\n';
			n.addSon("Op");
			Op(n.getSon(n.children.size() - 1));
			n.addSon("Operators1");
			Operators1(n.getSon(n.children.size() - 1));
			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + ' ' + n.getSon(n.children.size() - 1).odwrotny_zapis_Polski;

		}
		else {
			throw exception("Wrong input");
		}
		--otstup;
	}
	void End(Node& n) {
		++otstup;
		if (i < code.size() && code[i].first == "return") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "End" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("return");
			cout << "	return" << '\n';
			++i;
			n.addSon("Id");
			Id(n.getSon(n.children.size() - 1));
			if (i + 1 < code.size() && code[i].first == ";" && code[i + 1].first == "}") {
				for (int j = 0; j < otstup; ++j) {
					cout << "	";
				}
				n.addSon(";");
				cout << "	;" << '\n';
				for (int j = 0; j < otstup; ++j) {
					cout << "	";
				}
				n.addSon("}");
				cout << "	}";
				i += 2;
				if (i < code.size()) {
					throw exception("Wrong input");
				}
			}
			else {
				throw exception("Wrong input");
			}
		}
		else {
			throw exception("Wrong input");
		}
		--otstup;
	}
	void Op(Node& n) {
		++otstup;
		if (i < code.size() && (lextype[i] == "id_name")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Op" << '\n';
			n.odwrotny_zapis_Polski += code[i].first + ' ';
			n.type = vars[code[i].first];
			n.addSon("Id");
			Id(n.getSon(n.children.size() - 1));
			if (i < code.size() && (code[i].first == "=")) {
				for (int j = 0; j < otstup; ++j) {
					cout << "	";
				}
				n.addSon("=");
				cout << "	" << "=\n";
				++i;
			}
			else {
				throw exception("Wrong input");
			}
			n.addSon("Op1");
			Op1(n.getSon(n.children.size() - 1));
			if (n.type == "char" && n.getSon(n.children.size() - 1).type == "char") {
				throw runtime_error("incorrect types ");
			}
			else if (n.type == "char") {
				if (n.getSon(n.children.size() - 1).type != "const_char") {
					throw runtime_error("incorrect types ");
				}
			}

			else if (n.type != "char" && n.type != n.getSon(n.children.size() - 1).type) {
				cout << n.getSon(n.children.size() - 1).type;
				throw runtime_error("incorrect types ");
			}


			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + ' ' + n.getSon(n.children.size() - 1).odwrotny_zapis_Polski + "=";

		}
		else {
			throw exception("Wrong input");
		}
		--otstup;
	}
	void Op1(Node& n) {
		++otstup;
		if (i < code.size() && (code[i].first == "(")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Op1" << '\n';
			n.addSon("NumExpr");
			NumExpr(n.getSon(n.children.size() - 1));
			n.type = n.getSon(0).type;
			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + ' ';
			if (i < code.size() && (code[i].first == ";")) {
				for (int j = 0; j < otstup; ++j) {
					cout << "	";
				}
				n.addSon(";");
				cout << "	" << ";\n";
				++i;
			}
			else throw exception("Wrong input");
		}
		else if (i < code.size() && (lextype[i] == "id_name")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Op1" << '\n';
			n.type = vars[code[i].first];
			n.addSon("NumExpr");
			NumExpr(n.getSon(n.children.size() - 1));
			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + ' ';
			if (i < code.size() && (code[i].first == ";")) {
				for (int j = 0; j < otstup; ++j) {
					cout << "	";
				}
				n.addSon(";");
				cout << "	" << ";\n";
				++i;
			}
			else throw exception("Wrong input");
		}
		else if (i < code.size() && (lextype[i] == "int_num")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Op1" << '\n';
			n.type = "int";
			n.addSon("NumExpr");
			NumExpr(n.getSon(n.children.size() - 1));
			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + ' ';
			if (i < code.size() && (code[i].first == ";")) {
				for (int j = 0; j < otstup; ++j) {
					cout << "	";
				}
				n.addSon(";");
				cout << "	" << ";\n";
				++i;
			}
			else throw exception("Wrong input");
		}
		else if (i < code.size() && (lextype[i] == "char_const")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Op1" << '\n';
			n.type = "const_char";
			n.addSon("StringExpr");
			StringExpr(n.getSon(n.children.size() - 1));
			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + ' ';
			if (i < code.size() && (code[i].first == ";")) {
				for (int j = 0; j < otstup; ++j) {
					cout << "	";
				}
				n.addSon(";");
				cout << "	" << ";\n";
				++i;
			}
			else throw exception("Wrong input");
		}
		else {
			throw exception("Wrong input");
		}
		--otstup;
	}
	void Operators1(Node& n) {
		++otstup;
		if (i < code.size() - 1 && code[i].first == "return") {
			n.odwrotny_zapis_Polski += code[i + 1].first + ' ';
			if (vars[code[i + 1].first] != global_type) {
				throw runtime_error("incorrect types ");
			}
			n.odwrotny_zapis_Polski += code[i].first + ' ';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Operators1" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("eps");
			cout << "	" << "eps" << '\n';
		}
		else if (i < code.size() && lextype[i] == "id_name") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}

			cout << "Operators1" << '\n';
			n.addSon("Operators");
			Operators(n.getSon(n.children.size() - 1));
			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + ' ';
		}
		--otstup;
	}
	void NumExpr(Node& n) {
		++otstup;
		if (i < code.size() && (code[i].first == "(" || lextype[i] == "id_name" || lextype[i] == "int_num")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "NumExpr" << '\n';
			n.addSon("SimpleNumExpr");
			SimpleNumExpr(n.getSon(n.children.size() - 1));
			n.addSon("NumExpr1");

			NumExpr1(n.getSon(n.children.size() - 1));
			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + ' ' + n.getSon(n.children.size() - 1).odwrotny_zapis_Polski;
			if (n.getSon(n.children.size() - 1).type == "-1" || (n.getSon(0).type == n.getSon(1).type)) {
				n.type = n.getSon(0).type;
			}
			else throw runtime_error("incorrect types ");
		}
		else throw exception("Wrong input");
		--otstup;
	}
	void NumExpr1(Node& n) {
		++otstup;
		if (i < code.size() && code[i].first == ")") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "NumExpr1" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("eps");
			cout << "	" << "eps" << '\n';
		}

		else if (i < code.size() && code[i].first == ";") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "NumExpr1" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("eps");
			cout << "	" << "eps" << '\n';
		}
		else if (i < code.size() && code[i].first == "+") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "NumExpr1" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("+");
			cout << "	" << "+" << '\n';
			++i;
			n.addSon("NumExpr");
			NumExpr(n.getSon(n.children.size() - 1));
			n.type = n.getSon(n.children.size() - 1).type;
			n.odwrotny_zapis_Polski += n.getSon(n.children.size() - 1).odwrotny_zapis_Polski + ' ' + n.getSon(0).data;
		}
		else if (i < code.size() && code[i].first == "-") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "NumExpr1" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("-");
			cout << "	" << "-" << '\n';
			++i;
			n.addSon("NumExpr");
			NumExpr(n.getSon(n.children.size() - 1));
			n.type = n.getSon(n.children.size() - 1).type;
			n.odwrotny_zapis_Polski += n.getSon(n.children.size() - 1).odwrotny_zapis_Polski + ' ' + n.getSon(0).data;
		}
		else throw exception("Wrong input");
		--otstup;
	}
	void SimpleNumExpr(Node& n) {
		++otstup;
		if (i < code.size() && code[i].first == "(") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "SimpleNumExpr" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("(");
			cout << "	" << "(\n";
			++i;
			n.addSon("NumExpr");
			NumExpr(n.getSon(n.children.size() - 1));
			n.type = n.getSon(n.children.size() - 1).type;
			n.odwrotny_zapis_Polski += n.getSon(n.children.size() - 1).odwrotny_zapis_Polski + ' ';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon(")");
			cout << "	" << ")\n";
			++i;
		}
		else if (i < code.size() && lextype[i] == "id_name") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "SimpleNumExpr" << '\n';
			n.odwrotny_zapis_Polski += code[i].first + ' ';
			n.type = vars[code[i].first];
			n.addSon("Id");
			Id(n.getSon(n.children.size() - 1));


		}
		else if (i < code.size() && lextype[i] == "int_num") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "SimpleNumExpr" << '\n';
			n.odwrotny_zapis_Polski += code[i].first + ' ';
			n.addSon("Const");
			Const(n.getSon(n.children.size() - 1));
			n.type = "int";
		}
		else throw exception("Wrong input");
		--otstup;
	}
	void SimpleStringExpr(Node& n) {
		++otstup;
		if (i < code.size() && lextype[i] == "char_const") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.odwrotny_zapis_Polski += code[i].first + ' ';
			cout << "SimpleStringExpr" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon(code[i].first);
			cout << "	" << code[i].first << '\n';
			++i;
		}
		else throw exception("Wrong input");
		--otstup;
	}
	void StringExpr(Node& n) {
		++otstup;
		if (i < code.size() && lextype[i] == "char_const") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "StringExpr" << '\n';
			n.addSon("SimpleStringExpr");
			SimpleStringExpr(n.getSon(n.children.size() - 1));
			n.addSon("StringExpr");
			StringExpr1(n.getSon(n.children.size() - 1));
			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + ' ' + n.getSon(n.children.size() - 1).odwrotny_zapis_Polski;
		}
		else throw exception("Wrong input");
		--otstup;
	}
	void StringExpr1(Node& n) {
		++otstup;
		if (i < code.size() && code[i].first == ";") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "StringExpr1" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("eps");
			cout << "	" << "eps" << '\n';
		}
		else if (i < code.size() && code[i].first == "+") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "StringExpr1" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("+");
			cout << "	" << "+" << '\n';
			++i;
			n.addSon("StringExpr");
			StringExpr(n.getSon(n.children.size() - 1));
			n.odwrotny_zapis_Polski += n.getSon(n.children.size() - 1).odwrotny_zapis_Polski + ' ' + n.getSon(0).data;
		}
		else throw exception("Wrong input");
		--otstup;
	}
	void Const(Node& n) {
		++otstup;
		if (i < code.size() && lextype[i] == "int_num") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Const" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon(code[i].first);
			cout << "	" << code[i].first << '\n';
			++i;
			n.type = "int";
		}
		else throw exception("Wrong input");
		--otstup;
	}
	void Type(Node& n) {
		++otstup;
		for (int j = 0; j < otstup; ++j) {
			cout << "	";
		}
		cout << "Type" << '\n';
		for (int j = 0; j < otstup; ++j) {
			cout << "	";
		}
		n.odwrotny_zapis_Polski += code[i].first + ' ';
		n.addSon(code[i].first);
		cout << "	" << code[i].first << '\n';
		++i;
		--otstup;
	}
	void FunctionName(Node& n) {
		++otstup;
		if (i < code.size() && (lextype[i] == "id_name")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.odwrotny_zapis_Polski += code[i].first + ' ';
			cout << "FunctionName" << '\n';
			n.addSon("Id");
			Id(n.getSon(n.children.size() - 1));
		}
		else {
			throw exception("Wrong input");
		}
		if (i + 1 < code.size() && (code[i].first == "(" && code[i + 1].first == ")")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("(");
			n.addSon(")");
			cout << "(\n";
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << ")\n";
			i += 2;
		}
		else {
			throw exception("Wrong input");
		}
		if (i < code.size() && code[i].first == "{") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("}");
			cout << "{\n";
			++i;
		}
		else {
			throw exception("Wrong input");
		}
		--otstup;
	}
	void Id(Node& n) {
		++otstup;
		for (int j = 0; j < otstup; ++j) {
			cout << "	";
		}
		cout << "Id" << '\n';
		for (int j = 0; j < otstup; ++j) {
			cout << "	";
		}
		n.addSon(code[i].first);
		cout << "	" << code[i].first << '\n';
		++i;
		--otstup;
	}
	void Descr(Node& n) {
		++otstup;
		if (i < code.size() && (code[i].first == "int" || code[i].first == "char")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Descr" << '\n';
			n.addSon("Type");
			Type(n.getSon(n.children.size() - 1));
			n.addSon("VarList");
			VarList(n.getSon(n.children.size() - 1));
			n.odwrotny_zapis_Polski += n.getSon(0).odwrotny_zapis_Polski + n.getSon(n.children.size() - 1).odwrotny_zapis_Polski + ' ';
			if (n.getSon(n.children.size() - 1).count > 1) {
				n.odwrotny_zapis_Polski += to_string(n.getSon(n.children.size() - 1).count + 1) + ' ';
			}
			string cur = "";
			string tipchik;
			for (int Z = 0; Z < n.odwrotny_zapis_Polski.size(); ++Z) {
				if (n.odwrotny_zapis_Polski[Z] == ' ' && cur.size() > 0) {
					if (cur == "int" || cur == "char") {
						tipchik = cur;
					}
					else {
						if (vars.find(cur) == vars.end()) {
							vars[cur] = tipchik;
						}
						else throw runtime_error("muliple announcement: " + cur);
					}
					cur = "";
					continue;
				}
				else {
					if (n.odwrotny_zapis_Polski[Z] != ' ') {
						cur += n.odwrotny_zapis_Polski[Z];
					}
				}
			}
			n.odwrotny_zapis_Polski += "DECL ";
		}
		else throw exception("Wrong input");
		if (i < code.size() && (code[i].first == ";")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon(";");
			cout << "	" << ";\n";
			++i;
		}
		else throw exception("Expected ; ");
		--otstup;
	}
	void Descriptions1(Node& n) {
		++otstup;
		if (i < code.size() && (code[i].first == "int" || code[i].first == "char")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Descriptions1" << '\n';
			n.addSon("Descriptions");
			Descriptions(n.getSon(n.children.size() - 1));
			n.odwrotny_zapis_Polski += n.getSon(n.children.size() - 1).odwrotny_zapis_Polski + ' ';
		}
		else if (i < code.size() && (lextype[i] == "id_name")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "Descriptions1" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("eps");
			cout << "	" << "eps\n";
		}
		else throw exception("Wrong input");
		--otstup;
	}
	void VarList(Node& n) {
		++otstup;
		if (i < code.size() && (lextype[i] == "id_name")) {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			++n.count;
			n.odwrotny_zapis_Polski += code[i].first + ' ';
			cout << "VarList" << '\n';
			n.addSon("Id");
			Id(n.getSon(n.children.size() - 1));
			n.addSon("VarList1");
			VarList1(n.getSon(n.children.size() - 1));
			n.count += n.getSon(1).count;
			n.odwrotny_zapis_Polski += n.getSon(1).odwrotny_zapis_Polski;
		}
		else throw exception("Wrong input");
		--otstup;
	}
	void VarList1(Node& n) {
		++otstup;
		if (i < code.size() && code[i].first == ";") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "VarList1" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon("eps");
			cout << "	" << "eps\n";

		}
		else if (i < code.size() && code[i].first == ",") {
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			cout << "VarList1" << '\n';
			for (int j = 0; j < otstup; ++j) {
				cout << "	";
			}
			n.addSon(",");
			cout << "	" << ",\n";
			++i;
			n.addSon("VarList");
			VarList(n.getSon(n.children.size() - 1));
			n.count += n.getSon(n.children.size() - 1).count;
			n.odwrotny_zapis_Polski += n.getSon(n.children.size() - 1).odwrotny_zapis_Polski;

		}
		else throw exception("Expected ; ");
		--otstup;
	}
public:
	vector<pair<string, int>> code;
	vector<string> lextype;
	int i = 0;
	Parser(vector<pair<string, int>>& v, vector<string>& v1) {
		code = v;
		lextype = v1;
	}

	void parse() {
		Node S("Function");
		Function(S);

	}
	string get_res() {
		return result;
	}
};
