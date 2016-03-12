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

	E();
	if(token->getTokenType().compare("ENDOFFILE") != 0)
		cout << "\nError --- End of File expected after lexical analysis!";
	if(!nodeList.empty()) {
		rootNode = nodeList.top();
		if(exec_mode.compare("ast") == 0) {
			treeTraversal(rootNode, 0);
		}
	}
}

/* Following grammar rules
mentioned in rpal.grammar.pdf */

void Parser::E() {

	if(token->getTokenValue().compare("let") == 0 && token->getTokenType().compare("RESERVED") == 0) {
		tokenValueReader("let");
		D();
		tokenValueReader("in");
		E();
		treeBuilder("LET", "let", 2);
	}
	else if(token->getTokenValue().compare("fn") == 0 && token->getTokenType().compare("RESERVED") == 0) {
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
	else 
		Ew();
}

void Parser::Ew() {

	T();
	if(token->getTokenValue().compare("where") == 0 && token->getTokenType().compare("RESERVED") == 0) {
		tokenValueReader("where");
		Dr();
		treeBuilder("WHERE", "where", 2);
	}
}

void Parser::T() {

	Ta();
        int i = 0;
        while(token->getTokenValue().compare(",") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) {
                i++; 
                tokenValueReader(",");
                Ta();
        } 
        if(i > 0) {
                treeBuilder("TAU", "tau", i+1);
	}
}

void Parser::Ta() {

	Tc();
        while(token->getTokenValue().compare("aug") == 0 && token->getTokenType().compare("RESERVED") == 0) { 
                tokenValueReader("aug");
                Tc();
                treeBuilder("AUG", "aug", 2);
        }
}

void Parser::Tc() {

	B();
        if(token->getTokenValue().compare("->") == 0 && token->getTokenType().compare("OPERATOR") == 0) { 
                tokenValueReader("->");
                Tc();
                tokenValueReader("|");
                Tc();
                treeBuilder("CONDITION", "->", 3);
        }
}

void Parser::B() {

	Bt();
        while(token->getTokenValue().compare("or") == 0 && token->getTokenType().compare("RESERVED") == 0) { 
                tokenValueReader("or");
                Bt();
                treeBuilder("OR", "or", 2);
        }
}

void Parser::Bt() {

        Bs();
        while(token->getTokenValue().compare("&") == 0 && token->getTokenType().compare("OPERATOR") == 0) { 
                tokenValueReader("&");
                Bs();
                treeBuilder("AMPERSAND", "&", 2);
        }
}

void Parser::Bs() {

        if(token->getTokenValue().compare("not") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("not");
                Bp();
                treeBuilder("NOT", "not", 1);
        }
        else {
                Bp();
	}
}

void Parser::Bp() {

	A();
        if((token->getTokenValue().compare("gr") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare(">") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                tokenValueReader(token->getTokenValue());
                A();
                treeBuilder("GREATER", "gr", 2);
        }
        else if((token->getTokenValue().compare("ge") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare(">=") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                tokenValueReader(token->getTokenValue());
                A();
                treeBuilder("GREATEREQUAL", "ge", 2);
        }
        else if((token->getTokenValue().compare("ls") == 0 && token->getTokenType().compare("RESERVED") == 0 ) || (token->getTokenValue().compare("<") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                tokenValueReader(token->getTokenValue());
                A();
                treeBuilder("LESS", "ls", 2);
        }
        else if((token->getTokenValue().compare("le") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare("<=") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                tokenValueReader(token->getTokenValue());
                A();
                treeBuilder("LESSEQUAL", "le", 2);
        }
        else if(token->getTokenValue().compare("eq") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("eq");
                A();
                treeBuilder("EQUAL", "eq", 2);
        }
        else if(token->getTokenValue().compare("ne") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("ne");
                A();
                treeBuilder("NOTEQUAL", "ne", 2);
        }
}

void Parser::A() {

        if(token->getTokenValue().compare("+") == 0 && token->getTokenType().compare("OPERATOR") == 0) { 
                tokenValueReader("+");
                At();
        }
        else if(token->getTokenValue().compare("-") == 0 && token->getTokenType().compare("OPERATOR") == 0) { 
                tokenValueReader("-");
                At();
                treeBuilder("NEGATIVE", "neg", 1);
        }
        else { 
                At();
	}
        while((token->getTokenValue().compare("+") == 0 && token->getTokenType().compare("OPERATOR") == 0) || (token->getTokenValue().compare("-") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                tokenValueReader(token->getTokenValue());
                At();
                if(token->getTokenValue().compare("+") == 0 ) {
                        treeBuilder("ADDITION", token->getTokenValue(), 2);
		}
                else {
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
                	treeBuilder("MULTIPLICATION", token->getTokenValue(), 2);
		}
                else {
                	treeBuilder("DIVISION", token->getTokenValue(), 2);
		}
        }
}

void Parser::Af() {

        Ap();
        if(token->getTokenValue().compare("**") == 0 && token->getTokenType().compare("OPERATOR") == 0)  {
                tokenValueReader("**");
                Af();
                treeBuilder("EXPONENTIAL", "**", 2);
        }
}

void Parser::Ap() {
        
        R();
        while(token->getTokenValue().compare("@") == 0 && token->getTokenType().compare("OPERATOR") == 0){
                tokenValueReader("@");
                tokenTypeReader("IDENTIFIER");
                R();
                treeBuilder("ATTHERATE", "@", 3);
        }
}

void Parser::R() {
        
        Rn();
        while((token->getTokenValue().compare("true") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare("false") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare("nil") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) || (token->getTokenValue().compare("dummy") == 0 && (token->getTokenType().compare("RESERVED") == 0)) || (token->getTokenType().compare("IDENTIFIER") == 0) || (token->getTokenType().compare("STRING") == 0) || (token->getTokenType().compare("INTEGER") == 0)) {
                Rn();
                treeBuilder("GAMMA", "gamma", 2);
        }
}

void Parser::Rn() {

        if(token->getTokenType().compare("IDENTIFIER") == 0) {
                tokenTypeReader("IDENTIFIER");
	}
        else if(token->getTokenType().compare("INTEGER") == 0) {
                tokenTypeReader("INTEGER");
	}
        else if(token->getTokenType().compare("STRING") == 0) {
				cout << "\nRule: Rn -> <STRING>";
                tokenTypeReader("STRING");
	}
        else if(token->getTokenValue().compare("true") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("true");
                treeBuilder("TRUE", "true", 0);
        }
        else if(token->getTokenValue().compare("false") == 0 && token->getTokenType().compare("RESERVED")) {
                tokenValueReader("false");
                treeBuilder("FALSE", "false", 0);
        }
        else if(token->getTokenValue().compare("nil") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("nil");
                treeBuilder("NIL", "nil", 0);
        }
        else if(token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) {
                tokenValueReader("(");
                E();
                tokenValueReader(")");
        }
        else {
                tokenValueReader("dummy");
                treeBuilder("DUMMY", "dummy", 0);
        }
}

void Parser::D() {

        Da();
        if(token->getTokenValue().compare("within") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("within");
                D();
                treeBuilder("WITHIN", "within", 2);
        }
}

void Parser::Da() {

        Dr();
        int i=0;
        while(token->getTokenValue().compare("and") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                i++;
                tokenValueReader("and");
                Dr();
        }
        if(i > 0)
                treeBuilder("AND", "and", i+1);
}

void Parser::Dr() {

        if(token->getTokenValue().compare("rec") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("rec");
                Db();
                treeBuilder("REC", "rec", 1);
        }
        else {
                Db();
	}
}

void Parser::Db() {

        if(token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) { 
                tokenValueReader("(");
                D();
                tokenValueReader(")");
        }
        else if (token->getTokenType().compare("IDENTIFIER") == 0) { 
                tokenTypeReader("IDENTIFIER"); 
                if (token->getTokenValue().compare(",") == 0  && token->getTokenType().compare("PUNCTUATION") == 0 || (token->getTokenValue().compare("=") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                        Vl(); 
                        tokenValueReader("=");
                        E();
                        treeBuilder("EQUAL", "=", 2);
                }
                else {
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
                tokenTypeReader("IDENTIFIER");
        }
        else { 
                tokenValueReader("(");
                if(token->getTokenValue().compare(")") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) {
                        tokenValueReader("(");
                        treeBuilder("PARANTHESIS", "()", 2);
                }
                else{ 
                        tokenTypeReader("IDENTIFIER");
                        Vl();
                        tokenValueReader(")");
                }
        }
}

void Parser::Vl() {

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
        token = lexer();
}

void Parser::treeBuilder(string node_type, string node_value, int node_num) {

	treeNode *node = new treeNode(node_value, node_type);
	while(!nodeList.empty()) {
		treeNode *temp = nodeList.top();
		nodeList.pop();
		for(int i=0; i < node_num-1; i++) {
			while(!nodeList.empty()) {
				treeNode *topNode = nodeList.top();
				nodeList.pop();
				topNode->right = temp;
				temp = topNode;
			}
		}
		node->left = temp;
	}
	nodeList.push(node);
}

void Parser::treeTraversal(treeNode *root, int dots) {

	treeNode *temp = root;
	if(root != NULL) {
		for(int i=1; i<= dots; i++)
			cout << ".";
		if(root->getNodeType() == "STRING")
			cout << "<STRING:'" << root->getNodeValue() << "'>" << endl;
		else if(root->getNodeType() == "IDENTIFIER")
                        cout << "<IDENTIFIER:" << root->getNodeValue() << ">" << endl;
		else if(root->getNodeType() == "INTEGER")
                        cout << "<INTEGER:" << root->getNodeValue() << ">" << endl;
		else
			cout << root->getNodeValue() << endl;
		if(temp->left != NULL)
			treeTraversal(temp->left, dots+1);
		if(temp->right != NULL)
			treeTraversal(temp->right, dots);
	}
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
		if(std::find(RESERVED_LIST.begin(), RESERVED_LIST.end(), tokenbuilder) != RESERVED_LIST.end()) {
			tok = new Token(tokenbuilder, "RESERVED");
		}
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
	else if(ch == '\'') {
		ch = inputfilestream.get();
		while(ch != '\'') {
			if(ch == '\\') {
				char prev = ch;
				ch = inputfilestream.get();
				if(ch != '\'')
					tokenbuilder += prev;
				else {
					tokenbuilder += ch;
					ch = inputfilestream.get();
				}
			}
			else {
				tokenbuilder += ch;
				ch = inputfilestream.get();
			}
			if(ch == EOF) {
				cout << "\nError Scanning Strings!";
				cout << "\nExiting ..." << endl;
				exit(0);
			}
		}
		last_scanned_char = ' ';
		tok = new Token(tokenbuilder, "STRING");
		return tok;
	}
	else if(ch == EOF) {
		tok = new Token("", "ENDOFFILE");
		return tok;
	}
}
