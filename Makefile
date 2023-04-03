CPP = g++
OFLAG = -o
CPPFLAGS = -Wall
.SUFFIXES : .o .cpp .c

.cpp.o:
	$(CPP) $(CPPFLAGS) -c $<

keylogger: main.cpp logger.o logger.h
	$(CPP) $(CPPFLAGS) main.cpp logger.h logger.o $(OFLAG) keylogger

logger.o:	logger.h logger.cpp

run:
	sudo ./keylogger

verbose:
	sudo ./keylogger -v

log:
	sudo cat /dev/001a

clean:
	rm -f keylogger *.o && sudo rm -f /dev/001a