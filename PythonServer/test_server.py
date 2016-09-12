#!/usr/bin/env python3
import socket
from threading import Thread
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.bind(("192.168.12.1", 2000))
serversocket.listen(5)
while( True):
  connection, address = serversocket.accept()
  def receiver():
    buf1 = connection.recv(4096)
    while (True):
      if len(buf1) > 0:
          print("Data Received : ", buf1)
  rt = Thread(target=receiver)
