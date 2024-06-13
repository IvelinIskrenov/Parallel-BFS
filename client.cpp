#include <iostream>
#include <winsock2.h>

const char* SERVER_IP = "127.0.0.1";
const int PORT = 8080;

int read_bytes_from_socket(SOCKET& s, char *buf, int len, int flags)
{
    int bytes_read = recv(s, buf, len, 0);

    if (bytes_read == SOCKET_ERROR)
    {
        closesocket(s);
        WSACleanup();
        return SOCKET_ERROR;
    }
    return 0;
}


int main() {
    WSADATA was_data;
    if (WSAStartup(MAKEWORD(2, 2), &was_data) != 0)
    {
        return EXIT_FAILURE;
    }

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET)
    {
        WSACleanup();
        return EXIT_FAILURE;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        closesocket(client_socket);
        WSACleanup();
        return EXIT_FAILURE;
    }
    while (true)
    {
        long long numbers[2];
        std::cin >> numbers[0] >> numbers[1];
        if (numbers[0] <= 0 || numbers[1] <= 0)
        {
            break;
        }
        send(client_socket, (char*)numbers, sizeof(numbers), 0);

        long long result[2];
        int bytes_read = read_bytes_from_socket(client_socket, (char*)result, sizeof(result), 0);
        if (bytes_read == SOCKET_ERROR)
        {
            closesocket(client_socket);
            return EXIT_FAILURE;
        }

        std::cout  << result[0] << " microseconds" << std::endl;
        std::cout  << result[1] << " microseconds" << std::endl;
    }
    closesocket(client_socket);
    WSACleanup();

    return EXIT_SUCCESS;
}