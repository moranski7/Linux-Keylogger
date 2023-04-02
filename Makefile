keylogger: main.cpp logger.h
	g++ logger.h main.cpp -o keylogger

run:
	sudo ./keylogger

verbose:
	sudo ./keylogger -v

clean:
	rm -f keylogger *.gch 001a && sudo rm -f /dev/001a