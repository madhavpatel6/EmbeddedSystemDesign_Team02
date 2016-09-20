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
#include "messages.h"
#include "debug.h"


static STATES parserstate = IDLE_STATE;
static size_t internalBufferIndex = 0;
static char internalCheckSum = 0;;
static size_t size = 0;

bool ParseMessage(char c, char data[], char* source, char* messageCount) {
    dbgOutputLoc(RXTHREAD_ENTER_PARSER);
	switch (parserstate) {
	case IDLE_STATE: {
		internalBufferIndex = 0;
		internalCheckSum = NULL;
		size = 0;
		memset(data, 0, MAXMESSAGESIZE);
		if (c == STARTOFTEXT) {
			parserstate = CHECK_DESTINATION_CHAR;
		}
		return false;
	}
	case CHECK_DESTINATION_CHAR: {
		if (c == MYMODULE) {
			parserstate = CHECK_SOURCE_CHAR;
		}
		else {
			parserstate = IDLE_STATE;
		}
		return false;
	}
    case CHECK_SOURCE_CHAR: {
        switch(c) {
            case SEARCHERMOVER: {
                *source = SEARCHERMOVER;
                break;
            }
            case TARGETLOCATOR: {
                *source = TARGETLOCATOR;
                break;
            }
            case PATHFINDER: {
                *source = PATHFINDER;
                break;
            }
            case TARGETGRABBER: {
                *source = TARGETGRABBER;
                break;
            }
            default: {
                parserstate = IDLE_STATE;
                return false;
            }
        }
        parserstate = CHECK_MESSAGE_COUNT;
        return false;
    }
	case CHECK_MESSAGE_COUNT: {
		parserstate = GET_DATALENGTH_UPPER;
        //Pass the message count back
        *messageCount = c;
		return false;
	}
	case GET_DATALENGTH_UPPER: {
		size = (c & 0xFF) << 8;
		parserstate = GET_DATALENGTH_LOWER;
		return false;
	}
	case GET_DATALENGTH_LOWER: {
		size = size | (c & 0x00FF);
        if(size > (MAXMESSAGESIZE - 8)) {
            parserstate = IDLE_STATE;
            return false;
        }
		parserstate = GET_DATA;
		return false;
	}
	case GET_DATA: {
		data[internalBufferIndex] = c;
		internalBufferIndex = internalBufferIndex + 1;
		if (internalBufferIndex >= size || internalBufferIndex >= MAXMESSAGESIZE) {
			parserstate = GET_CHECK_SUM;
            internalBufferIndex = 0;
		}
        else if(c == STARTOFTEXT) {
            parserstate = CHECK_DESTINATION_CHAR;
        }
        else if(c == ENDOFTEXT) {
            parserstate = IDLE_STATE;
        }
		return false;
	}
	case GET_CHECK_SUM: {
		internalCheckSum = c;
		if (internalCheckSum != checksum(data)) {
			parserstate = IDLE_STATE;
		}
		parserstate = CHECK_ENDCHAR;
		return false;
	}
	case CHECK_ENDCHAR: {
		parserstate = IDLE_STATE;
		return c == ENDOFTEXT;
	}
	}
    dbgOutputLoc(RXTHREAD_LEAVE_PARSER);
}

/**
 * Create a message using our format
 * @param buf destination for message
 * @param messageData data for the message
 * @param destination character for who message is sent to
 * @return length of the message
 */
int CreateMessage(char buf[], char messageData[], char destination) {
	if (messageData == NULL) {
		//dbgOut/putBlock(false);
	}
	memset(buf, 0, MAXMESSAGESIZE);
	// Pack message into string 'buf'
	// Format: start byte, destination, source, message count, data length (upper 8 bits), data length (lower 8 bits), data, checksum
	size_t len = strlen(messageData);
	return sprintf(buf, "%c%c%c%c%c%c%s%c%c",
		STARTOFTEXT,
		destination,
		MYMODULE,
		37, (len & 0xFF00) >> 8,
		len & 0x00FF,
		messageData,
		checksum(messageData),
		ENDOFTEXT
		);
}

// Simple string checksum
char checksum(char* s)
{
    char* temp = s;
	signed char sum = -1;
	while (*temp != 0)
	{
		sum += *temp;
		temp++;
	}
	return sum;
}

/* *****************************************************************************
 End of File
 */
