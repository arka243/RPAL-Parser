#include<cstdlib>
#include "parser.h"

using namespace std;

int main(int argc, char *argv[]) {
	
	string filename, switch_case;

	if(argc == 2) {
		filename = (string)argv[1];
		switch_case = "off";
	}
	else if(argc == 3) {
		filename = (string)argv[2];
		if((string)argv[2].compare("-ast") == 0)
			switch_case = "ast"
		else
			cout << "\nError --- Execution mode not supported!";
	}
	else {
		cout << "\nPlease follow the following format: ./p1 [-ast] FILENAME";
		cout << "\nExiting ...";
		exit(0);
	}

	Parser parser = new Parser(filename);
	parser.parse(switch_case);
}
