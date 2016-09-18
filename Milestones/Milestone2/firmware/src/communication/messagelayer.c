/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "messagelayer.h"
#include <stdio.h>


static STATES parserstate;
static size_t internalBufferIndex = 0;

bool ParseMessage(char messageData[], char buf[]) {
	if (messageData == NULL) {
		dbgOutputBlock(false);
	}
	memset(messageData, 0, MAXMESSAGESIZE);
	if (buf[0] == STARTOFTEXT && buf[1] == WHATPICAMI) {
		/*Get Message Count*/
		uint8_t messagecount = buf[2];
		/*Check Message Count Here*/

		/*Message*/
		uint16_t datalength = buf[3] << 8 | buf[4];
		size_t i = 0;
		/*The reason we have the i + 5 is because we want to skip the added characters when we packed the message*/
		for (i = 0; i < datalength; i++) {
			messageData[i] = buf[i + 5];
		}
		messageData[i] = '\0';
		char readchecksum = buf[i + 5];
		/*compute check sum and return early if error*/
		if (readchecksum != checksum(buf)) {
			return false;
		}
		return buf[++i + 5] == ENDOFTEXT;
	}
	else {
		/*Discard the message*/
		return false;
	}
}

bool ParseMessage(char c, char data[], size_t& size) {
	switch (parserstate) {
	case IDLE_STATE: {
		size = 0;
		memset(data, 0, MAXMESSAGESIZE);
		size = 0;
		if (c == STARTOFTEXT) {
			parserstate = CHECK_DESTINATION_CHAR;
		}
		return false;
	}
	case CHECK_DESTINATION_CHAR: {
		if (c == WHATPICAMI) {
			parserstate = CHECK_MESSAGE_COUNT;
		}
		else {
			parserstate = IDLE_STATE;
		}
		return false;
	}
	case CHECK_MESSAGE_COUNT: {
		parserstate = GET_DATALENGTH_UPPER;
		return false;
	}
	case GET_DATALENGTH_UPPER: {
		size = c << 8;
		parserstate = GET_DATALENGTH_LOWER;
		return false;
	}
	case GET_DATALENGTH_LOWER: {
		size = size | c;
		parserstate = GET_DATA;
		return false;
	}
	case GET_DATA: {
		data[internalBufferIndex] = c;
		internalBufferIndex++;
		if (internalBufferIndex == size - 1) {
			parserstate = CHECK_ENDCHAR;
		}
		return false;
	}
	case CHECK_ENDCHAR: {
		parserstate = IDLE_STATE;
		return c == ENDOFTEXT;
	}
	}
}

void CreateMessage(char buf[], char messageData[], char destination) {
	if (messageData == NULL) {
		dbgOutputBlock(false);
	}
	memset(buf, 0, MAXMESSAGESIZE);
	// Pack message into string 'buf'
	// Format: start byte, destination, message count, data length (upper 8 bits), data length (lower 8 bits), data, checksum
	size_t len = strlen(messageData);
	sprintf(buf, "%c%c%c%c%c%s",
		STARTOFTEXT,
		destination,
		37, (len & 0xFF00) >> 8,
		len & 0x00FF,
		messageData);

	int index = len + 5; //length of the string + start of text, destination, message count, data length
	// Calculate checksum of message
	buf[index] = checksum(buf);
	buf[index + 1] = ENDOFTEXT;
}

// Simple string checksum
char checksum(char* s)
{
	signed char sum = -1;
	while (*s != 0)
	{
		sum += *s;
		s++;
	}
	return sum;
}

/* *****************************************************************************
 End of File
 */
