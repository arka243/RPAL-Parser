#include "lexer.h"
#include "token.h"

/* Following the grammar rules 
mentioned in lexer.pdf */


Token* Lexer::lexer() {

	Token* tok = new Token("value", "type");
	return tok;
}
