#!/usr/bin/env python3
# Socket server in python using select function
# Created: 9/18/2016
# Last Updated: 11/10/16
# Server Author: Chris Cox via Online w/ modifications

 
import socket
import select
from timeit import default_timer
from enum import Enum
import binascii

# Function to broadcast messages to all connected clients (PICs)
def broadcast_data(incoming_socket, message, CONNECTION_LIST, server_socket):
    # Do not send the message to master socket and the client who has send us the message
    for s in CONNECTION_LIST:
        if s != server_socket and s != incoming_socket:
            try:
                if(message != b'*HELLO*'):
                    s.send(message)
                else:
                    socket.close()
                    CONNECTION_LIST.remove(socket)
            except:
                # broken socket connection may be, chat client pressed ctrl+c for example
                socket.close()
                CONNECTION_LIST.remove(socket)


def main():
    CONNECTION_LIST = []    # list of socket clients
    RECV_BUFFER = 512 # Advisable to keep it as an exponent of 2
    PORT = 2000
    SERVER_ADDRESS = socket.gethostbyname(socket.gethostname())
         
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # this has no effect, why ?
    #server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((SERVER_ADDRESS, PORT))
    server_socket.listen(10)
 
    # Add server socket to the list of readable connections
    CONNECTION_LIST.append(server_socket)
 
    print("Server started on port ", PORT)
    print("Server started on IP:  ", SERVER_ADDRESS)
    start = default_timer()
    while 1:
        # Get the list sockets which are ready to be read through select
        read_sockets, write_sockets, error_sockets = select.select(CONNECTION_LIST, [], [])

        for sock in read_sockets:
            # New connection
            if sock == server_socket:
                # Handle the case in which there is a new connection recieved through server_socket
                sockfd, addr = server_socket.accept()
                CONNECTION_LIST.append(sockfd)
                print ("Client connected: ", sockfd, addr)
                 
            # Some incoming message from a client
            else:
                # Data recieved from client, process it
                try:
                    # In Windows, sometimes when a TCP program closes abruptly,
                    # a "Connection reset by peer" exception will be thrown
                    data = sock.recv(RECV_BUFFER)
                    # echo back the client message
                    if data:
                        duration = default_timer() - start
                        # print("Time since last valid message: ", duration)
                        start = default_timer()
                        broadcast_data(sock, data, CONNECTION_LIST, server_socket)  # This calls to broadcast messages to all pics
                        print(data)

                # client disconnected, so remove from socket list
                except:
                    # broadcast_data(sock, "Client (%s, %s) is offline" % addr)
                    print ("Client is offline %s  %s ", sockfd, addr)
                    sock.close()
                    CONNECTION_LIST.remove(sock)
                    continue

    server_socket.close()

if __name__ == "__main__":
    main()
