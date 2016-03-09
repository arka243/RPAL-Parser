#include<cstdlib>
#include<string>

using namespace std;

class Token {

	string token_value, token_type;

	public:
	
	Token(value, type);

	string getTokenValue();
	void setTokenValue(value);
	string getTokenType();
	void setTokenType(type);
};
