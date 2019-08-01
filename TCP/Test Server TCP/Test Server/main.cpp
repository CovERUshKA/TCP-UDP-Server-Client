#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

#define Message(msg) MessageBoxA(GetConsoleWindow(), msg, "Test Server - Error", MB_OK)
#define PORT XXXXX

using namespace std;

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// Initialize winsock

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);

	if (wsOk != NULL)
	{
		Message("Can't initialize winsock! Quiting");
		cout << "Can't initialize winsock! Quiting" << endl << endl;
		return NULL;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, NULL);
	if (listening == INVALID_SOCKET)
	{
		Message("Can't create a socket! Quiting");
		cout << "Can't create a socket! Quiting" << endl << endl;
		return NULL;
	}

	// Bind the socket to an ip address and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	hint.sin_addr.S_un.S_addr = htons(INADDR_ANY);

	bind(listening, (sockaddr*)&hint, sizeof(hint));
	
	// Tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "Waiting for incoming connection...\n" << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	if (clientSocket < 0)
	{
		Message("Connection accept failed");
		cout << "Connection accept failed" << endl << endl;
		return 1;
	}

	char ipofclient[NI_MAXHOST];

	ZeroMemory(ipofclient, NI_MAXHOST);

	inet_ntop(AF_INET, &client.sin_addr, ipofclient, NI_MAXHOST);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "Client " << ipofclient << " connected on port " << ntohs(client.sin_port) << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	// While loop: accept and echo message bacl to client
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);

		// Wait for client to send data
		int bytesreceived = recv(clientSocket, buf, 4096, NULL);
		if (bytesreceived == SOCKET_ERROR)
		{
			Message("Error in recv()");
			cout << "Error in recv()" << endl << endl;
			break;
		}
		if (bytesreceived == NULL)
		{
			ZeroMemory(ipofclient, NI_MAXHOST);

			inet_ntop(AF_INET, &client.sin_addr, ipofclient, NI_MAXHOST);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "Client " << ipofclient << " disconnected from port " << ntohs(client.sin_port) << endl << endl;
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

			//accept connection from an incoming client
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cout << "Waiting for incoming connection...\n" << endl;
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

			ZeroMemory(ipofclient, NI_MAXHOST);

			inet_ntop(AF_INET, &client.sin_addr, ipofclient, NI_MAXHOST);
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cout << "Client " << ipofclient << " connected on port " << ntohs(client.sin_port) << endl;
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		}
		else
		{
			cout << "Bytes received: " << bytesreceived << endl;
			cout << "String received: " << buf << endl;
			// Echo message back to client
			send(clientSocket, buf, bytesreceived + 1, NULL);
		}
	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	return NULL;
}