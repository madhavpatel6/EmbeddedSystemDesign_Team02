#!/usr/bin/env python3

import socket
sock = socket.socket()
sock2 = socket.socket()

# allow ports to be reused, helpful when programs crash and restart
sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR, 1);

sock.bind(("", 2000))
print("bound")
sock.listen(5)
print("listen")
client_sock, address = sock.accept()
print("conn0 complete")
while(True):
    try:
        data = client_sock.recv(512)
        print(data)
    except:
        client_sock, address = sock.accept()
        print("attempting to reconnect 2000");
    if(data):
        try:
            client_sock.send(data)
        except:
            client_sock, address = sock.accept()
            print("attempting to reconnect 2000");
    else:
        break

