

import subprocess
# subprocess.run(["ls"],["-l"])
# subprocess.run(["echo", "-l"])

import socket
sock = socket.socket()
sock2 = socket.socket()
# sock.connect(("192.168.12.196", 2000))

sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR, 1);
sock2.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR, 1);

sock.bind(("", 2000))
sock2.bind(("", 2001))
sock.listen(5)
sock2.listen(5)
client_sock, address = sock.accept()
print("conn0 complete")
client2_sock, addy2 = sock2.accept()
print("conn1 complete")
while(True):
    try:
        data = client_sock.recv(512)
        print(data)
    except:
        client_sock, address = sock.accept()
        print("attempting to reconnect 2000");
    if(data):
        try:
            client2_sock.send(data)
        except:
            client2_sock, addy2 = sock2.accept()
            print("attempting to reconnect 2001");
    else:
        break

