/*
 * conio_gnu.h
 *
 *  Created on: 31.05.2016.
 *      Author: d-logic (http://www.d-logic.net/nfc-rfid-reader-sdk/)
 */

#ifndef CONIO_GNU_H_
#define CONIO_GNU_H_
#if linux || __linux__ || __APPLE__

void _initTermios(int echo);
void _resetTermios(void);
char _getch(void);
int  _kbhit(void);

#endif // linux || __linux__ || __APPLE__
#endif /* CONIO_GNU_H_ */
