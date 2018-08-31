/*
 * conio_gnu.c
 *
 *  Created on: 31.05.2016.
 *      Author: d-logic (http://www.d-logic.net/nfc-rfid-reader-sdk/)
 */

#if linux || __linux__ || __APPLE__

#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>

static struct termios old, new;

// Initialize new terminal i/o settings:
void _initTermios(int echo)
{
	tcgetattr(0, &old); // grab old terminal i/o settings
	new = old; // make new settings same as old settings
	new.c_lflag &= ~ICANON; // disable buffered i/o
	new.c_lflag &= echo ? ECHO : ~ECHO; // set echo mode
	tcsetattr(0, TCSANOW, &new); // use these new terminal i/o settings now
}

// Restore old terminal i/o settings:
void _resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}

// Read 1 character:
char _getch(void)
{
	return getchar();
}

int _kbhit(void)
{
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

#endif // linux || __linux__ || __APPLE__
