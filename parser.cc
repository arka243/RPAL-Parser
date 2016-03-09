#include "parser.h"

void Parser:E() {

	if(token.getTokenValue().compare("let") == 0 && token.getTokenType().compare("RESERVED") == 0) {
		readToken("let");
		D();
		readToken("in");
		E();
		buildTree("LET", "let", 2);
	}
	else if(token.getTokenValue().compare("fn") == 0 && token.getTokenType().compare("RESERVED") == 0) {
		readToken("fn");
		int i = 0;
		while((token.getTokenValue().compare("(") == 0 && token.getTokenType().compare("PUNCTUATION") == 0) || token.getTokenType().compare("IDENTIFIER")) {
			i++;
			Vb();
		}
		readToken(".");
		E();
		buildTree("LAMBDA", "lambda", i+1);
	}
	else
		Ew();
}

void Parser:Ew() {

	T();
	if(token.getTokenValue().compare("where") == 0 && token.getTokenType().compare("RESERVED") == 0) {
		readToken("where");
		Dr();
		buildTree("WHERE", "where", 2);
	}
}

void Parser::T() {

        Ta();
        int i = 0;
        while(token.getTokenValue().compare(",") == 0 && token.getTokenType().compare("PUNCTUATION") == 0){
                i++; 
                readToken(",");
                Ta();
        }
        if(i > 0)
                buildTree("TAU", "tau", i+1);
}

void Parser::Ta() {

        Tc();
        while( (token.getTokenValue()).compare("aug") == 0 && token.getTokenType.compare("RESERVED") == 0{ 
                readToken("aug");
                Tc();
                buildTree("AUG", "aug", 2);
        }
}

void Parser::Tc() {

        B();
        if(token.getTokenValue().compare("->") == 0 && token.getTokenType().compare("OPERATOR") == 0) { 
                readToken("->");
                Tc();
                readToken("|");
                Tc();
                buildTree("COND", "->", 3);
        }
}

void Parser::B() {

        Bt();
        while(token.getTokenValue().compare("or") == 0 && token.getTokenType().compare("RESERVED") == 0) { 
                readToken("or");
                Bt();
                buildTree("OR", "or", 2);
        }
}

void Parser::Bt() {

        Bs();
        while(token.getTokenValue().compare("&") == 0 && token.getTokenType().compare("OPERATOR") == 0) { 
                readToken("&");
                Bs();
                buildTree("AMP", "&", 2);
        }
}

void Parser::Bs() {

        if(token.getTokenValue().compare("not") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                readToken("not");
                Bp();
                buildTree("NOT", "not", 1);
        }
        else
                Bp();
}

void Parser::Bp() {

        A();
        if( (token.getTokenValue().compare("gr") == 0 && token.getTokenType().compare("RESERVED") == 0) || (token.getTokenValue().compare(">") == 0 && token.getTokenType().compare("OPERATOR") == 0)) {
                readToken(token.getTokenValue());
                A();
                buildTree("GR", "gr", 2);
        }
        else if((token.getTokenValue().compare("ge") == 0 && token.getTokenType().compare("RESERVED") == 0) || (token.getTokenValue().compare(">=") == 0 && token.getTokenType().compare("OPERATOR") == 0)) {
                readToken(token.getTokenValue());
                A();
                buildTree("GE", "ge", 2);
        }
        else if((token.getTokenValue().compare("ls") == 0 && token.getTokenType().compare("RESERVED") == 0 ) || (token.getTokenValue().compare("<") == 0 && token.getTokenType().compare("OPERATOR" == 0)) {
                readToken(token.getTokenValue());
                A();
                buildTree("LS", "ls", 2);
        }
        else if((token.getTokenValue().compare("le") == 0 && token.getTokenType().compare("RESERVED") == 0) || (token.getTokenValue().compare("<=") == 0 && token.getTokenType().compare("OPERATOR") == 0)) {
                readToken(token.getTokenValue());
                A();
                buildTree("LE", "le", 2);
        }
        else if(token.getTokenValue().compare("eq") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                readToken("eq");
                A();
                buildTree("EQ", "eq", 2);
        }
        else if(token.getTokenValue().compare("ne") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                readToken("ne");
                A();
                buildTree("NE", "ne", 2);
        }
}

void Parser::A() {

        if(token.getTokenValue().compare("+") == 0 && token.getTokenType().compare("OPERATOR") == 0) { 
                readToken("+");
                At();
        }
        else if(token.getTokenValue().compare("-") == 0 && token.getTokenType().compare("OPERATOR") == 0) { 
                readToken("-");
                At();
                buildTree("NEG", "neg", 1);
        }
        else 
                At();
        while((token.getTokenValue().compare("+") == 0 && token.getTokenType().compare("OPERATOR") == 0) || (token.getTokenValue().compare("-") == 0 && token.getTokenType().compare("OPERATOR" == 0)) {
                readToken(token.getTokenValue());
                At();
                if(temp_string.compare("+") == 0 )
                        buildTree("PLUS", token.getTokenValue(), 2);
                else
                        buildTree("MINUS", token.getTokenValue(), 2);
        }
}

void Parser::At() {

        Af();
        while( ((token.getTokenValue()).compare("*") == 0 && token.getTokenType() == OPERATOR )
                        || ( (token.getTokenValue()).compare("/") == 0 && token.getTokenType() == OPERATOR) ) {
                        string temp_string = token.getTokenValue();
                        readToken(temp_string);
                        Af();
                        if( temp_string.compare("*") == 0)
                                buildTree(MULTI, temp_string, 2);
                        else
                                buildTree(DIV, temp_string, 2);
        }
}

void Parser::Af() {

        Ap();
        if( (token.getTokenValue()).compare("**") == 0 && token.getTokenType() == OPERATOR ){
                readToken("**");
                Af();
                buildTree(EXPO, "**", 2);
        }

}

void Parser::Ap() {
        R();

        while((token.getTokenValue()).compare("@") == 0 && token.getTokenType() == OPERATOR ){
                readToken("@");
                readToken(IDENTIFIER);
                R();
                buildTree(AT, "@", 3);
        }

}

void Parser::R() {
        Rn();
        while( ((token.getTokenValue()).compare("true") == 0 && token.getTokenType() == RESERVED )
                        || ((token.getTokenValue()).compare("false") == 0 && token.getTokenType() == RESERVED )
                        || ((token.getTokenValue()).compare("nil") == 0 && token.getTokenType() == RESERVED )
                        || ((token.getTokenValue()).compare("(") == 0 && token.getTokenType() == PUNCTUATION)
                        || ((token.getTokenValue()).compare("dummy") == 0 && token.getTokenType() == RESERVED)
                        || token.getTokenType() == IDENTIFIER
                        || token.getTokenType() == STRING
                        || token.getTokenType() == INTEGER

                        ) {
                Rn();
                buildTree(GAMMA, "gamma", 2);
        }


}

void Parser::Rn() {

        if(token.getTokenType().compare("IDENTIFIER") == 0)
                readToken("IDENTIFIER");

        else if(token.getTokenType().compare("INTEGER") == 0)
                readToken("INTEGER");

        else if(token.getTokenType().compare("STRING") == 0)
                readToken("STRING");

        else if(token.getTokenValue().compare("true") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                readToken("true");
                buildTree("TRUE", "true", 0);
        }

        else if(token.getTokenValue().compare("false") == 0 && token.getTokenType().compare("RESERVED")) {
                readToken("false");
                buildTree("FALSE", "false", 0);
        }

        else if(token.getTokenValue().compare("nil") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                readToken("nil");
                buildTree("NIL", "nil", 0);
        }

        else if(token.getTokenValue().compare("(") == 0 && token.getTokenType().compare("PUNCTUATION") == 0) {
                readToken("(");
                E();
                readToken(")");
        }

        else {
                readToken("dummy");
                buildTree("DUMMY", "dummy", 0);
        }
}

void Parser::D() {

        Da();

        if( token.getTokenValue().compare("within") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                readToken("within");
                D();
                buildTree("WITHIN", "within", 2);
        }
}

void Parser::Da() {

        Dr();
        int i=0;

        while( token.getTokenValue().compare("and") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                i++;
                readToken("and");
                Dr();
        }

        if(i > 0)
                buildTree("AND", "and", i+1);
}

void Parser::Dr() {

        if( token.getTokenValue().compare("rec") == 0 && token.getTokenType().compare("RESERVED") == 0) {
                readToken("rec");
                Db();
                buildTree("REC", "rec", 1);
        }

        else
                Db();
}

void Parser::Db() {

        if(token.getTokenValue()).compare("(") == 0 && token.getTokenType().compare("PUNCTUATION") == 0) { 
                readToken("(");
                D();
                readToken(")");
        }

        else if (token.getTokenType().compare("IDENTIFIER") == 0) { 
                readToken(IDENTIFIER); 

                if ((token.getTokenValue().compare(",") == 0  && token.getTokenType().compare("PUNCTUATION") == 0) || (token.getTokenValue().compare("=") == 0 && token.getTokenType().compare("OPERATOR" == 0)) {
                        Vl(); 
                        readToken("=");
                        E();

                        buildTree(EQUAL, "=", 2);

                }

                else {
                        int i = 0;
                        while((token.getTokenValue().compare("(") == 0 && token.getTokenType().compare("PUNCTUATION") == 0) || token.getTokenType().compare("IDENTIFIER") == 0) {
                                i++;
                                Vb();
                        }

                        readToken("=");
                        E();
                        buildTree("FCN_FORM", "fcn_form", i+2)
                }
        }
}

void Parser::Vb() {
       	if( token.getTokenType().compare("IDENTIFIER") == 0 ) { 
                readToken("IDENTIFIER");
        }

        else { 
                readToken("(");

                if(token.getTokenValue().compare(")") == 0 && token.getTokenType().compare("PUNCTUATION") == 0) {
                        readToken("(");
                        buildTree("PARANTHESIS", "()", 2);
                }

                else{ 
                        readToken("IDENTIFIER");
                        Vl();
                        readToken(")");
                }
        }
}

void Parser::Vl() {

	int i = 1;
        while(token.getTokenValue().compare(",") == 0 && token.getTokenType.compare("PUNCTUATION") == 0) { 
                i++;
                readToken(",");
                readToken("IDENTIFIER");
        }

        if(i > 1)
                buildTree("COMMA", ",", i);
}
