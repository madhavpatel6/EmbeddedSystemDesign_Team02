#include <stdio.h>
#include <string.h>
#include "messageTest.h"

static STATES parserstate;
static int internalBufferIndex = 0;
static char internalMessageData[MAXMESSAGESIZE];
static size_t _internalMessageSize = 0;
static char internalCheckSum;
static size_t size = 0;

enum Tests {
	droppedBytes1, droppedBytes2, extraBytes, droppedMessages, invalidSrc, invalidDest 
} test;

bool ParseMessage(char c, char data[], char* source, char* messageCount, bool *isError) {
	switch (parserstate) {
		case IDLE_STATE: {
	        *isError = false;
			internalBufferIndex = 0;
			internalCheckSum = '\0';
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
			else if(c == SEARCHERMOVER || c == PATHFINDER || c == TARGETLOCATOR || c == TARGETGRABBER) {
				parserstate = IDLE_STATE;
			}
	        else {
	        	fprintf(stderr,"\nTEST PASSED: ERROR CAUGHT - Invalid destination character\n");
	            *isError = true;
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
	            	fprintf(stderr,"\nTEST PASSED: ERROR CAUGHT - Invalid source character\n");
	                *isError = true;
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
	        	fprintf(stderr,"\nTEST PASSED: ERROR CAUGHT - Invalid data length\n");
	            parserstate = IDLE_STATE;
	            *isError = true;
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
	        if(c == STARTOFTEXT) {
	        	fprintf(stderr,"\nTEST PASSED: ERROR CAUGHT - Missing bytes, reached STX too early\n");
	            *isError = true;
	            parserstate = CHECK_DESTINATION_CHAR;
	        }
	        if(c == ENDOFTEXT) {
	        	fprintf(stderr,"\nTEST PASSED: ERROR CAUGHT - Missing bytes, reached EOT too early\n");
	            *isError = true;
	            parserstate = IDLE_STATE;
	        }
	        // dbgOutputLoc(AFTER_THIRD_IF_GET_DATA_PARSEMESSAGE_MESSAGE_C);
			return false;
		}
		case GET_CHECK_SUM: {
	        internalCheckSum = c;
			if (internalCheckSum != checksum(data)) {
				fprintf(stderr,"\nTEST PASSED: ERROR CAUGHT - Checksum incorrect\n");
	            *isError = true;
				parserstate = IDLE_STATE;
			} else {
				parserstate = CHECK_ENDCHAR;
			}
			return false;
		}
		case CHECK_ENDCHAR: {
	        parserstate = IDLE_STATE;
	        *isError = c != ENDOFTEXT;
	        if (*isError) {
	        	fprintf(stderr,"\nTEST PASSED: ERROR CAUGHT - No EOT character found\n");
	        }
			return !(*isError);
		}
	}
}

int CreateMessage(char buf[], char messageData[], char destination, char messagecount) {
	if (messageData == NULL) {
	}

	memset(buf, 0, MAXMESSAGESIZE);
	// Pack message into string 'buf'
	// Format: start byte, destination, source, message count, data length (upper 8 bits), data length (lower 8 bits), data, checksum
	int len = strlen(messageData);

	switch(test) {
		case droppedBytes1: {
			char badMessage[50];
		    int i;
		    int len = strlen(messageData);

		    /* remove rem'th char from word */
		    for (i = 0; i < len - 1; i++) {
		    	badMessage[i] = messageData[i + 5];
		    }

	    	if(i < len) {
		    	badMessage[i] = '\0';
		    }
		    
			return sprintf(buf, "%c%c%c%c%c%c%s%c%c",
				STARTOFTEXT,
				destination,
				MYMODULE,
				messagecount,
		        (len & 0xFF00) >> 8,
				len & 0x00FF,
				badMessage,
				checksum(messageData),
				ENDOFTEXT
				);
			break;	
		}
		case droppedBytes2: {
			char badMessage[50];
		    int i;
		    int len = strlen(messageData);

		    /* remove rem'th char from word */
		    for (i = 0; i < len - 1; i++) {
		    	badMessage[i] = messageData[i + 1];
		    }

	    	if(i < len) {
		    	badMessage[i] = 0x02;
		    	badMessage[i+1] = '\0';
		    }
		    
			return sprintf(buf, "%c%c%c%c%c%c%s%c%c",
				STARTOFTEXT,
				destination,
				MYMODULE,
				messagecount,
		        (len & 0xFF00) >> 8,
				len & 0x00FF,
				badMessage,
				checksum(messageData),
				ENDOFTEXT
				);
			break;	
		}
		case extraBytes: {
			char badMessage[70];
		    int i;
		    int j;
		    int len = strlen(messageData);

		    /* remove rem'th char from word */
		    for (i = 0; i < len - 1; i++) {
		    	badMessage[i] = messageData[i];
		    }
		    for (j = 0; j < len - 1; j++) {
		    	badMessage[i] = messageData[j];
		    	i++;
		    }

	    	if(i < len) {
		    	badMessage[i] = '\0';
		    }

			return sprintf(buf, "%c%c%c%c%c%c%s%c%c",
				STARTOFTEXT,
				destination,
				MYMODULE,
				messagecount,
		        (len & 0xFF00) >> 8,
				len & 0x00FF,
				badMessage,
				checksum(messageData),
				ENDOFTEXT
				);
			break;			
		}
		case droppedMessages: {
			return sprintf(buf, "%c%c%c%c%c%c%s%c%c",
				STARTOFTEXT,
				destination,
				MYMODULE,
				messagecount,
		        (len & 0xFF00) >> 8,
				len & 0x00FF,
				messageData,
				checksum(messageData),
				ENDOFTEXT
				);
			break;	
		}
		case invalidSrc: {
			return sprintf(buf, "%c%c%c%c%c%c%s%c%c",
				STARTOFTEXT,
				destination,
				'w',
				messagecount,
		        (len & 0xFF00) >> 8,
				len & 0x00FF,
				messageData,
				checksum(messageData),
				ENDOFTEXT
				);
			break;	
		}
		case invalidDest: {
			return sprintf(buf, "%c%c%c%c%c%c%s%c%c",
				STARTOFTEXT,
				destination,
				MYMODULE,
				messagecount,
		        (len & 0xFF00) >> 8,
				len & 0x00FF,
				messageData,
				checksum(messageData),
				ENDOFTEXT
				);
			break;	
		}
		default:
			return sprintf(buf, "%c%c%c%c%c%c%s%c%c",
				STARTOFTEXT,
				destination,
				MYMODULE,
				messagecount,
		        (len & 0xFF00) >> 8,
				len & 0x00FF,
				messageData,
				checksum(messageData),
				ENDOFTEXT
				);
			break;	
	}
}

// Simple string checksum
char checksum(char* s)
{
    // dbgOutputLoc(ENTER_CHECKSUM_MESSAGE_C);
    char* temp = s;
	signed char sum = -1;
	while (*temp != 0)
	{
		sum += *temp;
		temp++;
	}
    // dbgOutputLoc(LEAVE_CHECKSUM_MESSAGE_C);
	return sum;
}

int main (void) {
	char c;
	char packedMessage[MAXMESSAGESIZE];
	char messageData[] = "{'type':'test','items':'test'}";
	int length = 0;
	bool isError = false;
	char source = 'p';
	char messageCount = '0';
	int i;

	//*******************************************************************************/
	test = droppedBytes1;

	fprintf(stderr,"\n\nTest %d: Dropped bytes - data missing\n",test);
	fprintf(stderr,"------------------------------------------------------------\n");
	fprintf(stderr,"Expected: Error - EOT in data");
	length = CreateMessage(packedMessage, messageData, PATHFINDER, messageCount);

	for (i = 0; i < length; i++) {
		c = packedMessage[i];
		if(ParseMessage(c, internalMessageData, &source, &messageCount, &isError)) {
    		fprintf(stderr,"Valid message\n");
	    }
	}

	//*******************************************************************************/
	test = droppedBytes2;

	fprintf(stderr,"\n\nTest %d: Dropped bytes - data missing\n",test);
	fprintf(stderr,"------------------------------------------------------------\n");
	fprintf(stderr,"Expected: Error - incomplete message");
	length = CreateMessage(packedMessage, messageData, PATHFINDER, messageCount);

	for (i = 0; i < length; i++) {
		c = packedMessage[i];
		if(ParseMessage(c, internalMessageData, &source, &messageCount, &isError)) {
    		fprintf(stderr,"Valid message\n");
	    }
	}

	//*******************************************************************************/
	test = extraBytes;

	fprintf(stderr,"\n\nTest %d: Extra bytes - too much data\n",test);
fprintf(stderr,"------------------------------------------------------------\n");
	fprintf(stderr,"Expected: Error - checksum mismatch");
	length = CreateMessage(packedMessage, messageData, PATHFINDER, messageCount);

	for (i = 0; i < length; i++) {
		c = packedMessage[i];
		if(ParseMessage(c, internalMessageData, &source, &messageCount, &isError)) {
    		fprintf(stderr,"Valid message\n");
	    }
	}

	//*******************************************************************************/
	test = droppedMessages;

	fprintf(stderr,"\n\nTest %d: Dropped messages - message count mismatch\n",test);
	fprintf(stderr,"------------------------------------------------------------\n");
	fprintf(stderr,"Expected: Valid message - report dropped messages\n");
	length = CreateMessage(packedMessage, messageData, PATHFINDER, 3);

	for (i = 0; i < length; i++) {
		c = packedMessage[i];
		if(ParseMessage(c, internalMessageData, &source, &messageCount, &isError)) {
    		fprintf(stderr,"Valid message\n");
    		if (messageCount != 0) {
    			fprintf(stderr,"There were %d dropped messages\n", messageCount - 0);
    		}
	    }
	}
	fprintf(stderr,"TEST PASSED\n");

	//*******************************************************************************/
	test = invalidSrc;

	fprintf(stderr,"\n\nTest %d: Invalid source - invalid source character\n",test);
	fprintf(stderr,"------------------------------------------------------------\n");
	fprintf(stderr,"Expected: Error - invalid source");
	length = CreateMessage(packedMessage, messageData, PATHFINDER, 3);

	for (i = 0; i < length; i++) {
		c = packedMessage[i];
		if(ParseMessage(c, internalMessageData, &source, &messageCount, &isError)) {
    		fprintf(stderr,"Valid message\n");
    		if (messageCount != 0) {
    			fprintf(stderr,"There were %d dropped messages\n", messageCount - 0);
    		}
	    }
	}

	//*******************************************************************************/
	test = invalidDest;

	fprintf(stderr,"\n\nTest %d: Invalid destination - invalid destination character\n",test);
	fprintf(stderr,"------------------------------------------------------------\n");
	fprintf(stderr,"Expected: Error - invalid destination");
	length = CreateMessage(packedMessage, messageData, 'f', 3);

	for (i = 0; i < length; i++) {
		c = packedMessage[i];
		if(ParseMessage(c, internalMessageData, &source, &messageCount, &isError)) {
    		fprintf(stderr,"Valid message\n");
    		if (messageCount != 0) {
    			fprintf(stderr,"There were %d dropped messages\n", messageCount - 0);
    		}
	    }
	}

	return 0;
}
