all:
	g++ -g -std=c++11 parser.cc parser.h token.cc token.h p2.cc -o p2
clean:
	rm -f *.o p2
