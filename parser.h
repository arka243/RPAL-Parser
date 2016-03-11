#include "token.h"
#include<iostream>
#include<cstdlib>
#include<fstream>
#include<cstdio>

using namespace std;

class Parser {

	Token token;
	bool isAST;
	ifstream inputfilestream;
        char last_scanned_char;
	
	public:
	
	Parser(string filepath);
	
	Token* lexer();

	void parse(string exec_mode);

	void tokenValueReader(string value);
	void tokenTypeReader(string type);

	void treeBuilder(string node_type, string node_value, int node_num);

	void E();
	void Ew();
	void T();
	void Ta();
	void Tc();
	void B();
	void Bt();
	void Bs();
	void Bp();
	void A();
	void At();
	void Af();
	void Ap();
	void R();
	void Rn();
	void D();
	void Da();
	void Dr();
	void Db();
	void Vb();
	void Vl();

};
