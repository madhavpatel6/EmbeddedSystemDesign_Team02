#!/usr/bin/env python3
# Socket server in python using select function
# Created: 9/18/2016
# Server Author: Chris Cox via Online w/ modifications
# State Machine: Madhav Patel

 
import socket, select
from enum import Enum
import binascii

# Defining States for state machine
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


# Building parser object
class Parser:
    count = 0 # This represents the number of Parser Objects
    def __init__(self):
        self.currentState = States.IDLE_STATE
        self.message = str("")
        self.packedmessage = str("")
        self.messagelength = 0
        self.messageindex = 0
        Parser.count += 1

    def __del__(self):
        print("Deleted: ", self.message)
        Parser.count -= 1
        if Parser.count == 0:
            print("Last Parser object deleted")
        else:
            print("Parser objects remaining: ", Parser.count)

    def parse(self, c):
        if self.currentState == States.IDLE_STATE:
            self.packedmessage = chr(0x02)
            self.message = str()
            self.messagelength = 0
            self.messageindex = 0
            if c == 0x02:
                self.currentState = States.CHECK_DESTINATION_CHAR
            return False
        elif self.currentState == States.CHECK_DESTINATION_CHAR:
            #print("Destination: ", chr(c))
            self.packedmessage += chr(c)
            self.currentState = States.CHECK_SOURCE_CHAR
            return False
        elif self.currentState == States.CHECK_SOURCE_CHAR:
            #print("Source: ", chr(c))
            self.packedmessage += chr(c)
            self.currentState = States.CHECK_MESSAGE_COUNT
            return False
        elif self.currentState == States.CHECK_MESSAGE_COUNT:
            #print("Message Count: ", c)
            self.packedmessage += chr(c)
            self.currentState = States.GET_DATALENGTH_UPPER
            return False
        elif self.currentState == States.GET_DATALENGTH_UPPER:
            #print("Data Length Upper: ", c)
            self.messagelength = c << 8
            self.packedmessage += chr(c)
            self.currentState = States.GET_DATALENGTH_LOWER
            return False
        elif self.currentState == States.GET_DATALENGTH_LOWER:
            #print("Data Length Lower: ", c)
            self.packedmessage += chr(c)
            self.messagelength = c | self.messagelength
            #print("Total Data Length: ", self.messagelength)
            self.currentState = States.GET_DATA
            return False
        elif self.currentState == States.GET_DATA:
            self.packedmessage += chr(c)
            self.message = self.message + chr(c)
            self.messageindex += 1
            if self.messagelength == self.messageindex:
                self.currentState = States.GET_CHECK_SUM
            return False
        elif self.currentState == States.GET_CHECK_SUM:
            #print("Check Sum: ", c)
            self.packedmessage += chr(c)
            self.currentState = States.CHECK_ENDCHAR
            return False
        elif self.currentState == States.CHECK_ENDCHAR:
            #print("End Char: ", c)
            self.packedmessage += chr(c)
            self.currentState = States.IDLE_STATE
            return True

#Function to broadcast chat messages to all connected clients
def broadcast_data (sock, message):
    #Do not send the message to master socket and the client who has send us the message
    for socket in CONNECTION_LIST:
        if socket != server_socket and socket != sock :
            try :
                socket.send(message)
            except :
                # broken socket connection may be, chat client pressed ctrl+c for example
                socket.close()
                CONNECTION_LIST.remove(socket)

  
if __name__ == "__main__":
      
    CONNECTION_LIST = []    # list of socket clients
    RECV_BUFFER = 512 # Advisable to keep it as an exponent of 2
    PORT = 2000
    SERVER_ADDRESS = socket.gethostbyname(socket.gethostname())
         
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # this has no effect, why ?
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((SERVER_ADDRESS, PORT))
    server_socket.listen(10)
 
    # Add server socket to the list of readable connections
    CONNECTION_LIST.append(server_socket)
 
    print ("Server started on port ", PORT)
    print ("Server started on IP:  ", SERVER_ADDRESS)
    ps = Parser()
    while 1:
        # Get the list sockets which are ready to be read through select
        read_sockets,write_sockets,error_sockets = select.select(CONNECTION_LIST,[],[])


        for sock in read_sockets:
             
            #New connection
            if sock == server_socket:
                # Handle the case in which there is a new connection recieved through server_socket
                sockfd, addr = server_socket.accept()
                CONNECTION_LIST.append(sockfd)
                print ("Client connected: ", sockfd, addr)
                 
            #Some incoming message from a client
            else:
                # Data recieved from client, process it
                try:
                    #In Windows, sometimes when a TCP program closes abruptly,
                    # a "Connection reset by peer" exception will be thrown
                    data = sock.recv(RECV_BUFFER)
                    # echo back the client message
                    if data:
                        for c in data:
                            if ps.parse(c):
                                print("Message: ", str(ps.message))
                                print("Packed Message: ", binascii.hexlify(str(ps.packedmessage).encode('utf-8')))
                                sock.send(str(ps.packedmessage).encode('utf-8'))


                # client disconnected, so remove from socket list
                except:
                    #broadcast_data(sock, "Client (%s, %s) is offline" % addr)
                    print ("Client is offline %s  %s ", sockfd, addr)
                    sock.close()
                    CONNECTION_LIST.remove(sock)
                    del ps
                    continue
         
    server_socket.close()