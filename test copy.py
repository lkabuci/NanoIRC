import socket
import concurrent.futures
import random
def connect_to_irc(client_id):
    server_address = ('localhost', 2000)
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client_socket.connect(server_address)
        print(f"Client {client_id} connected to IRC server at {server_address}")

        # Sending an example IRC message
        nickname = f"Client{random.randrange(1, 4214)}"
        client_socket.sendall(f"PASS 22\r\n".encode('utf-8'))
        client_socket.sendall(f"USER {nickname} 0 * :{nickname}\r\n".encode('utf-8'))
        client_socket.sendall(f"NICK {nickname}\r\n".encode('utf-8'))
        client_socket.sendall(f"JOIN #test\r\n".encode('utf-8'))
        #client_socket.sendall(f"PRIVMSG #test hello\r\n".encode('utf-8'))
        print(f"Client {client_id} sent IRC registration")

        # Receive and print response (you may need to handle responses in a more sophisticated way)
        data = client_socket.recv(1024)
        print(f"Client {client_id} received: {data.decode('utf-8')}")

    except Exception as e:
        print(f"Client {client_id} error: {e}")

    finally:
        pass
        #client_socket.close()
        #print(f"Client {client_id} socket closed")

def main():
    with concurrent.futures.ThreadPoolExecutor(max_workers=3) as executor:
        # Connect 10 clients to the IRC server concurrently
        futures = [executor.submit(connect_to_irc, i) for i in range(1, 4)]

        # Wait for all connections to complete
        concurrent.futures.wait(futures)

if __name__ == "__main__":
    main()
