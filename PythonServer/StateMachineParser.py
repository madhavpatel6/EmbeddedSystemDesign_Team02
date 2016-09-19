
from enum import Enum

class States(Enum):
    IDLE_STATE = 1
    CHECK_DESTINATION_CHAR = 2
    CHECK_SOURCE_CHAR = 3
    CHECK_MESSAGE_COUNT = 4
    GET_DATALENGTH_UPPER = 5
    GET_DATALENGTH_LOWER = 6
    GET_DATA = 7
    GET_CHECK_SUM = 8
    CHECK_ENDCHAR = 9

class Parser:
    def __init__(self):
        self.currentState = States.IDLE_STATE
        self.message = ""
        self.messagelength = 0
        self.messageindex = 0

    def parse(self, c):
        if self.currentState == States.IDLE_STATE:
            self.message = ""
            self.messagelength = 0
            self.messageindex = 0
            if c == 0x02:

                self.currentState = States.CHECK_DESTINATION_CHAR
            return False
        elif self.currentState == States.CHECK_DESTINATION_CHAR:
            print("Destination: %c", c)
            self.currentState = States.CHECK_SOURCE_CHAR
            return False
        elif self.currentState == States.CHECK_SOURCE_CHAR:
            print("Source: %c", c)
            self.currentState = States.CHECK_MESSAGE_COUNT
            return False
        elif self.currentState == States.CHECK_MESSAGE_COUNT:
            print("Message Count: %d", c)
            self.currentState = States.GET_DATALENGTH_UPPER
            return False
        elif self.currentState == States.GET_DATALENGTH_UPPER:
            self.messagelength = c << 8
            self.currentState = States.GET_DATALENGTH_LOWER
            return False
        elif self.currentState == States.GET_DATALENGTH_LOWER:
            self.messagelength = c | self.messagelength
            self.currentState = States.GET_DATA
            return False
        elif self.currentState == States.GET_DATA:
            self.message += c
            self.messageindex += 1
            if self.messagelength == self.messageindex:
                self.currentState = States.GET_CHECK_SUM
            return False
        elif self.currentState == States.GET_CHECK_SUM:
            self.currentState = States.CHECK_ENDCHAR
            return False
        elif self.currentState == States.CHECK_ENDCHAR:
            self.currentState = States.IDLE_STATE
            return True


def main():
    ps = Parser()
    if ps.parse(0x02):
        print(ps.message)


if __name__ == "__main__": main()