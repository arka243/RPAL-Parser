#include "parser.h"
#include<algorithm>
#include<vector>

treeNode::treeNode(string value, string type) {

	node_value = value;
	node_type = type;
}

string treeNode::getNodeValue() {

	return node_value;
}

void treeNode::setNodeValue(string value) {

	node_value = value;
}

string treeNode::getNodeType() {

	return node_type;
}

void treeNode::setNodeType(string type) {

	node_type = type;
}

controlStructure::controlStructure(treeNode *treenode) {
	node = treenode;	
}

controlStructure::controlStructure() {

}

stackElement::stackElement(environment *e) {

	elementType = "ENVIRONMENT";
	stackEnv = e;
}

stackElement::stackElement(int value) {

	elementType = "INTEGER";
	intValue = value;
}

stackElement::stackElement(string str) {

	elementType = "STRING";
	strValue = str;
}

stackElement::stackElement() {

}

environment::environment() {

}

Parser::Parser(string filepath) {

	last_scanned_char = ' ';
	controlstructure = new controlStructure();
	inputfilestream.open(filepath.c_str(), ifstream::in);
	if(!inputfilestream) {
		cout << "\nError...Could not open file!";
		cout << "\nExiting ...";
		exit(0);
	}
	token = lexer();
}

void Parser::parse(string exec_mode) {

	E();
	if(token->getTokenType().compare("ENDOFFILE") != 0)
		cout << "\nError --- End of File expected after lexical analysis!";
	if(!nodeList.empty()) {
		rootNode = nodeList.top();
		if(exec_mode.compare("ast") == 0) {
			treeTraversal(rootNode, 0);
		}
		treeStandardize(rootNode);
		treetoControlStructure(rootNode, controlstructure);
		env = new environment();
		generateControlStack(env, controlstructure);
		generateExecutionStack(env);
		runCSEMachine();
	}
}

/* Following grammar rules
mentioned in rpal.grammar.pdf */

void Parser::E() {

	if(token->getTokenValue().compare("let") == 0 && token->getTokenType().compare("RESERVED") == 0) {
		tokenValueReader("let");
		D();
		tokenValueReader("in");
		E();
		treeBuilder("LET", "let", 2);
	}
	else if(token->getTokenValue().compare("fn") == 0 && token->getTokenType().compare("RESERVED") == 0) {
		tokenValueReader("fn");
		int i = 0;
		while((token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) || token->getTokenType().compare("IDENTIFIER") == 0) {
			i++;
			Vb();
		}
		tokenValueReader(".");
		E();
		treeBuilder("LAMBDA", "lambda", i+1);
	}
	else 
		Ew();
}

void Parser::Ew() {

	T();
	if(token->getTokenValue().compare("where") == 0 && token->getTokenType().compare("RESERVED") == 0) {
		tokenValueReader("where");
		Dr();
		treeBuilder("WHERE", "where", 2);
	}
}

void Parser::T() {

	Ta();
        int i = 0;
        while(token->getTokenValue().compare(",") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) {
                i++; 
                tokenValueReader(",");
                Ta();
        } 
        if(i > 0) {
                treeBuilder("TAU", "tau", i+1);
	}
}

void Parser::Ta() {

	Tc();
        while(token->getTokenValue().compare("aug") == 0 && token->getTokenType().compare("RESERVED") == 0) { 
                tokenValueReader("aug");
                Tc();
                treeBuilder("AUG", "aug", 2);
        }
}

void Parser::Tc() {

	B();
        if(token->getTokenValue().compare("->") == 0 && token->getTokenType().compare("OPERATOR") == 0) { 
                tokenValueReader("->");
                Tc();
                tokenValueReader("|");
                Tc();
                treeBuilder("CONDITION", "->", 3);
        }
}

void Parser::B() {

	Bt();
        while(token->getTokenValue().compare("or") == 0 && token->getTokenType().compare("RESERVED") == 0) { 
                tokenValueReader("or");
                Bt();
                treeBuilder("OR", "or", 2);
        }
}

void Parser::Bt() {

        Bs();
        while(token->getTokenValue().compare("&") == 0 && token->getTokenType().compare("OPERATOR") == 0) { 
                tokenValueReader("&");
                Bs();
                treeBuilder("AMPERSAND", "&", 2);
        }
}

void Parser::Bs() {

        if(token->getTokenValue().compare("not") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("not");
                Bp();
                treeBuilder("NOT", "not", 1);
        }
        else {
                Bp();
	}
}

void Parser::Bp() {

	A();
        if((token->getTokenValue().compare("gr") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare(">") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                tokenValueReader(token->getTokenValue());
                A();
                treeBuilder("GREATER", "gr", 2);
        }
        else if((token->getTokenValue().compare("ge") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare(">=") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                tokenValueReader(token->getTokenValue());
                A();
                treeBuilder("GREATEREQUAL", "ge", 2);
        }
        else if((token->getTokenValue().compare("ls") == 0 && token->getTokenType().compare("RESERVED") == 0 ) || (token->getTokenValue().compare("<") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                tokenValueReader(token->getTokenValue());
                A();
                treeBuilder("LESS", "ls", 2);
        }
        else if((token->getTokenValue().compare("le") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare("<=") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                tokenValueReader(token->getTokenValue());
                A();
                treeBuilder("LESSEQUAL", "le", 2);
        }
        else if(token->getTokenValue().compare("eq") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("eq");
                A();
                treeBuilder("EQUAL", "eq", 2);
        }
        else if(token->getTokenValue().compare("ne") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("ne");
                A();
                treeBuilder("NOTEQUAL", "ne", 2);
        }
}

void Parser::A() {

        if(token->getTokenValue().compare("+") == 0 && token->getTokenType().compare("OPERATOR") == 0) { 
                tokenValueReader("+");
                At();
        }
        else if(token->getTokenValue().compare("-") == 0 && token->getTokenType().compare("OPERATOR") == 0) { 
                tokenValueReader("-");
                At();
                treeBuilder("NEGATIVE", "neg", 1);
        }
        else { 
                At();
	}
        while((token->getTokenValue().compare("+") == 0 && token->getTokenType().compare("OPERATOR") == 0) || (token->getTokenValue().compare("-") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                tokenValueReader(token->getTokenValue());
                At();
                if(token->getTokenValue().compare("+") == 0 ) {
                        treeBuilder("ADDITION", token->getTokenValue(), 2);
		}
                else {
                        treeBuilder("SUBTRACTION", token->getTokenValue(), 2);
		}
        }
}

void Parser::At() {

        Af();
        while((token->getTokenValue().compare("*") == 0 && token->getTokenType().compare("OPERATOR") == 0) || (token->getTokenValue().compare("/") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
		tokenValueReader(token->getTokenValue());
                Af();
                if(token->getTokenValue().compare("*") == 0) {
                	treeBuilder("MULTIPLICATION", token->getTokenValue(), 2);
		}
                else {
                	treeBuilder("DIVISION", token->getTokenValue(), 2);
		}
        }
}

void Parser::Af() {

        Ap();
        if(token->getTokenValue().compare("**") == 0 && token->getTokenType().compare("OPERATOR") == 0)  {
                tokenValueReader("**");
                Af();
                treeBuilder("EXPONENTIAL", "**", 2);
        }
}

void Parser::Ap() {
        
        R();
        while(token->getTokenValue().compare("@") == 0 && token->getTokenType().compare("OPERATOR") == 0){
                tokenValueReader("@");
                tokenTypeReader("IDENTIFIER");
                R();
                treeBuilder("ATTHERATE", "@", 3);
        }
}

void Parser::R() {
        
        Rn();
        while((token->getTokenValue().compare("true") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare("false") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare("nil") == 0 && token->getTokenType().compare("RESERVED") == 0) || (token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) || (token->getTokenValue().compare("dummy") == 0 && (token->getTokenType().compare("RESERVED") == 0)) || (token->getTokenType().compare("IDENTIFIER") == 0) || (token->getTokenType().compare("STRING") == 0) || (token->getTokenType().compare("INTEGER") == 0)) {
                Rn();
                treeBuilder("GAMMA", "gamma", 2);
        }
}

void Parser::Rn() {

        if(token->getTokenType().compare("IDENTIFIER") == 0) {
                tokenTypeReader("IDENTIFIER");
	}
        else if(token->getTokenType().compare("INTEGER") == 0) {
                tokenTypeReader("INTEGER");
	}
        else if(token->getTokenType().compare("STRING") == 0) {
                tokenTypeReader("STRING");
	}
        else if(token->getTokenValue().compare("true") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("true");
                treeBuilder("TRUE", "true", 0);
        }
        else if(token->getTokenValue().compare("false") == 0 && token->getTokenType().compare("RESERVED")) {
                tokenValueReader("false");
                treeBuilder("FALSE", "false", 0);
        }
        else if(token->getTokenValue().compare("nil") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("nil");
                treeBuilder("NIL", "nil", 0);
        }
        else if(token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) {
                tokenValueReader("(");
                E();
                tokenValueReader(")");
        }
        else {
                tokenValueReader("dummy");
                treeBuilder("DUMMY", "dummy", 0);
        }
}

void Parser::D() {

        Da();
        if(token->getTokenValue().compare("within") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("within");
                D();
                treeBuilder("WITHIN", "within", 2);
        }
}

void Parser::Da() {

        Dr();
        int i=0;
        while(token->getTokenValue().compare("and") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                i++;
                tokenValueReader("and");
                Dr();
        }
        if(i > 0)
                treeBuilder("AND", "and", i+1);
}

void Parser::Dr() {

        if(token->getTokenValue().compare("rec") == 0 && token->getTokenType().compare("RESERVED") == 0) {
                tokenValueReader("rec");
                Db();
                treeBuilder("REC", "rec", 1);
        }
        else {
                Db();
	}
}

void Parser::Db() {

        if(token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) { 
                tokenValueReader("(");
                D();
                tokenValueReader(")");
        }
        else if (token->getTokenType().compare("IDENTIFIER") == 0) { 
                tokenTypeReader("IDENTIFIER"); 
                if (token->getTokenValue().compare(",") == 0  && token->getTokenType().compare("PUNCTUATION") == 0 || (token->getTokenValue().compare("=") == 0 && token->getTokenType().compare("OPERATOR") == 0)) {
                        Vl(); 
                        tokenValueReader("=");
                        E();
                        treeBuilder("EQUAL", "=", 2);
                }
                else {
                        int i = 0;
                        while((token->getTokenValue().compare("(") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) || token->getTokenType().compare("IDENTIFIER") == 0) {
                                i++;
                                Vb();
                        }
                        tokenValueReader("=");
                        E();
                        treeBuilder("FUNCTION_FORM", "function_form", i+2);
                }
        }
}

void Parser::Vb() {
	
       	if(token->getTokenType().compare("IDENTIFIER") == 0 ) { 
                tokenTypeReader("IDENTIFIER");
        }
        else { 
                tokenValueReader("(");
                if(token->getTokenValue().compare(")") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) {
                        tokenValueReader("(");
                        treeBuilder("PARANTHESIS", "()", 2);
                }
                else{ 
                        tokenTypeReader("IDENTIFIER");
                        Vl();
                        tokenValueReader(")");
                }
        }
}

void Parser::Vl() {

	int i = 1;
        while(token->getTokenValue().compare(",") == 0 && token->getTokenType().compare("PUNCTUATION") == 0) { 
                i++;
                tokenValueReader(",");
                tokenTypeReader("IDENTIFIER");
        }
        if(i > 1)
                treeBuilder("COMMA", ",", i);
}

void Parser::tokenValueReader(string value) {
	
	if(token->getTokenValue().compare(value) != 0) {
		cout << "\nError --- Wrong Token Found! Expected " << value << " but found " << token->getTokenValue();
		cout << "\nExiting ...";
		exit(0);
	}
	token = lexer();
}

void Parser::tokenTypeReader(string type) {

	if(token->getTokenType().compare(type) != 0) {
		cout << "\nError --- Wrong Token Found! Expected " << type << " but found " << token->getTokenType();
                cout << "\nExiting ...";
                exit(0);
        }
	else {
		if(token->getTokenType().compare("IDENTIFIER") == 0)
			treeBuilder("IDENTIFIER", token->getTokenValue(), 0);
		else if(token->getTokenType().compare("INTEGER") == 0)
			treeBuilder("INTEGER", token->getTokenValue(), 0);
		else if(token->getTokenType().compare("STRING") == 0)
			treeBuilder("STRING", token->getTokenValue(), 0);		
	}
        token = lexer();
}

void Parser::treeBuilder(string node_type, string node_value, int node_num) {

	treeNode *node = new treeNode(node_value, node_type);
	treeNode *temp = NULL;
	for(int i=0; i < node_num; i++) {
		treeNode *topNode = nodeList.top();
		nodeList.pop();
		if(temp == NULL)
			temp = topNode;
		else {
			topNode->right = temp;
			temp = topNode;
		}
	}
	node->left = temp;
	nodeList.push(node);
}

void Parser::treeTraversal(treeNode *root, int dots) {

	treeNode *temp = root;
	if(root != NULL) {
		for(int i=1; i<= dots; i++)
			cout << ".";
		if(root->getNodeType() == "STRING")
			cout << "<STR:'" << root->getNodeValue() << "'>" << endl;
		else if(root->getNodeType() == "IDENTIFIER")
                        cout << "<ID:" << root->getNodeValue() << ">" << endl;
		else if(root->getNodeType() == "INTEGER")
                        cout << "<INT:" << root->getNodeValue() << ">" << endl;
		else
			cout << root->getNodeValue() << endl;
		if(temp->left != NULL)
			treeTraversal(temp->left, dots+1);
		if(temp->right != NULL)
			treeTraversal(temp->right, dots);
	}
}
		
void Parser::treeStandardize(treeNode *treenode) {

	if(treenode->left != NULL)
		treeStandardize(treenode->left);
	if(treenode->right != NULL)
		treeStandardize(treenode->right);
	string nodeType = treenode->getNodeType();
	if(nodeType.compare("LET") == 0) {
		if(treenode != NULL) {
			treenode->setNodeValue("gamma");
			treenode->setNodeType("GAMMA");
			if(treenode->left != NULL) {
				(treenode->left)->setNodeValue("lambda");
				(treenode->left)->setNodeType("LAMBDA");
			}
			else {
				cout << "\nNode left child is null...exiting";
				exit(0);
			}
			if((treenode->left)->right != NULL) {
				if(((treenode->left)->left)->right != NULL) {
					treeNode *temp = (treenode->left)->right;
					(treenode->left)->right = ((treenode->left)->left)->right;
					((treenode->left)->left)->right = temp;
				}
				else {
					cout << "\nError in standardizing node LET...exiting";
					exit(0);
				}
			}
			else {
				cout << "\nNode left child's right sibling is null...exiting";
				exit(0);
			}	
		}
		else {
			cout << "\nNode is null...exiting";
			exit(0);
		}
	}
	else if(nodeType.compare("WHERE") == 0) {
		if(treenode != NULL) {
			treenode->setNodeValue("gamma");
			treenode->setNodeType("GAMMA");
			if(treenode->left != NULL) {
				treeNode *temp1 = treenode->left; 
				if((treenode->left)->right != NULL) {
					if(((treenode->left)->right)->getNodeType().compare("EQUAL") == 0) {
						(temp1->right)->setNodeValue("lambda");
						(temp1->right)->setNodeType("LAMBDA");
						if(((treenode->left)->right)->left != NULL) {
							if((((treenode->left)->right)->left)->right != NULL) {
								treeNode *temp2 = (temp1->right)->left; 
								treeNode *temp3 = temp2->right; 
								treenode->left = temp1->right;
								temp1->right = NULL;
								(treenode->left)->right = temp3;
								temp2->right = temp1;
							}
							else {
								cout << "Node cannot be null...exiting";
                                                        	exit(0);
							}
						}
						else {
							cout << "Node cannot be null...exiting";
							exit(0);
						}	
					}
					else {
						cout << "\nUnexpected node type encountered...exiting";
						exit(0);
					}
				}
				else {
					cout << "\nNode left child's right sibling is null...exiting";
					exit(0);
				}
			}
			else {
				cout << "\nNode left child is null...exiting";
				exit(0);
			}
		}
		else {
			cout << "\nNode is null...exiting";
			exit(0);
		} 	
	}
	else if(nodeType.compare("FUNCTION_FORM") == 0) {
		if(treenode != NULL) {
			treenode->setNodeValue("eq");
			treenode->setNodeType("EQUAL");
			if(treenode->left != NULL) {
				if((treenode->left)->right != NULL) {
					treeNode *temp = treenode->left;	
					treeNode *nodeptr = temp->right;	
					while(nodeptr->right != NULL) {
						treeNode *newnode = new treeNode("lambda","LAMBDA");
						newnode->left = nodeptr;
						temp->right = newnode;
						temp = nodeptr;
						nodeptr = nodeptr->right;
					}
					temp->right = nodeptr;
				}
				else {
					cout << "\nNode left child's right sibling is null...exiting";
					exit(0);
				}
			}
			else {
				cout << "\nNode left child is null...exiting";
				exit(0);
			}
		}
		else {
			cout << "\nNode is null...exiting";
			exit(0);
		}
	}
	else if(nodeType.compare("WITHIN") == 0) {
		if(treenode != NULL) {
			treenode->setNodeValue("eq");
			treenode->setNodeType("EQUAL");
			if(treenode->left != NULL) {
				if((treenode->left)->left != NULL && (treenode->left)->right != NULL) {
					if(((treenode->left)->right)->left != NULL) {
						treeNode *temp1 = (treenode->left)->left;			
						treeNode *temp2 = ((treenode->left)->right)->left;		
						treeNode *temp3 = temp1->right; 		
						treeNode *temp4 = temp2->right; 	
						treenode->left = temp2;
						treeNode *newnode1 = new treeNode("gamma", "GAMMA");
						temp2->right = newnode1;
						treeNode *newnode2 = new treeNode("lambda","LAMBDA");
						(temp2->right)->left = newnode2;
						newnode2->right = temp3;
						newnode2->left = temp1;
						temp1->right = temp4;						
					}
					else {
						cout << "\nNode cannot be null...exiting";
						exit(0);
					}
				}
				else {
					cout << "\nNode left child's left child or right sibling is null...exiting";
					exit(0);
				}
			}
			else {
				cout << "\nNode left child is null...exiting";
				exit(0);
			}
		}
		else {
			cout << "\nNode is null...exiting";
			exit(0);
		}
	}
	else if(nodeType.compare("ATTHERATE") == 0) {
		if(treenode != NULL) {
			treenode->setNodeValue("gamma");
			treenode->setNodeType("GAMMA");
			if(treenode->left != NULL) {
				if((treenode->left)->right != NULL) {
					if(((treenode->left)->right)->right != NULL) {
						treeNode *temp1 = treenode->left;
						treeNode *temp2 = temp1->right;
						treeNode *temp3 = temp2->right;
						treeNode *newnode = new treeNode("gamma", "GAMMA");
						treenode->left = newnode;
						newnode->left = temp2;
						newnode->right = temp3;
						temp2->right = temp1;
						temp1->right = NULL;
					}
					else {
						cout << "\nNode cannot be null...exiting";
						exit(0);
					}
				}
				else {
					cout << "\nNode left child's right sibling is null...exiting";
					exit(0);
				}
			}
			else {
				cout << "\nNode left child is null...exiting";
				exit(0);
			}
		}
		else {
			cout << "\nNode is null...exiting";
			exit(0);
		}
	}
	else if(nodeType.compare("AND") == 0) {
		if(treenode != NULL) {
			treenode->setNodeValue("eq");
			treenode->setNodeType("EQUAL");
			if(treenode->left != NULL) {
				if((treenode->left)->left != NULL) {
					if(((treenode->left)->left)->right != NULL) {
						treeNode *temp = treenode->left;
						treeNode *commanode = new treeNode(",", "COMMA");
						treeNode *taunode = new treeNode("tau", "TAU");
						treenode->left = commanode;
						commanode->left = temp->left;
						commanode->right = taunode;
						taunode->left = (commanode->left)->right;
						commanode = commanode->left;
						taunode = taunode->left;
						temp = temp->right;
						while(temp != NULL) {
							if(commanode != NULL && taunode != NULL) {
								commanode->right = temp->left;
                                                        	taunode->right = (commanode->right)->right;
								commanode = commanode->right;
								taunode = taunode->right;
							}
							else {
                                                                cout << "\ncomma or tau node cannot be null";
                                                                exit(0);
                                                        }
							temp = temp->right;
						}
						commanode->right = NULL;
					}
					else {
						cout << "\nNode cannot be null...exiting";
						exit(0);
					}
				}
				else {
					cout << "\nNode cannot be null...exiting";
					exit(0);
				}
			}
			else {
				cout << "\nNode left child is null...exiting";
				exit(0);
			}
		}
		else {
			cout << "\nNode is null...exiting";
			exit(0);
		}
	}
	else if(nodeType.compare("REC") == 0) {
		if(treenode != NULL) {
			treenode->setNodeValue("eq");
			treenode->setNodeType("EQUAL");
			if(treenode->left != NULL) {
				if((treenode->left)->left != NULL) {
						treeNode *gammanode = new treeNode("gamma", "GAMMA");
						treeNode *lambdanode = new treeNode("lambda", "LAMBDA");
						treeNode *ystarnode = new treeNode("ystar", "YSTAR");
						treeNode *temp1 = treenode->left;
						treeNode *temp2 = temp1->left;
						treeNode *temp3 = temp2->right;
						treenode->left = temp2;
						temp2->right = gammanode;
						gammanode->left = ystarnode;
						ystarnode->right = lambdanode;
						treeNode *newnode = new treeNode(temp2->getNodeValue(), temp2->getNodeType());
						lambdanode->left = newnode;
						newnode->left = temp2->left;
						newnode->right = temp3;
				}
				else {
					cout << "\nNode cannot be null...exiting";
					exit(0);
				}
			}
			else {
				cout << "\nNode left child is null...exiting";
				exit(0);
			}
		}
		else {
			cout << "\nNode is null...exiting";
			exit(0);
		}
	}
}

void Parser::treetoControlStructure(treeNode *treenode, controlStructure *cs) {

	if(treenode != NULL) {
		string nodeType = treenode->getNodeType();
		if(nodeType.compare("GAMMA") == 0) {
			controlStructure *temp = new controlStructure(treenode);
                        cs->next = temp;
                        cs = cs->next;
                        if(treenode->left != NULL)
                                treetoControlStructure(treenode->left, cs);
                        while(cs->next != NULL)
                                cs = cs->next;
                        if(treenode->right != NULL)
                                treetoControlStructure(treenode->right, cs);
		}
		else if(nodeType.compare("LAMBDA") == 0) {
			treeNode *tempnode = (treenode->left)->right;
			controlStructure *temp = new controlStructure(treenode);
			if(treenode->left != NULL) {
				temp->leftchild = treenode->left;
				(temp->leftchild)->right = NULL;
			}
			else {
				cout << "\nLeft child cannot be null...exiting";
				exit(0);
			}
			cs->next = temp;
			cs = cs->next;
			controlStructure *deltabranchoff = new controlStructure();
			treetoControlStructure(tempnode, deltabranchoff);
			cs->deltaIndex = deltabranchoff->next;
			if(treenode->right != NULL)
				treetoControlStructure(treenode->right, cs);
		}
		else if(nodeType.compare("CONDITION") == 0) {
			if(treenode->left != NULL) {
				if((treenode->left)->right != NULL) {
					if(((treenode->left)->right)->right != NULL) {
						treeNode *temp1 = treenode->left;
						treeNode *temp2 = temp1->right;
						treeNode *temp3 = temp2->right;
						temp1->right = NULL;
						temp2->right = NULL;
						treeNode *deltathen = new treeNode("deltathen", "DELTATHEN");
						controlStructure *csdeltathen = new controlStructure(deltathen);
						cs->next = csdeltathen;
						cs = cs->next;
						controlStructure *ctrlstruct1 = new controlStructure();
						treetoControlStructure(temp1, ctrlstruct1);
						cs->deltaIndex = ctrlstruct1->next;
						treeNode *deltaelse = new treeNode("deltaelse", "DELTAELSE");
						controlStructure *csdeltaelse = new controlStructure(deltaelse);
						cs->next = csdeltaelse;
						cs = cs->next;
						controlStructure *ctrlstruct2 = new controlStructure();
						treetoControlStructure(temp2, ctrlstruct2);
						cs->deltaIndex = ctrlstruct2->next;
						treeNode *beta = new treeNode("beta", "BETA");
						controlStructure *csbeta = new controlStructure(beta);
						cs->next = csbeta;
						cs = cs->next;
						controlStructure *ctrlstruct3 = new controlStructure();
						treetoControlStructure(temp3, ctrlstruct3);
						cs->next = ctrlstruct3->next;
						while(cs->next != NULL)
							cs = cs->next;
						if(treenode->right != NULL)
							treetoControlStructure(treenode->right, cs);
					}
					else {
						cout << "\nNode cannot be null...exiting";
						exit(0);
					}
				}
				else {
					cout << "\nNode left child's right node is null...exiting";
					exit(0);
				}
			}
			else {
				cout << "\nNode left child is null...exiting";
				exit(0);
			}
		}
		else if(nodeType.compare("TAU") == 0) {
                        controlStructure *temp = new controlStructure(treenode);
                        cs->next = temp;
                        cs = cs->next;
                        treetoControlStructure(treenode->left, cs);
                        while(cs->next != NULL)
                                cs = cs->next;
                        if(treenode->right != NULL)
                                treetoControlStructure(treenode->right, cs);
                }
		else {
			controlStructure *temp = new controlStructure(treenode);
			cs->next = temp;
			cs = cs->next;
			if(treenode->left != NULL)
				treetoControlStructure(treenode->left, cs);
			while(cs->next != NULL)
				cs = cs->next;
			if(treenode->right != NULL)
				treetoControlStructure(treenode->right, cs);
		}
	}
	else {
		cout << "\nNode cannot be null! Exiting...";
		exit(0);
	}
}

void Parser::generateControlStack(environment *new_env, controlStructure *cs) {
	
	controlStructure *envctrl = new controlStructure(new treeNode("environment", "ENVIRONMENT"));
	envctrl->currentEnv = new_env;
	controlStack.push(envctrl);
	if(cs->next != NULL) {
		controlStructure *temp = cs->next;
		while(temp->next != NULL) {
			controlStack.push(temp);
			temp = temp->next;
		}
		controlStack.push(temp);
	}
	else {
		cout << "\nError...control structure not found! Exiting...";
		exit(0);
	}
}

void Parser::generateExecutionStack(environment *new_env) {

	stackElement *se = new stackElement(new_env);
	executionStack.push(se);
}

void Parser::runCSEMachine() {

	while(!controlStack.empty()) {
		controlStructure *currentctrlElement = controlStack.top();
		controlStack.pop();
		string currentelementType = (currentctrlElement->node)->getNodeType();
		string currentelementValue = (currentctrlElement->node)->getNodeValue();
		cout << currentelementType << endl;
		if(currentelementType.compare("INTEGER") == 0) {
			int value = atoi(currentelementValue.c_str());
			stackElement *new_se = new stackElement(value);
			executionStack.push(new_se);			
		}
		if(currentelementType.compare("STRING") == 0) {
			stackElement *new_se = new stackElement(currentelementValue);
			executionStack.push(new_se);
		}
		if(currentelementType.compare("IDENTIFIER") == 0) {
			stackElement *new_se = new stackElement();
			map<string, stackElement*>::iterator itr = declaration_table.find(currentelementValue);
			if(itr != declaration_table.end())
				executionStack.push(itr->second);
			else {
				stackElement *new_se = new stackElement();
				new_se->elementType = "PRIMITIVE_FUNCTION";
				if(currentelementValue.compare("Isinteger") == 0) 
					new_se->primfunc = "ISINTEGER";
				else if(currentelementValue.compare("Isstring") == 0) 
                                        new_se->primfunc = "ISSTRING";
				else if(currentelementValue.compare("Istruthvalue") == 0) 
                                        new_se->primfunc = "ISTRUTHVALUE";
				else if(currentelementValue.compare("Istuple") == 0) 
                                        new_se->primfunc = "ISTUPLE";
				else if(currentelementValue.compare("Print") == 0) 
                                        new_se->primfunc = "PRINT";
				else if(currentelementValue.compare("Order") == 0)
                                        new_se->primfunc = "ORDER";
				else if(currentelementValue.compare("ItoS") == 0)
                                        new_se->primfunc = "ITOS";
				else if(currentelementValue.compare("Stern") == 0)
                                        new_se->primfunc = "STERN";
				else if(currentelementValue.compare("Stem") == 0)
                                        new_se->primfunc = "STEM";
				else if(currentelementValue.compare("Conc") == 0)
                                        new_se->primfunc = "CONC";
				else {
					cout << "\nUndefined identifier...exiting";
					exit(0);
				}
				executionStack.push(new_se);
			}				
		}
		if(currentelementType.compare("TRUE") == 0) {
			stackElement *new_se = new stackElement();
			new_se->elementType = "TRUE";
			executionStack.push(new_se);
		}
		if(currentelementType.compare("FALSE") == 0) {
			stackElement *new_se = new stackElement();
                        new_se->elementType = "FALSE";
                        executionStack.push(new_se);
		}
		if(currentelementType.compare("NIL") == 0) {
			stackElement *new_se = new stackElement();
                        new_se->elementType = "NIL";
                        executionStack.push(new_se);
		}
		if(currentelementType.compare("DUMMY") == 0) {
			stackElement *new_se = new stackElement();
                        new_se->elementType = "DUMMY";
                        executionStack.push(new_se);
		}
		if(currentelementType.compare("EQUAL") == 0) {
			stackElement *firstElement = executionStack.top();
			if(firstElement == NULL) {
				cout << "\nElement cannot be null...exiting";
				exit(0);
			}
			executionStack.pop();
			stackElement *secondElement = executionStack.top();
			if(secondElement == NULL) {
                                cout << "\nElement cannot be null...exiting";
                                exit(0);
                        }
			executionStack.pop();
			bool output;
			if((firstElement->elementType.compare(secondElement->elementType) == 0) || (firstElement->elementType.compare("TRUE") == 0 && secondElement->elementType.compare("FALSE") == 0) || (firstElement->elementType.compare("FALSE") == 0 && secondElement->elementType.compare("TRUE") == 0)) {
				if(firstElement->elementType.compare("STRING") == 0) {
					if(firstElement->strValue.compare(secondElement->strValue) == 0)
						output = true;
					else
						output = false;
				}
				else if(firstElement->elementType.compare("INTEGER") == 0) {
					if(firstElement->intValue == secondElement->intValue)
						output = true;
					else
						output = false;
				}
				else if(firstElement->elementType.compare("TRUE") == 0 || firstElement->elementType.compare("FALSE") == 0) {
					if(firstElement->elementType.compare(secondElement->elementType) == 0)
						output = true;
					else
						output = false;
				}
				else {
					cout << "\nError - unidentified type of element...exiting";
					exit(0);
				}
				if(output) {
					stackElement *new_se = new stackElement();
					new_se->elementType = "TRUE";
					executionStack.push(new_se);
				}
				else {
					stackElement *new_se = new stackElement();
					new_se->elementType = "FALSE";
					executionStack.push(new_se);
				}
			}
			else {
				cout << "\nElement type mismatch! Exiting";
				exit(0);
			}
		}
		if(currentelementType.compare("NOTEQUAL") == 0) {
                        stackElement *firstElement = executionStack.top();
                        if(firstElement == NULL) {
                                cout << "\nElement cannot be null...exiting";
                                exit(0);
                        }
                        executionStack.pop();
                        stackElement *secondElement = executionStack.top();
                        if(secondElement == NULL) {
                                cout << "\nElement cannot be null...exiting";
                                exit(0);
                        }
                        executionStack.pop();
                        bool output;
                        if((firstElement->elementType.compare(secondElement->elementType) == 0) || (firstElement->elementType.compare("TRUE") == 0 && secondElement->elementType.compare("FALSE") == 0) || (firstElement->elementType.compare("FALSE") == 0 && secondElement->elementType.compare("TRUE") == 0)) {
                                if(firstElement->elementType.compare("STRING") == 0) {
                                        if(firstElement->strValue.compare(secondElement->strValue) == 1)
                                                output = true;
                                        else
                                                output = false;
                                }
                                else if(firstElement->elementType.compare("INTEGER") == 0) {
                                        if(firstElement->intValue != secondElement->intValue)
                                                output = true;
                                        else
                                                output = false;
                                }
                                else if(firstElement->elementType.compare("TRUE") == 0 || firstElement->elementType.compare("FALSE") == 0) {
                                        if(firstElement->elementType.compare(secondElement->elementType) == 1)
                                                output = true;
                                        else
                                                output = false;
                                }
                                else {
                                        cout << "\nError - unidentified type of element...exiting";
                                        exit(0);
                                }
                                if(output) {
                                        stackElement *new_se = new stackElement();
                                        new_se->elementType = "TRUE";
                                        executionStack.push(new_se);
                                }
                                else {
                                        stackElement *new_se = new stackElement();
                                        new_se->elementType = "FALSE";
                                        executionStack.push(new_se);
                                }
                        }
                        else {
                                cout << "\nElement type mismatch! Exiting";
                                exit(0);
                        }
                }
		
	}
}

/* Following the grammar rules 
mentioned in lexer.pdf */

std::vector<string> RESERVED_LIST = { "let", "in", "fn", "where", "aug", "or", "not", "gr", "ge", "ls", "le", "eq", "ne", "true", "false", "nil", "dummy", "within", "and", "rec" };

Token* Parser::lexer() {

	char ch;
	Token *tok;
	if(last_scanned_char == ' ')
		ch = inputfilestream.get();
	else
		ch = last_scanned_char;
	string tokenbuilder = "";
	
	while(ch == ' ' || ch == '\t' || ch == '\n') {
		ch = inputfilestream.get();
	}

	if(isalpha(ch)) {
		tokenbuilder += ch;
		ch = inputfilestream.get();
		while(ch == '_' || isalpha(ch) || isdigit(ch)) {
			tokenbuilder += ch;
			ch = inputfilestream.get();
		}
		last_scanned_char = ch;
		if(std::find(RESERVED_LIST.begin(), RESERVED_LIST.end(), tokenbuilder) != RESERVED_LIST.end()) {
			tok = new Token(tokenbuilder, "RESERVED");
		}
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
	else if(ch == '+' || ch == '.' || ch == '&' || ch == '@' || ch == '=' || ch == ':' || ch == '~' || ch == '|' || ch == '$' || ch == '!' || ch == '#' || ch == '%' || ch == '^' || ch == '_' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '"' || ch == '?') {
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
			tok = new Token("", "DELETE");
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
	else if(ch == '\'') {
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
			if(ch == EOF) {
				cout << "\nError Scanning Strings!";
				cout << "\nExiting ..." << endl;
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
