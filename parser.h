#include "token.h"
#include<iostream>
#include<cstdlib>
#include<fstream>
#include<cstdio>
#include<stack>
#include<queue>
#include<map>

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

class environment {

	public:
	environment *origin;
	
	environment();
};

class controlStructure {

        public:
        treeNode *node;
	treeNode *leftchild;	// for LAMBDA encounters
        controlStructure *next;
	controlStructure *deltaIndex; // for LAMBDA encounters
	environment *currentEnv;	// for TAU encounters
	int numofelements;

        controlStructure(treeNode *node);
	controlStructure();
};

class stackElement {

	public:
	int intValue;
	string strValue;
	string primfunc;
	environment *stackEnv;
	string elementType;
	queue<stackElement *> tupleQueue;	// for TAU encounters
	controlStructure *lambdaRef;	// for LAMBDA encounters
	
	stackElement(environment *env);
	stackElement(int value);
	stackElement(string str);
	stackElement();
};

class Parser {

	Token *token;
	ifstream inputfilestream;
        char last_scanned_char;
	stack<treeNode*> nodeList;
	environment *env;	
	treeNode *rootNode;
	controlStructure *controlstructure;	
	stack<controlStructure*> controlStack;
	stack<stackElement*> executionStack;
	map<string, stackElement*> declaration_table;

	public:
	
	Parser(string filepath);
	
	Token* lexer();

	void parse(string exec_mode);

	void tokenValueReader(string value);
	void tokenTypeReader(string type);

	void treeBuilder(string node_type, string node_value, int node_num);
	void treeTraversal(treeNode *root, int dots);
	void treeStandardize(treeNode *root);
	void treetoControlStructure(treeNode *root, controlStructure *cs);	
	void generateControlStack(environment *env, controlStructure *cs);
	void generateExecutionStack(environment *env);
	void runCSEMachine();
	void printelement(stackElement *se);
	void printtuple(stackElement *se);

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
