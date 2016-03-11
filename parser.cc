#include "parser.h"
#include<algorithm>
#include<vector>

treeNode::treeNode(string value, string type) {

	node_value = value;
	node_type = type;
}

string treeNode::getNodeValue() {

	return node_value;
}

void treeNode::setNodeValue(string value) {

	node_value = value;
}

string treeNode::getNodeType() {

	return node_type;
}

void treeNode::setNodeType(string type) {

	node_type = type;
}

Parser::Parser(string filepath) {

	isAST = false;
	last_scanned_char = ' ';
	inputfilestream.open(filepath.c_str(), ifstream::in);
	if(!inputfilestream) {
		cout << "\nError...Could not open file!";
		cout << "\nExiting ...";
		exit(0);
	}
	token = lexer();
}

void Parser::parse(string exec_mode) {

	if(exec_mode.compare("ast")) 
		isAST = true;
	E();
	if(token->getTokenType().compare("ENDOFFILE") != 0)
		cout << "\nError --- End of File expected after lexical analysis!";
	rootNode = nodeList.top();
	if(isAST)
		//generate_tree
	cout << "\nAbstract Syntax Tree generated..!!";
}

/* Following grammar rules
mentioned in rpal.grammar.pdf */

void Parser::E() {

	if(token->getTokenValue().compare("let") == 0 && token->getTokenType().compare("RESERVED") == 0) {
		cout << "\nRule: E -> let D in E";
		tokenValueReader("let");
		D();
		tokenValueReader("in");
		E();
		treeBuilder("LET", "let", 2);
	}
	else if(token->getTokenValue().compare("fn") == 0 && token->getTokenType().compare("RESERVED") == 0) {
		cout << "\nRule: E -> fn Vb+ . E";
		tokenValueReader("fn");
		int i = 0;
		while((token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) || token->getTokenType().compare("IDENTIFIER") == 0) {
			i++;
			Vb();
		}
		tokenValueReader(".");
		E();
		treeBuilder("LAMBDA", "lambda", i+1);
	}
	else {
		cout << "\nRule: E -> Ew";
		Ew();
	}
}

void Parser::Ew() {

	T();
	if(token->getTokenValue().compare("where") == 0 && token->getTokenType().compare("RESERVED") == 0) {
		cout << "\nRule: Ew -> T where Dr";
		tokenValueReader("where");
		Dr();
		treeBuilder("WHERE", "where", 2);
	}
	else {
		cout << "\nRule: Ew -> T";
	}
}

void Parser::T() {

		Ta();
        int i = 0;
        while(token->getTokenValue().compare(",") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) {
                cout << "\nRule: T -> Ta (, Ta) +";
                i++; 
                tokenValueReader(",");
                Ta();
        } 
        if(i > 0) {
				cout << "\nRule: T -> Ta";
                treeBuilder("TAU", "tau", i+1);
		}
}

void Parser::Ta() {

		Tc();
        while(token->getTokenValue().compare("aug") == 0 && token->getTokenType().compare("RESERVED") == 0) { 
				cout << "\nRule: Ta -> Ta aug Tc";
                tokenValueReader("aug");
                Tc();
                treeBuilder("AUG", "aug", 2);
        }
        cout << "\nRule: Ta -> Tc";
}

void Parser::Tc() {

		B();
        if(token->getTokenValue().compare("->") == 0 && token->getTokenType().compare("OPERATOR") == 0) { 
                cout << "\nRule: Tc -> B -> Tc | Tc";
                tokenValueReader("->");
                Tc();
                tokenValueReader("|");
                Tc();
                treeBuilder("CONDITION", "->", 3);
        }
        else {
				cout << "\nRule: Tc -> B";
		}
}

void Parser::B() {

		Bt();
        while(token->getTokenValue().compare("or") == 0 && token->getTokenType().compare("RESERVED") == 0) { 
                cout << "\nRule: B -> B or Bt";
                tokenValueReader("or");
                Bt();
                treeBuilder("OR", "or", 2);
        }
        cout << "\nRule: B -> Bt";
}

void Parser::Bt() {

        Bs();
        while(token->getTokenValue().compare("&") == 0 && token->getTokenType().compare("OPERATOR") == 0) { 
                cout << "\nRule: Bt -> Bt & Bs";
                tokenValueReader("&");
                Bs();
                treeBuilder("AMPERSAND", "&", 2);
        }
        cout << "\nRule: Bt -> Bs";
}

void Parser::Bs() {

        if(token->getTokenValue().compare("not") == 0 && token->getTokenType().compare("RESERVED") == 0) {
				cout << "\nRule: Bs -> not Bp";
                tokenValueReader("not");
                Bp();
                treeBuilder("NOT", "not", 1);
        }
        else {
                Bp();
                cout << "\nRule: Bs -> Bp";
		}
}

void Parser::Bp() {

		A();
        if((token->getTokenValue().compare("gr") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare(">") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
				cout << "\nRule: Bp -> A (gr | >) A";
                tokenValueReader(token->getTokenValue());
                A();
                treeBuilder("GREATER", "gr", 2);
        }
        else if((token->getTokenValue().compare("ge") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare(">=") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                cout << "\nRule: Bp -> A (ge | >=) A";
                tokenValueReader(token->getTokenValue());
                A();
                treeBuilder("GREATEREQUAL", "ge", 2);
        }
        else if((token->getTokenValue().compare("ls") == 0 && token->getTokenType().compare("RESERVED") == 0 ) || (token->getTokenValue().compare("<") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                cout << "\nRule: Bp -> A (ls | <) A";
                tokenValueReader(token->getTokenValue());
                A();
                treeBuilder("LESS", "ls", 2);
        }
        else if((token->getTokenValue().compare("le") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare("<=") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                cout << "\nRule: Bp -> A (le | <=) A";
                tokenValueReader(token->getTokenValue());
                A();
                treeBuilder("LESSEQUAL", "le", 2);
        }
        else if(token->getTokenValue().compare("eq") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                cout << "\nRule: Bp -> A eq A";
                tokenValueReader("eq");
                A();
                treeBuilder("EQUAL", "eq", 2);
        }
        else if(token->getTokenValue().compare("ne") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                cout << "\nRule: Bp -> A ne A";
                tokenValueReader("ne");
                A();
                treeBuilder("NOTEQUAL", "ne", 2);
        }
        else {
				cout << "\nRule: Bp -> A";
		}
}

void Parser::A() {

        if(token->getTokenValue().compare("+") == 0 && token->getTokenType().compare("OPERATOR") == 0) { 
				cout << "\nRule: A -> A + At";
                tokenValueReader("+");
                At();
        }
        else if(token->getTokenValue().compare("-") == 0 && token->getTokenType().compare("OPERATOR") == 0) { 
                cout << "\nRule: A -> A - At";
                tokenValueReader("-");
                At();
                treeBuilder("NEGATIVE", "neg", 1);
        }
        else { 
				cout << "\nRule: A -> At";
                At();
		}
        while((token->getTokenValue().compare("+") == 0 && token->getTokenType().compare("OPERATOR") == 0) || (token->getTokenValue().compare("-") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                tokenValueReader(token->getTokenValue());
                At();
                if(token->getTokenValue().compare("+") == 0 ) {
						cout << "\nRule: A -> + At";
                        treeBuilder("ADDITION", token->getTokenValue(), 2);
				}
                else {
						cout << "\nRule: A -> - At";
                        treeBuilder("SUBTRACTION", token->getTokenValue(), 2);
				}
        }
}

void Parser::At() {

        Af();
        while((token->getTokenValue().compare("*") == 0 && token->getTokenType().compare("OPERATOR") == 0) || (token->getTokenValue().compare("/") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
		tokenValueReader(token->getTokenValue());
                Af();
                if(token->getTokenValue().compare("*") == 0) {
                	cout << "\nRule: At -> At * Af";
                	treeBuilder("MULTIPLICATION", token->getTokenValue(), 2);
				}
                else {
					cout << "\nRule: At -> At / Af";
                	treeBuilder("DIVISION", token->getTokenValue(), 2);
				}
        }
        cout << "\nRule: At -> Af";
}

void Parser::Af() {

        Ap();
        if(token->getTokenValue().compare("**") == 0 && token->getTokenType().compare("OPERATOR") == 0)  {
                cout << "\nRule: Af -> Ap ** Af";
                tokenValueReader("**");
                Af();
                treeBuilder("EXPONENTIAL", "**", 2);
        }
        else {
				cout << "\nRule: Af -> Ap";
		}
}

void Parser::Ap() {
        
        R();
        while(token->getTokenValue().compare("@") == 0 && token->getTokenType().compare("OPERATOR") == 0){
                cout << "\nRule: Ap -> Ap @ <IDENTIFIER> R";
                tokenValueReader("@");
                tokenTypeReader("IDENTIFIER");
                R();
                treeBuilder("ATTHERATE", "@", 3);
        }
        cout << "\nRule: Ap -> R";
}

void Parser::R() {
        
        Rn();
        while((token->getTokenValue().compare("true") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare("false") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare("nil") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) || (token->getTokenValue().compare("dummy") == 0 && (token->getTokenType().compare("RESERVED") == 0)) || (token->getTokenType().compare("IDENTIFIER") == 0) || (token->getTokenType().compare("STRING") == 0) || (token->getTokenType().compare("INTEGER") == 0)) {
                cout << "\nRule: R -> R Rn";
                Rn();
                treeBuilder("GAMMA", "gamma", 2);
        }
        cout << "\nRule: R -> Rn";
}

void Parser::Rn() {

        if(token->getTokenType().compare("IDENTIFIER") == 0) {
				cout << "\nRule: Rn -> <IDENTIFIER>";
                tokenTypeReader("IDENTIFIER");
		}
        else if(token->getTokenType().compare("INTEGER") == 0) {
				cout << "\nRule: Rn -> <INTEGER>";
                tokenTypeReader("INTEGER");
		}
        else if(token->getTokenType().compare("STRING") == 0) {
				cout << "\nRule: Rn -> <STRING>";
                tokenTypeReader("STRING");
		}
        else if(token->getTokenValue().compare("true") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                cout << "\nRule: Rn -> true";
                tokenValueReader("true");
                treeBuilder("TRUE", "true", 0);
        }
        else if(token->getTokenValue().compare("false") == 0 && token->getTokenType().compare("RESERVED")) {
                cout << "\nRule: Rn -> false";
                tokenValueReader("false");
                treeBuilder("FALSE", "false", 0);
        }
        else if(token->getTokenValue().compare("nil") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                cout << "\nRule: Rn -> nil";
                tokenValueReader("nil");
                treeBuilder("NIL", "nil", 0);
        }
        else if(token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) {
                cout << "\nRule: Rn -> ( E )";
                tokenValueReader("(");
                E();
                tokenValueReader(")");
        }
        else {
				cout << "\nRule: Rn -> dummy";
                tokenValueReader("dummy");
                treeBuilder("DUMMY", "dummy", 0);
        }
}

void Parser::D() {

        Da();
        if(token->getTokenValue().compare("within") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                cout << "\nRule: D -> Da within D";
                tokenValueReader("within");
                D();
                treeBuilder("WITHIN", "within", 2);
        }
        cout << "\nRule: D -> Da";
}

void Parser::Da() {

        Dr();
        int i=0;
        while(token->getTokenValue().compare("and") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                cout << "\nRule: Da -> Dr ( and Dr ) +";
                i++;
                tokenValueReader("and");
                Dr();
        }
        if(i > 0)
                treeBuilder("AND", "and", i+1);
		else
				cout << "\nRule: Da -> Dr";
}

void Parser::Dr() {

        if(token->getTokenValue().compare("rec") == 0 && token->getTokenType().compare("RESERVED") == 0) {
				cout << "\nRule: Dr -> rec Db";
                tokenValueReader("rec");
                Db();
                treeBuilder("REC", "rec", 1);
        }
        else {
				cout << "\nRule: Dr -> Db";
                Db();
		}
}

void Parser::Db() {

        if(token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) { 
                cout << "\nRule: Db -> ( D )";
                tokenValueReader("(");
                D();
                tokenValueReader(")");
        }
        else if (token->getTokenType().compare("IDENTIFIER") == 0) { 
                tokenTypeReader("IDENTIFIER"); 
                if (token->getTokenValue().compare(",") == 0  && token->getTokenType().compare("PUNCTUATION") == 0 || (token->getTokenValue().compare("=") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                        cout << "\nRule: Db -> Vl = E";
                        Vl(); 
                        tokenValueReader("=");
                        E();
                        treeBuilder("EQUAL", "=", 2);

                }
                else {
						cout << "\nRule: Db -> <IDENTIFIER> Vb+ = E";
                        int i = 0;
                        while((token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) || token->getTokenType().compare("IDENTIFIER") == 0) {
                                i++;
                                Vb();
                        }
                        tokenValueReader("=");
                        E();
                        treeBuilder("FCN_FORM", "fcn_form", i+2);
                }
        }
}

void Parser::Vb() {
	
       	if(token->getTokenType().compare("IDENTIFIER") == 0 ) { 
				cout << "\nRule: Vb -> <IDENTIFIER>";
                tokenTypeReader("IDENTIFIER");
        }
        else { 
                tokenValueReader("(");
                if(token->getTokenValue().compare(")") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) {
                        cout << "\nRule: Vb -> ( )";
                        tokenValueReader("(");
                        treeBuilder("PARANTHESIS", "()", 2);
                }
                else{ 
						cout << "\nRule: Vb -> ( Vl )";
                        tokenTypeReader("IDENTIFIER");
                        Vl();
                        tokenValueReader(")");
                }
        }
}

void Parser::Vl() {

	cout << "\nRule: Vl -> <IDENTIFIER> list ,";
	int i = 1;
        while(token->getTokenValue().compare(",") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) { 
                i++;
                tokenValueReader(",");
                tokenTypeReader("IDENTIFIER");
        }
        if(i > 1)
                treeBuilder("COMMA", ",", i);
}

void Parser::tokenValueReader(string value) {
	
	if(token->getTokenValue().compare(value) != 0) {
		cout << "\nError --- Wrong Token Found! Expected " << value << " but found " << token->getTokenValue();
		cout << "\nExiting ...";
		exit(0);
	}
	cout << "\nToken Value: " << token->getTokenValue();
	delete token;
	token = NULL;
	token = lexer();
}

void Parser::tokenTypeReader(string type) {

	if(token->getTokenType().compare(type) != 0) {
		cout << "\nError --- Wrong Token Found! Expected " << type << " but found " << token->getTokenType();
                cout << "\nExiting ...";
                exit(0);
        }
	else {
		if(token->getTokenType().compare("IDENTIFIER") == 0)
			treeBuilder("IDENTIFIER", token->getTokenValue(), 0);
		else if(token->getTokenType().compare("INTEGER") == 0)
			treeBuilder("INTEGER", token->getTokenValue(), 0);
		else if(token->getTokenType().compare("STRING") == 0)
			treeBuilder("STRING", token->getTokenValue(), 0);		
	}
	cout << "\nToken Type: " << token->getTokenType();
	delete token;
	token = NULL;
        token = lexer();
}

void Parser::treeBuilder(string node_type, string node_value, int node_num) {

	treeNode *node = new treeNode(node_type, node_value);
	while(!nodeList.empty()) {
		treeNode *temp = nodeList.top();
		nodeList.pop();
		for(int i=0; i < node_num-1; i++) {
			treeNode *topNode = nodeList.top();
			nodeList.pop();
			topNode->right = temp;
			temp = topNode;
		}
		node->left = temp;
	}
	nodeList.push(node);		
	//cout << "\nBuilding Tree node for type: " << node_type << " and value: " << node_value;
}

/* Following the grammar rules 
mentioned in lexer.pdf */

std::vector<string> RESERVED_LIST = { "let", "in", "fn", "where", "aug", "or", "not", "gr", "ge", "ls", "le", "eq", "ne", "true", "false", "nil", "dummy", "within", "and", "rec" };

Token* Parser::lexer() {

	char ch;
	Token *tok;
	if(last_scanned_char == ' ')
		ch = inputfilestream.get();
	else
		ch = last_scanned_char;
	string tokenbuilder = "";
	
	while(ch == ' ' || ch == '\t' || ch == '\n') {
		ch = inputfilestream.get();
	}

	if(isalpha(ch)) {
		tokenbuilder += ch;
		ch = inputfilestream.get();
		while(ch == '_' || isalpha(ch) || isdigit(ch)) {
			tokenbuilder += ch;
			ch = inputfilestream.get();
		}
		last_scanned_char = ch;
		if(std::find(RESERVED_LIST.begin(), RESERVED_LIST.end(), tokenbuilder) != RESERVED_LIST.end())
			tok = new Token(tokenbuilder, "RESERVED");
		else
			tok = new Token(tokenbuilder, "IDENTIFIER");
		return tok;
	}
	else if(isdigit(ch)) {
		tokenbuilder += ch;
                ch = inputfilestream.get();
                while(isdigit(ch)) {
                        tokenbuilder += ch;
                        ch = inputfilestream.get();
                }
                last_scanned_char = ch;
		tok = new Token(tokenbuilder, "INTEGER");
		return tok;
	}
	else if(ch == '+' || ch == '.' || ch == '&' || ch == '@' || ch == '=' || ch == ':' || ch == '~' || ch == '|' || ch == '$' || ch == '!' || ch == '#' || ch == '%' || ch == '^' || ch == '_' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '"' || ch == '?') {
		last_scanned_char == ' ';
		tokenbuilder += ch;
		tok = new Token(tokenbuilder, "OPERATOR");
		return tok;
	}
	else if(ch == '>' || ch == '<') {
		tokenbuilder += ch;
		ch = inputfilestream.get();
		if(ch == '=') {
			last_scanned_char = ' ';
			tokenbuilder += ch;
			tok = new Token(tokenbuilder, "OPERATOR");
			return tok;
		}
		else {
			last_scanned_char = ch;
			tok = new Token(tokenbuilder, "OPERATOR");
			return tok;
		}
	}
	else if(ch == '*') {
                tokenbuilder += ch;
                ch = inputfilestream.get();
                if(ch == '*') {
                        last_scanned_char = ' ';
                        tokenbuilder += ch;
                        tok = new Token(tokenbuilder, "OPERATOR");
                        return tok;
                }
                else {
                        last_scanned_char = ch;
                        tok = new Token(tokenbuilder, "OPERATOR");
                        return tok;
                }
        }
	else if(ch == '-') {
                tokenbuilder += ch;
                ch = inputfilestream.get();
                if(ch == '>') {
                        last_scanned_char = ' ';
                        tokenbuilder += ch;
                        tok = new Token(tokenbuilder, "OPERATOR");
                        return tok;
                }
                else {
                        last_scanned_char = ch;
                        tok = new Token(tokenbuilder, "OPERATOR");
                        return tok;
                }
        }
	else if(ch == '/') {
		tokenbuilder += ch;
		ch = inputfilestream.get();
		if(ch == '/') {
			while(ch != '\n') {
				ch = inputfilestream.get();
				if(ch == EOF) {
					tok = new Token("", "ENDOFFILE");
					return tok;
				}
			}
			tok = new Token("", "DELETE");
			return tok;
		}
		else {
			last_scanned_char = ch;
			tok = new Token(tokenbuilder, "OPERATOR");
			return tok;
		}
	}
	else if(ch == '(' || ch == ')' || ch == ';' || ch == ',') {
		tokenbuilder += ch;
		last_scanned_char = ' ';
		tok = new Token(tokenbuilder, "PUNCTUATION");
		return tok;
	}
	else if(ch == EOF) {
		tok = new Token("", "ENDOFFILE");
		return tok;
	}
}
