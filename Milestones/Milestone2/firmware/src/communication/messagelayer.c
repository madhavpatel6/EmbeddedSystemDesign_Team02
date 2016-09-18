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



static STATES parserstate;
static uint32_t internalBufferIndex = 0;
static bool badMessage = false;
static char internalCheckSum;

bool ParseMessage(char c, char data[], size_t* size) {
	switch (parserstate) {
	case IDLE_STATE: {
		internalBufferIndex = 0;
		internalCheckSum = NULL;
		badMessage = false;
		*size = 0;
		memset(data, 0, MAXMESSAGESIZE);
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
			badMessage = true;
		}
		return false;
	}
	case CHECK_MESSAGE_COUNT: {
		parserstate = GET_DATALENGTH_UPPER;
		return false;
	}
	case GET_DATALENGTH_UPPER: {
		*size = c << 8;
		parserstate = GET_DATALENGTH_LOWER;
		return false;
	}
	case GET_DATALENGTH_LOWER: {
		*size = *size | c;
		parserstate = GET_DATA;
		return false;
	}
	case GET_DATA: {
		data[internalBufferIndex] = c;
		internalBufferIndex++;
		if (internalBufferIndex == *size) {
			parserstate = GET_CHECK_SUM;
		}
		return false;
	}
	case GET_CHECK_SUM: {
		internalCheckSum = c;
		if (internalCheckSum != checksum(data)) {
			badMessage = true;
		}
		parserstate = CHECK_ENDCHAR;
		return false;
	}
	case CHECK_ENDCHAR: {
		parserstate = IDLE_STATE;
		return c == ENDOFTEXT && !badMessage;
	}
	}
}

/**
 * Create a message using our format
 * @param buf destination for message
 * @param messageData data for the message
 * @param destination character for who message is sent to
 * @return length of the message
 */
int createMessage(char buf[], char messageData[], char destination) {
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
	buf[index] = checksum(messageData);
	buf[index + 1] = ENDOFTEXT;
    
    return index + 2;
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
