#include <iostream>
#include <winsock2.h>
#include <utility>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include "bfs_task.h"
#include "thread_pool.h"

const int PORT = 8080;



pair<long long, long long> do_bfs(const long long table_size, const long long thread_count)
{
    vector<vector<int>> table(table_size, vector<int>(table_size, 0));
	BFSTask solo_task(table);
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    solo_task.fill(1);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    long long miliseconds_normal_fill = chrono::duration_cast<chrono::microseconds>(end - begin).count();

    cout << miliseconds_normal_fill << endl;

    vector<vector<int>> table2(table_size, vector<int>(table_size, 0));

    ThreadPool thread_pool(thread_count);
	for (int i =0; i < thread_count + 1; i++)
	{
		thread_pool.push_job(BFSTask(table2));
	}
    begin = chrono::steady_clock::now();
	thread_pool.start();
    thread_pool.wait_work();
	thread_pool.stop();
    end = chrono::steady_clock::now();

    long long miliseconds_thread_bfs = chrono::duration_cast<chrono::microseconds>(end - begin).count();

    cout <<  miliseconds_thread_bfs << endl;
    return {miliseconds_normal_fill, miliseconds_thread_bfs};
}


int main()
{
    do_bfs(5000, 16);
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != NO_ERROR)
    {
        return EXIT_FAILURE;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET)
    {
        WSACleanup();
        return EXIT_FAILURE;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (const sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        closesocket(server_socket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    if (listen(server_socket, 10) == SOCKET_ERROR)
    {
        closesocket(server_socket);
        WSACleanup();
        return EXIT_FAILURE;
    }


    SOCKET client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET)
    {
        closesocket(server_socket);
        WSACleanup();
        return EXIT_FAILURE;
    }


    while(true)
    {
        long long numbers[2];
        int bytesRead = recv(client_socket, (char*)numbers, sizeof(numbers), 0);

        if (bytesRead == SOCKET_ERROR)
        {
            closesocket(client_socket);
            closesocket(server_socket);
            WSACleanup();
            return EXIT_FAILURE;
        }

        pair<long long, long long> result_pair = do_bfs(numbers[0], numbers[1]);
        long long result[2] = {result_pair.first, result_pair.second};
        send(client_socket, (char*)result, sizeof(result), 0);
    }
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return EXIT_SUCCESS;
}

