#!/usr/bin/env python
import socketserver as ss

class LeetTCPHandler(ss.BaseRequestHandler):
    def handle(self):
        self.data = self.request.recv(1024).strip()
        print(self.data)
        if self.data == b'VERSION':
            self.request.sendall(b"Numeric Serial Server Validation System 2.1a")
        elif self.data[:6] == b'SERIAL':
            self.request.sendall(b"SERIAL_VALID=1")
        else:
            self.request.sendall(b"Invalid command")


if __name__ == "__main__":
    HOST, PORT = "localhost", 1337

    server = ss.TCPServer((HOST, PORT), LeetTCPHandler)

    server.serve_forever()
