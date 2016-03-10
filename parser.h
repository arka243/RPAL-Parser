#include "token.h"
#include "lexer.h"
#include<iostream>
#include<cstdlib>
#include<fstream>
#include<cstdio>

using namespace std;

class Parser {

	Token token;
	Lexer lexicon;
	ifstream inputfilestream;

	public:
	
	Parser(string filepath);

	void tokenReader(string value);
	void tokenReader(string type);

	void treeBuilder(string node_type, string node_value, int node_num)

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
