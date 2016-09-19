#include <stdio.h>
#include <iostream.h>
#include "messageTest.h"

static STATES parserstate;
static uint32_t internalBufferIndex = 0;
static char internalCheckSum;

enum Tests {
	droppedBytes, extraBytes, droppedMessages, messageRate
} test;

bool ParseMessage(char c, char data[], size_t* size) {
	switch (parserstate) {
	case IDLE_STATE: {
		internalBufferIndex = 0;
		internalCheckSum = NULL;
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
			parserstate = IDLE_STATE;
		}
		return false;
	}
    case CHECK_SOURCE_CHAR: {
        switch(c) {
            case SEARCHERMOVER: {
                break;
            }
            case TARGETLOCATOR: {
                break;
            }
            case PATHFINDER: {
                break;
            }
            case TARGETGRABBER: {
                break;
            }
            default: {
                parserstate = IDLE_STATE;
                break;
            }
        }
    }
	case CHECK_MESSAGE_COUNT: {
		parserstate = GET_DATALENGTH_UPPER;
        //Fill here with check
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
}

int CreateMessage(char buf[], char messageData[], char destination) {
	if (messageData == NULL) {
		//dbgOut/putBlock(false);
	}
	memset(buf, 0, MAXMESSAGESIZE);
	// Pack message into string 'buf'
	// Format: start byte, destination, source, message count, data length (upper 8 bits), data length (lower 8 bits), data, checksum
	size_t len = strlen(messageData);

	switch(test) {
		case droppedBytes: {
			return sprintf(buf, "%c%c%c%c%c%c%s%c%c",
				STARTOFTEXT,
				destination,
				WHATPICAMI,
				37, (len & 0xFF00) >> 8,
				len & 0x00FF,
				messageData,
				checksum(messageData),
				ENDOFTEXT
				);
			break;		
		}
		case extraBytes: {
			break;			
		}
		case droppedMessages: {
			break;			
		}
		case messageRate: {
			break;			
		}
	}
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

int main (void) {
	char packedMessage[MAXMESSAGESIZE];
	char messageData[] = "{"type":"test","items":"test"}";
	int length = 0;

	test = droppedBytes;
	length = CreateMessage(packedMessage, messageData, PATHFINDER);

	if(ParseMessage(c, _internalMessageData, &_internalMessageSize)) {
    	cout << "Valid message";
    } else {
    	cout << "Message error: Dropped bytes detected";
    }

	test = extraBytes;
	length = CreateMessage(packedMessage, messageData, PATHFINDER);

	if(ParseMessage(c, _internalMessageData, &_internalMessageSize)) {
            /*Since we returned true we assume the message is valid*/
    }

	test = droppedMessages;
	length = CreateMessage(packedMessage, messageData, PATHFINDER);

	if(ParseMessage(c, _internalMessageData, &_internalMessageSize)) {
            /*Since we returned true we assume the message is valid*/
    }

	test = messageRate;
	length = CreateMessage(packedMessage, messageData, PATHFINDER);	

	if(ParseMessage(c, _internalMessageData, &_internalMessageSize)) {
            /*Since we returned true we assume the message is valid*/
    }

	return 0
}