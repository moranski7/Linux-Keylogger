#include <iostream> 
#include <regex>
#include <unistd.h>		//For close()
#include <fcntl.h>	//Needed for open() and O_RDONLY
#include <linux/input.h>  //Needed for input_event struct
using namespace std;

int main() {
	
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
	for (int i = 0; i < 30; i++){ 
		read(fd, &holdEvent, sizeof(holdEvent));
		if (holdEvent.type == EV_KEY) {
			cout << "Value: " << holdEvent.value << endl;
			cout << "Code: " << holdEvent.code << endl;
		}
	}
	close(fd);
	cout << "Closed connection to handler." << endl;
	cout << "program done." << endl;
	return 0;
}