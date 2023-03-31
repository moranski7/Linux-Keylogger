keylogger: main.cpp logger.h
	g++ logger.h main.cpp -o keylogger

clean:
	rm -f keylogger *.gch