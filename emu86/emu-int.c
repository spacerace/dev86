
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "emu-proc.h"
#include "emu-serial.h"
#include "emu-int.h"


// BIOS video services

void int_10h ()
	{
	byte_t ah = reg8_get (REG_AH);
	switch (ah)
		{
		// Write character at current cursor position

		case 0x0A:
			serial_send (reg8_get (REG_AL));
			break;

		default:
			assert (0);
		}
	}


// BIOS keyboard services

void int_16h ()
	{
	byte_t c = 0;

	byte_t ah = reg8_get (REG_AH);
	switch (ah)
		{
		// Extended keyboard read

		case 0x10:
			c = serial_recv ();
			reg8_set (REG_AL, (byte_t) c);  // ASCII code
			reg8_set (REG_AH, 0);           // No scan code
			break;

		default:
			assert (0);
		}
	}


// Interrupt handle table

typedef void (* int_hand_t) ();

struct int_num_hand_s
	{
	byte_t num;
	int_hand_t hand;
	};

typedef struct int_num_hand_s int_num_hand_t;

int_num_hand_t _int_tab [] = {
		{ 0x10, int_10h },
		{ 0x16, int_16h },
		{ 0,    NULL    }
	};


int int_hand (byte_t i)
	{
	int err = -1;

	int_num_hand_t * desc = _int_tab;

	while (1)
		{
		byte_t num = desc->num;
		int_hand_t hand = desc->hand;

		if (!num && !hand) break;

		if (num == i)
			{
			(*hand) ();
			err = 0;
			break;
			}

		desc++;
		}

	return err;
	}
