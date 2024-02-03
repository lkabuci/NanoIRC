import socket
import threading

class IRCClient:
    def __init__(self, server, port, nickname, channel):
        self.server = server
        self.port = port
        self.nickname = nickname
        self.channel = channel
        self.irc_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self):
        self.irc_socket.connect((self.server, self.port))
        
        self.irc_socket.sendall(f"PASS 22\r\n".encode())
        self.irc_socket.sendall(f"USER {self.nickname} {self.nickname} {self.nickname} :{self.nickname}\r\n".encode())
        self.irc_socket.sendall(f"NICK {self.nickname}\r\n".encode())
        self.irc_socket.sendall(f"JOIN {self.channel}\r\n".encode())
        threading.Thread(target=self.receive_messages).start()

    def send_message(self, message):
        self.irc_socket.sendall(f"PRIVMSG {self.channel} :{message}\r\n".encode())

    def receive_messages(self):
        while True:
            message = self.irc_socket.recv(4096).decode()
            print(message.strip('\r\n'))

# Define your server details
server = 'localhost'
port = 2000
channel = '#my_channel'

# Create multiple clients
clients = []
for i in range(0, 1000):
    clients.append(IRCClient(server, port, f'n{i}', channel))

# Connect all clients
for client in clients:
    client.connect()

# Example: Sending message from one of the clients
clients[0].send_message("Hello from client 1!")

