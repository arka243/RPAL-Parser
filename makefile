all:
	g++ -g parser.cc parser.h token.cc token.h p1.cc -o p1
clean:
	rm -f *.o p1
