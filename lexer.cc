#include "lexer.h"

void Lexer:E() {

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
