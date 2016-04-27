#include<cstdlib>
#include "parser.h"

using namespace std;

int main(int argc, char *argv[]) {
	
	string filepath, exec_mode;

	if(argc == 2) {
		filepath = (string)argv[1];
		exec_mode = "none";
	}
	else if(argc == 3) {
		filepath = (string)argv[2];
		if(((string)argv[1]).compare("-ast") == 0) 
			exec_mode = "ast";
		else if(((string)argv[1]).compare("-std") == 0)
			exec_mode = "std";
		else
			cout << "\nError --- Execution mode " << ((string)argv[1]) << "not supported!";
	}
	else {
		cout << "\nPlease follow the following format: ./p2 [-ast]|[-std] FILENAME";
		cout << "\nExiting ...";
		exit(0);
	}

	Parser *parser = new Parser(filepath);
	parser->parse(exec_mode);
}
