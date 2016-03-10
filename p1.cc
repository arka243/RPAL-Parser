#include<cstdlib>
#include "parser.h"

using namespace std;

int main(int argc, char *argv[]) {
	
	string filepath, exec_mode;

	if(argc == 2) {
		filename = (string)argv[1];
		exec_mode = "none";
	}
	else if(argc == 3) {
		filename = (string)argv[2];
		if((string)argv[2].compare("-ast") == 0)
			exec_mode = "ast"
		else
			cout << "\nError --- Execution mode not supported!";
	}
	else {
		cout << "\nPlease follow the following format: ./p1 [-ast] FILENAME";
		cout << "\nExiting ...";
		exit(0);
	}

	Parser parser = new Parser(filepath);
	parser.parse(exec_mode);
}
