#include <iostream>
#include <ncurses.h>

using namespace std;

int main() {
	initscr(); //initializes the screen, sets up memory and clears the screen
	printw("Hello World!\n"); //Prints a string(const char *) to a window
 	refresh(); //refreshes the screen to match whats in memory
 	int c = 0;
 	while (c != 27) {
 		int c = wgetch(); // waits for user input, returns int value of that key
 		printw("c = %d\n", c);
 	}
	getch();
	endwin(); //deallocates memory and ends ncures
	return 0;
}
