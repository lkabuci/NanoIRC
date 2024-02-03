import socket
import concurrent.futures

def connect_to_server(client_id):
    server_address = ('localhost', 2000)
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client_socket.connect(server_address)
        #print(f"Client {client_id} connected to {server_address}")

        client_socket.sendall("PASS 22".encode('utf-8'))
        client_socket.sendall(f"NICK F{client_id}".encode('utf-8'))
        client_socket.sendall(f"USER l l l l".encode('utf-8'))
        client_socket.sendall(f"JOIN #test".encode('utf-8'))
        client_socket.sendall(f"PRIVMSG #test AAAAAAAAAA".encode('utf-8'))
        data = client_socket.recv(1024)
        print(f"Client {client_id} received: {data.decode('utf-8')}")

    except Exception as e:
        print(f"Client {client_id} error: {e}")

    finally:
        #client_socket.close()
        
        pass
        #print(f"Client {client_id} socket closed")

def main():
    with concurrent.futures.ThreadPoolExecutor(max_workers=1000) as executor:
        # Connect 1000 clients concurrently
        futures = [executor.submit(connect_to_server, i) for i in range(1, 1001)]

        # Wait for all connections to complete
        concurrent.futures.wait(futures)

if __name__ == "__main__":
    main()
