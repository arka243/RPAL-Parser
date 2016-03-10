#include "parser.h"

/* Following grammar rules
mentioned in rpal.grammar.pdf */

Parser:Parser(string filepath) {

	lexicon = new Lexer();
	inputfilestream.open(filepath.c_str(), ifstream::in);
	if(!inputfilestream) {
		cout << "\nError...Could not open file!"
		cout << "\nExiting ..."
		exit(0);
	}
	token = lexicon.lexer();
}

void Parser:E() {

	if(token.getTokenValue().compare("let") == 0 && token.getTokenType().compare("RESERVED") == 0) {
		tokenReader("let");
		D();
		tokenReader("in");
		E();
		treeBuilder("LET", "let", 2);
	}
	else if(token.getTokenValue().compare("fn") == 0 && token.getTokenType().compare("RESERVED") == 0) {
		tokenReader("fn");
		int i = 0;
		while((token.getTokenValue().compare("(") == 0 && token.getTokenType().compare("PUNCTUATION") == 0) || token.getTokenType().compare("IDENTIFIER")) {
			i++;
			Vb();
		}
		tokenReader(".");
		E();
		treeBuilder("LAMBDA", "lambda", i+1);
	}
	else
		Ew();
}

void Parser:Ew() {

	T();
	if(token.getTokenValue().compare("where") == 0 && token.getTokenType().compare("RESERVED") == 0) {
		tokenReader("where");
		Dr();
		treeBuilder("WHERE", "where", 2);
	}
}

void Parser::T() {

        Ta();
        int i = 0;
        while(token.getTokenValue().compare(",") == 0 && token.getTokenType().compare("PUNCTUATION") == 0){
                i++; 
                tokenReader(",");
                Ta();
        }
        if(i > 0)
                treeBuilder("TAU", "tau", i+1);
}

void Parser::Ta() {

        Tc();
        while( (token.getTokenValue()).compare("aug") == 0 && token.getTokenType.compare("RESERVED") == 0{ 
                tokenReader("aug");
                Tc();
                treeBuilder("AUG", "aug", 2);
        }
}

void Parser::Tc() {

        B();
        if(token.getTokenValue().compare("->") == 0 && token.getTokenType().compare("OPERATOR") == 0) { 
                tokenReader("->");
                Tc();
                tokenReader("|");
                Tc();
                treeBuilder("COND", "->", 3);
        }
}

void Parser::B() {

        Bt();
        while(token.getTokenValue().compare("or") == 0 && token.getTokenType().compare("RESERVED") == 0) { 
                tokenReader("or");
                Bt();
                treeBuilder("OR", "or", 2);
        }
}

void Parser::Bt() {

        Bs();
        while(token.getTokenValue().compare("&") == 0 && token.getTokenType().compare("OPERATOR") == 0) { 
                tokenReader("&");
                Bs();
                treeBuilder("AMPERSAND", "&", 2);
        }
}

void Parser::Bs() {

        if(token.getTokenValue().compare("not") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                tokenReader("not");
                Bp();
                treeBuilder("NOT", "not", 1);
        }
        else
                Bp();
}

void Parser::Bp() {

        A();
        if( (token.getTokenValue().compare("gr") == 0 && token.getTokenType().compare("RESERVED") == 0) || (token.getTokenValue().compare(">") == 0 && token.getTokenType().compare("OPERATOR") == 0)) {
                tokenReader(token.getTokenValue());
                A();
                treeBuilder("GREATER", "gr", 2);
        }
        else if((token.getTokenValue().compare("ge") == 0 && token.getTokenType().compare("RESERVED") == 0) || (token.getTokenValue().compare(">=") == 0 && token.getTokenType().compare("OPERATOR") == 0)) {
                tokenReader(token.getTokenValue());
                A();
                treeBuilder("GREATEREQUAL", "ge", 2);
        }
        else if((token.getTokenValue().compare("ls") == 0 && token.getTokenType().compare("RESERVED") == 0 ) || (token.getTokenValue().compare("<") == 0 && token.getTokenType().compare("OPERATOR" == 0)) {
                tokenReader(token.getTokenValue());
                A();
                treeBuilder("LESS", "ls", 2);
        }
        else if((token.getTokenValue().compare("le") == 0 && token.getTokenType().compare("RESERVED") == 0) || (token.getTokenValue().compare("<=") == 0 && token.getTokenType().compare("OPERATOR") == 0)) {
                tokenReader(token.getTokenValue());
                A();
                treeBuilder("LESSEQUAL", "le", 2);
        }
        else if(token.getTokenValue().compare("eq") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                tokenReader("eq");
                A();
                treeBuilder("EQUAL", "eq", 2);
        }
        else if(token.getTokenValue().compare("ne") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                tokenReader("ne");
                A();
                treeBuilder("NOTEQUAL", "ne", 2);
        }
}

void Parser::A() {

        if(token.getTokenValue().compare("+") == 0 && token.getTokenType().compare("OPERATOR") == 0) { 
                tokenReader("+");
                At();
        }
        else if(token.getTokenValue().compare("-") == 0 && token.getTokenType().compare("OPERATOR") == 0) { 
                tokenReader("-");
                At();
                treeBuilder("NEGATIVE", "neg", 1);
        }
        else 
                At();
        while((token.getTokenValue().compare("+") == 0 && token.getTokenType().compare("OPERATOR") == 0) || (token.getTokenValue().compare("-") == 0 && token.getTokenType().compare("OPERATOR" == 0)) {
                tokenReader(token.getTokenValue());
                At();
                if(temp_string.compare("+") == 0 )
                        treeBuilder("ADDITION", token.getTokenValue(), 2);
                else
                        treeBuilder("SUBTRACTION", token.getTokenValue(), 2);
        }
}

void Parser::At() {

        Af();
        while((token.getTokenValue().compare("*") == 0 && token.getTokenType().compare("OPERATOR") == 0) || (token.getTokenValue().compare("/") == 0 && token.getTokenType().compare("OPERATOR") == 0) {
		tokenReader(token.getTokenValue());
                Af();
                if( temp_string.compare("*") == 0)
                	treeBuilder("MULTIPLICATION", token.getTokenValue(), 2);
                else
                	treeBuilder("DIVISION", token.getTokenValue(), 2);
        }
}

void Parser::Af() {

        Ap();
        if(token.getTokenValue().compare("**") == 0 && token.getTokenType().compare("OPERATOR") == 0)  {
                tokenReader("**");
                Af();
                treeBuilder("EXPONENTIAL", "**", 2);
        }
}

void Parser::Ap() {
        R();
        while(token.getTokenValue().compare("@") == 0 && token.getTokenType().compare("OPERATOR") == 0){
                tokenReader("@");
                tokenReader("IDENTIFIER");
                R();
                treeBuilder("ATTHERATE", "@", 3);
        }
}

void Parser::R() {
        Rn();
        while((token.getTokenValue().compare("true") == 0 && token.getTokenType().compare("RESERVED") == 0) || (token.getTokenValue().compare("false") == 0 && token.getTokenType().compare("RESERVED") == 0) || (token.getTokenValue().compare("nil") == 0 && token.getTokenType().compare("RESERVED") == 0) || (token.getTokenValue().compare("(") == 0 && token.getTokenType().compare("PUNCTUATION") == 0) || (token.getTokenValue().compare("dummy") == 0 && (token.getTokenType().compare("RESERVED") == 0)) || (token.getTokenType().compare("IDENTIFIER") == 0) || (token.getTokenType().compare("STRING") == 0) || (token.getTokenType().compare("INTEGER") == 0)) {
                Rn();
                treeBuilder("GAMMA", "gamma", 2);
        }
}

void Parser::Rn() {

        if(token.getTokenType().compare("IDENTIFIER") == 0)
                tokenReader("IDENTIFIER");
        else if(token.getTokenType().compare("INTEGER") == 0)
                tokenReader("INTEGER");
        else if(token.getTokenType().compare("STRING") == 0)
                tokenReader("STRING");
        else if(token.getTokenValue().compare("true") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                tokenReader("true");
                treeBuilder("TRUE", "true", 0);
        }
        else if(token.getTokenValue().compare("false") == 0 && token.getTokenType().compare("RESERVED")) {
                tokenReader("false");
                treeBuilder("FALSE", "false", 0);
        }
        else if(token.getTokenValue().compare("nil") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                tokenReader("nil");
                treeBuilder("NIL", "nil", 0);
        }
        else if(token.getTokenValue().compare("(") == 0 && token.getTokenType().compare("PUNCTUATION") == 0) {
                tokenReader("(");
                E();
                tokenReader(")");
        }
        else {
                tokenReader("dummy");
                treeBuilder("DUMMY", "dummy", 0);
        }
}

void Parser::D() {

        Da();
        if(token.getTokenValue().compare("within") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                tokenReader("within");
                D();
                treeBuilder("WITHIN", "within", 2);
        }
}

void Parser::Da() {

        Dr();
        int i=0;
        while(token.getTokenValue().compare("and") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                i++;
                tokenReader("and");
                Dr();
        }
        if(i > 0)
                treeBuilder("AND", "and", i+1);
}

void Parser::Dr() {

        if(token.getTokenValue().compare("rec") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                tokenReader("rec");
                Db();
                treeBuilder("REC", "rec", 1);
        }
        else
                Db();
}

void Parser::Db() {

        if(token.getTokenValue()).compare("(") == 0 && token.getTokenType().compare("PUNCTUATION") == 0) { 
                tokenReader("(");
                D();
                tokenReader(")");
        }
        else if (token.getTokenType().compare("IDENTIFIER") == 0) { 
                tokenReader(IDENTIFIER); 
                if (token.getTokenValue().compare(",") == 0  && token.getTokenType().compare("PUNCTUATION") == 0 || (token.getTokenValue().compare("=") == 0 && token.getTokenType().compare("OPERATOR") == 0)) {
                        Vl(); 
                        tokenReader("=");
                        E();
                        treeBuilder("EQUAL", "=", 2);

                }
                else {
                        int i = 0;
                        while((token.getTokenValue().compare("(") == 0 && token.getTokenType().compare("PUNCTUATION") == 0) || token.getTokenType().compare("IDENTIFIER") == 0) {
                                i++;
                                Vb();
                        }
                        tokenReader("=");
                        E();
                        treeBuilder("FCN_FORM", "fcn_form", i+2)
                }
        }
}

void Parser::Vb() {
       	if( token.getTokenType().compare("IDENTIFIER") == 0 ) { 
                tokenReader("IDENTIFIER");
        }
        else { 
                tokenReader("(");
                if(token.getTokenValue().compare(")") == 0 && token.getTokenType().compare("PUNCTUATION") == 0) {
                        tokenReader("(");
                        treeBuilder("PARANTHESIS", "()", 2);
                }
                else{ 
                        tokenReader("IDENTIFIER");
                        Vl();
                        tokenReader(")");
                }
        }
}

void Parser::Vl() {

	int i = 1;
        while(token.getTokenValue().compare(",") == 0 && token.getTokenType.compare("PUNCTUATION") == 0) { 
                i++;
                tokenReader(",");
                tokenReader("IDENTIFIER");
        }
        if(i > 1)
                treeBuilder("COMMA", ",", i);
}
