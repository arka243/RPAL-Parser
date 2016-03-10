#include "lexer.h"
#include "token.h"
#include<algorithm>
#include<vector>

/* Following the grammar rules 
mentioned in lexer.pdf */

std::vector<string> RESERVED_LIST = {"let", "in", "fn", "where", "aug", "or", "not", "gr", "ge", "ls", "le", "eq", "ne", "true", "false", "nil", "dummy", "within", "and", "rec"};

Token* Lexer::lexer() {

	char ch;
	Token *tok;
	if(last_scanned_char == ' ')
		ch = inputfilestream.get();
	else
		ch = last_scanned_char;
	string tokenbuilder = "";
	
	while(ch == ' ' || ch == '\t' || ch == '\n')
		ch = inputfilestream.get();

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
	else if(ch == '+' || ch == '.' || ch == '&' || ch == '@' || ch == "=" || ch == ':' || ch == '~' || ch == '|' || ch == '$' || ch == '!' || ch == '#' || ch == '%' || ch == '^' || ch == '_' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '"' || ch == '?') {
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
			tok = new Token("", "ENDOFLINE");
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
	else if(ch == '\') {
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
			if(c == EOF) {
				cout << "\nError --- End Quotes not found!";
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
