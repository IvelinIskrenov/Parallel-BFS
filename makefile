all: client.exe server.exe

client.exe: client.cpp
	g++ client.cpp -o client.exe -lwsock32

server.exe: server.cpp bfs_task.o thread_pool.o
	g++ server.cpp -o server.exe  bfs_task.o  thread_pool.o -lwsock32

thread_pool.o: thread_pool.cpp bfs_task.o
	g++ -c thread_pool.cpp -o thread_pool.o bfs_task.o

bfs_task.o: bfs_task.cpp
	g++ -c bfs_task.cpp -o bfs_task.o