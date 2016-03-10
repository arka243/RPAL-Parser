#include "lexer.h"
#include "token.h"

Token* Lexer::lexer() {

	Token* tok = new Token("value", "type");
	return tok;
}
