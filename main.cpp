#include <iostream>
#include <regex>
#include <unistd.h>		//For close()
#include <fcntl.h>	//Needed for open() and O_RDONLY
using namespace std;

#define RD O_RDONLY

int main() {
	
	char buffer[128];
	string result = "";
	FILE * pipe = popen("cat /proc/bus/input/devices", "r");
	if (!pipe) {
		cout << "popen failed!" << endl;
	}

	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL) {
			result += buffer;
		}
	}
   
    // regex expression for pattern to be searched 
    regex regexp("Handlers=sysrq kbd event[0-9]"); 
   
    // flag type for determining the matching behavior (in this case on string objects)
    smatch m; 
	regex_search(result, m, regexp);
	string matched = m[0];
	size_t pos = matched.find("event");
	string event = "/dev/input/" + matched.substr (pos);
	const char * device = event.c_str();
	
	int fd = open (device, RD);
	close(fd);

	//Python program
    //import struct
	//f = open("/dev/input/event1", "rb");
	//while True:
	// data = f.read(24)
	// print(struct.unpack('4IHHi',data))

	pclose(pipe);
	return 0;
}