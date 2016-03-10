#include<string>

using namespace std;

class Token {

	string token_value, token_type;

	public:
	
	Token(string value, string type);

	string getTokenValue();
	void setTokenValue(string value);
	string getTokenType();
	void setTokenType(string type);
};
