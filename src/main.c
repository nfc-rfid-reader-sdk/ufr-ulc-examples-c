/*
 ============================================================================
 Project Name: project_name
 Name        : file_name.c
 Author      : d-logic (http://www.d-logic.net/nfc-rfid-reader-sdk/)
 Version     :
 Copyright   : 2017.
 Description : Project in C (Language standard: c99)
 Dependencies: uFR firmware - min. version x.y.z {define in ini.h}
               uFRCoder library - min. version x.y.z {define in ini.h}
 ============================================================================
 */

/* includes:
 * stdio.h & stdlib.h are included by default (for printf and LARGE_INTEGER.QuadPart (long long) use %lld or %016llx).
 * inttypes.h, stdbool.h & string.h included for various type support and utilities.
 * conio.h is included for windows(dos) console input functions.
 * windows.h is needed for various timer functions (GetTickCount(), QueryPerformanceFrequency(), QueryPerformanceCounter())
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#if __WIN32 || __WIN64
#	include <conio.h>
#	include <windows.h>
#elif linux || __linux__ || __APPLE__
#	define __USE_MISC
#	include <unistd.h>
#	include <termios.h>
#	undef __USE_MISC
#	include "conio_gnu.h"
#else
#	error "Unknown build platform."
#endif
#include <uFCoder.h>
#include "ini.h"
#include "uFR.h"
#include "utils.h"
//------------------------------------------------------------------------------
void usage(void);
void menu(char key);
UFR_STATUS NewCardInField(uint8_t sak, uint8_t *uid, uint8_t uid_size);
void enter_transceive(void);
void exit_transceive(void);
void set_user_zone(void);
void write_user_zone(void);
void read_user_zone(void);
//------------------------------------------------------------------------------

BOOL card_transceive_mode = FALSE;

int main(void)
{
	char key;
	bool card_in_field = false;
	uint8_t old_sak = 0, old_uid_size = 0, old_uid[10];
	uint8_t sak, uid_size, uid[10];
	UFR_STATUS status;

	usage();
	printf(" --------------------------------------------------\n");
	printf("     Please wait while opening uFR NFC reader.\n");
	printf(" --------------------------------------------------\n");

#ifdef __DEBUG
	status = ReaderOpenEx(1, PORT_NAME, 1, NULL);
#else
	status = ReaderOpen();
#endif
	if (status != UFR_OK)
	{
		printf("Error while opening device, status is: 0x%08X\n", status);
		getchar();
		return EXIT_FAILURE;
	}
//	status = ReaderReset();
//	if (status != UFR_OK)
//	{
//		ReaderClose();
//		printf("Error while opening device, status is: 0x%08X\n", status);
//		getchar();
//		return EXIT_FAILURE;
//	}
#if __WIN32 || __WIN64
	Sleep(500);
#else // if linux || __linux__ || __APPLE__
	usleep(500000);
#endif

	if (!CheckDependencies())
	{
		ReaderClose();
		getchar();
		return EXIT_FAILURE;
	}

	printf(" --------------------------------------------------\n");
	printf("        uFR NFC reader successfully opened.\n");
	printf(" --------------------------------------------------\n");

#if linux || __linux__ || __APPLE__
	_initTermios(0);
#endif
	do
	{
		while (!_kbhit())
		{
			if(card_transceive_mode)
				continue;

			status = GetCardIdEx(&sak, uid, &uid_size);
			switch (status)
			{
				case UFR_OK:
					if (card_in_field)
					{
						if (old_sak != sak || old_uid_size != uid_size || memcmp(old_uid, uid, uid_size))
						{
							old_sak = sak;
							old_uid_size = uid_size;
							memcpy(old_uid, uid, uid_size);
							NewCardInField(sak, uid, uid_size);
						}
					}
					else
					{
						old_sak = sak;
						old_uid_size = uid_size;
						memcpy(old_uid, uid, uid_size);
						NewCardInField(sak, uid, uid_size);
						card_in_field = true;
					}
					break;
				case UFR_NO_CARD:
					card_in_field = false;
					status = UFR_OK;
					break;
				default:
					ReaderClose();
					printf(" Fatal error while trying to read card, status is: 0x%08X\n", status);
					getchar();
#if linux || __linux__ || __APPLE__
					_resetTermios();
					tcflush(0, TCIFLUSH); // Clear stdin to prevent characters appearing on prompt
#endif
					return EXIT_FAILURE;
			}
#if __WIN32 || __WIN64
			Sleep(300);
#else // if linux || __linux__ || __APPLE__
			usleep(300000);
#endif
		}

		key = _getch();
		menu(key);
	}
	while (key != '\x1b');

	ReaderClose();
#if linux || __linux__ || __APPLE__
	_resetTermios();
	tcflush(0, TCIFLUSH); // Clear stdin to prevent characters appearing on prompt
#endif
	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------------
void menu(char key)
{

	switch (key)
	{
		case '1':
			enter_transceive();
			break;

		case '2':
			exit_transceive();
			break;

		case '3':
			set_user_zone();
			break;

		case '4':
			write_user_zone();
			break;

		case '5':
			read_user_zone();
			break;

		case '\x1b':
			break;

		default:
			usage();
			break;
	}
}
//------------------------------------------------------------------------------
void usage(void)
{
		printf(" +------------------------------------------------+\n"
			   " |             TRANSCEIVE EXAMPLE                 |\n"
			   " |             AT88RF04C COMMANDS                 |\n"
			   " |              version "APP_VERSION"             |\n"
			   " +------------------------------------------------+\n"
			   "                              For exit, hit escape.\n");
		printf(" --------------------------------------------------\n");
		printf("  (1) - Enter into transceive mode\n"
			   "  (2) - Exit from transceive mode\n"
			   "  (3) - Set User Zone\n"
			   "  (4) - Write User Zone\n"
			   "  (5) - Read User Zone\n");
}
//------------------------------------------------------------------------------
UFR_STATUS NewCardInField(uint8_t sak, uint8_t *uid, uint8_t uid_size)
{
	UFR_STATUS status;
	uint8_t dl_card_type;

	status = GetDlogicCardType(&dl_card_type);
	if (status != UFR_OK)
		return status;

	printf(" \a-------------------------------------------------------------------\n");
	printf(" Card type: %s, sak = 0x%02X, uid[%d] = ", GetDlTypeName(dl_card_type), sak, uid_size);
	print_hex_ln(uid, uid_size, ":");
	printf(" -------------------------------------------------------------------\n");

	return UFR_OK;
}
//------------------------------------------------------------------------------
void enter_transceive(void)
{
	UFR_STATUS status;

	printf(" -------------------------------------------------------------------\n");
	printf("                        Enter to transceive mode                    \n");
	printf(" -------------------------------------------------------------------\n");

	//transceive mode use HW TX_CRC and RX_CRC, RF timeout is 10000us, UART timeout is 500ms
	status = card_transceive_mode_start(1, 1, 10000, 500);

	if(status)
	{
		printf("\nReader did not entered into transceive mode\n");
		printf("Error code = %02X\n", status);
		card_transceive_mode = FALSE;
	}
	else
	{
		printf("\nReader is in transceive mode\n");
		card_transceive_mode = TRUE;
	}
}
//------------------------------------------------------------------------------

void exit_transceive(void)
{
	UFR_STATUS status;

	printf(" -------------------------------------------------------------------\n");
	printf("                    Exit from transceive mode                       \n");
	printf(" -------------------------------------------------------------------\n");

	status = card_transceive_mode_stop();

	if(status)
	{
		printf("\nReader did not exit from transceive mode\n");
		printf("Error code = %02X\n", status);
	}
	else
	{
		printf("\nReader exit from transceive mode\n");
		card_transceive_mode = FALSE;
	}
}
//------------------------------------------------------------------------------

void set_user_zone(void)
{
	UFR_STATUS status;
	unsigned char cmd_buff[20];
	unsigned char rcv_buff[20];
	uint32_t rcv_len;

	printf(" -------------------------------------------------------------------\n");
	printf("                           Set user zone                            \n");
	printf(" -------------------------------------------------------------------\n");

	//CID = 0, zone number = 0, anti-tearing off
	cmd_buff[0] = 0x01;
	cmd_buff[1] = 0x00;

	status = uart_transceive(cmd_buff, 2, rcv_buff, 3, &rcv_len);

	if(status)
	{
		printf("\nCard communication error\n");
		printf("Error code = %02X\n", status);
		printf("Receive length = %d\n", rcv_len);
		if(rcv_len > 0)
		{
			printf("Card reply = \n");
			print_hex_ln(rcv_buff, rcv_len, " ");
		}
	}
	else
	{
		printf("\nCard reply = \n");
		print_hex_ln(rcv_buff, rcv_len, " ");
	}
}
//------------------------------------------------------------------------------

void write_user_zone(void)
{
	UFR_STATUS status;
	unsigned char cmd_buff[20];
	unsigned char rcv_buff[20];
	uint32_t rcv_len;
	unsigned char i;

	printf(" -------------------------------------------------------------------\n");
	printf("                         Write user zone                            \n");
	printf(" -------------------------------------------------------------------\n");

	//CID = 0, zone number = 0, start address = 0, anti-tearing off, 16 data for writing
	cmd_buff[0] = 0x03;
	cmd_buff[1] = 0x00;
	cmd_buff[2] = 0x00;
	cmd_buff[3] = 15;
	for(i = 0; i < 16; i++)
		cmd_buff[4 + i] = i;

	status = uart_transceive(cmd_buff, 20, rcv_buff, 3, &rcv_len);

	if(status)
	{
		printf("\nCard communication error\n");
		printf("Error code = %02X\n", status);
		printf("Receive length = %d\n", rcv_len);
		if(rcv_len > 0)
		{
			printf("Card reply = \n");
			print_hex_ln(rcv_buff, rcv_len, " ");
		}
	}
	else
	{
		printf("\nCard reply = \n");
		print_hex_ln(rcv_buff, rcv_len, " ");
	}
}
//------------------------------------------------------------------------------

void read_user_zone(void)
{
	UFR_STATUS status;
	unsigned char cmd_buff[20];
	unsigned char rcv_buff[20];
	uint32_t rcv_len;

	printf(" -------------------------------------------------------------------\n");
	printf("                         Read user zone                            \n");
	printf(" -------------------------------------------------------------------\n");

	//CID = 0, zone number = 0, start address = 0, anti-tearing off, 16 data for reading
	cmd_buff[0] = 0x02;
	cmd_buff[1] = 0x00;
	cmd_buff[2] = 0x00;
	cmd_buff[3] = 15;

	status = uart_transceive(cmd_buff, 4, rcv_buff, 16 + 3, &rcv_len);

	if(status)
	{
		printf("\nCard communication error\n");
		printf("Error code = %02X\n", status);
		printf("Receive length = %d\n", rcv_len);
		if(rcv_len > 0)
		{
			printf("Card reply = \n");
			print_hex_ln(rcv_buff, rcv_len, " ");
		}
	}
	else
	{
		printf("\nCard reply = \n");
		print_hex_ln(rcv_buff, rcv_len, " ");
	}
}
