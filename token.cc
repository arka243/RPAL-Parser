#include "token.h"

Token::Token(string value, string type) {

	token_value = value;
	token_type = type;
}

string Token::getTokenValue() {

	return token_value;
}

void Token::setTokenValue(string value) {

	token_value = value;
}

string Token::getTokenType() {

	return token_type;
}

void Token::setTokenType(string type) {

	token_type = type;
}
