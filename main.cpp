#include <iostream> 
#include <regex>
#include <unistd.h>		//For close()
#include <fcntl.h>	//Needed for open() and O_RDONLY
#include <linux/input.h>  //Needed for input_event struct
#include <vector>
using namespace std;

int main() {
	
	vector<string> keyCodes = {
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

	char buffer[128];
	string result = "";

	cout << "For best result run program using sudo." << endl;
	cout << "Starting program..." << endl;
	cout << "Executing command cat /proc/bus/input/devices" << endl;
	FILE * pipe = popen("cat /proc/bus/input/devices", "r");
	if (!pipe) {
		cout << "popen failed!" << endl;
		return 1;
	}
	cout << "popen completed. Reading in result..." << endl;

	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL) {
			//cout << buffer << endl;
			result += buffer;
		}
	}
	if (result == "") {
		cout <<"Popen return empty string. Aborting!" << endl;
		return 1;
	}
	pclose(pipe);
	cout << "Using regex to searching for handler." << endl; 

    regex regexp("Handlers=sysrq kbd event[0-9]"); // regex expression for pattern to be searched    
    smatch m;  // flag type for determining the matching behavior (in this case on string objects)
	regex_search(result, m, regexp);
	if (m.size() == 0){
		cout << "regex search returned no match! Aborting!" << endl;
		return 1;
	}
	cout << "Regex search return: " << m[0] << endl;

	cout << "Creating path to handler." << endl;
	string matched = m[0];
	size_t pos = matched.find("event");
	string event = "/dev/input/" + matched.substr (pos);
	cout << "Path: " << event << endl;

	cout << "Connecting to the handler." << endl;
	const char * device = event.c_str();
	struct input_event holdEvent; 
	int fd = open (device, O_RDONLY);
	//for (int i = 0; i < 30; i++){ 
	while (true) {
		read(fd, &holdEvent, sizeof(holdEvent));
		if (holdEvent.type == EV_KEY and holdEvent.value == 1) {
			cout << "holdEvent.code: " << holdEvent.code << "\t";
			cout << "KeyCode: " << keyCodes[holdEvent.code] << endl;
		}
		if (holdEvent.code == 1) {
			break;
		}
	}
	close(fd);
	cout << "Closed connection to handler." << endl;
	cout << "program done." << endl;


	//for (int i = 0; i < keyCodes.size(); i++) {
	//	cout << "keyCodes["<<i<<"]: " << keyCodes[i] << endl;
	//}
	return 0;
}