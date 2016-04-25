#include "token.h"
#include<iostream>
#include<cstdlib>
#include<fstream>
#include<cstdio>
#include<stack>

using namespace std;

class treeNode {

	string node_value;
	string node_type;

	public:

	treeNode *left;
	treeNode *right;
	
	treeNode(string value, string type);

	string getNodeValue();
	void setNodeValue(string value);
	string getNodeType();
	void setNodeType(string type);
};

class Parser {

	Token *token;
	ifstream inputfilestream;
        char last_scanned_char;
	stack<treeNode*> nodeList;
	treeNode *rootNode;
	
	public:
	
	Parser(string filepath);
	
	Token* lexer();

	void parse(string exec_mode);

	void tokenValueReader(string value);
	void tokenTypeReader(string type);

	void treeBuilder(string node_type, string node_value, int node_num);
	void treeTraversal(treeNode *root, int dots);
	void treeStandardize(treeNode *root);
	void treetoControlStructure(treeNode *root);	

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
