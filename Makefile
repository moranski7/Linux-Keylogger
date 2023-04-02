keylogger: main.cpp logger.h
	g++ logger.h main.cpp -o keylogger

clean:
	rm -f keylogger *.gch 001a && sudo rm -f /dev/001a