/**********************************************************************
Name		: Example UDP Client
Author		: Sloan Kelly
Date		: 2017-12-16
Purpose		: Example of a bare bones UDP client

***********************************************************************/

#include <iostream>
#include <WS2tcpip.h>
#include <Windows.h>

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

#define Message(msg) MessageBoxA(GetConsoleWindow(), msg, "Test Client - Error", MB_OK);
#define PORT XXXXX
#define IP "XXX.XXX.X.XX"

// Saves us from typing std::cout << etc. etc. etc.
using namespace std;

int main() // We can pass in a command line option!! 
{
	////////////////////////////////////////////////////////////
	// INITIALIZE WINSOCK
	////////////////////////////////////////////////////////////

	// Structure to store the WinSock version. This is filled in
	// on the call to WSAStartup()
	WSADATA data;

	// To start WinSock, the required version must be passed to
	// WSAStartup(). This server is going to use WinSock version
	// 2 so I create a word that will store 2 and 2 in hex i.e.
	// 0x0202
	WORD version = MAKEWORD(2, 2);

	// Start WinSock
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		// Not ok! Get out quickly
		Message("Can't start Winsock!");
		cout << "Can't start Winsock!" << endl;
		return NULL;
	}

	char host[NI_MAXHOST];
	char service[NI_MAXHOST];
	char ipofclient[NI_MAXHOST];

	////////////////////////////////////////////////////////////
	// CONNECT TO THE SERVER
	////////////////////////////////////////////////////////////

	// Create a hint structure for the server
	sockaddr_in server;
	server.sin_family = AF_INET; // AF_INET = IPv4 addresses
	server.sin_port = htons(PORT); // Little to big endian conversion
	if (inet_pton(AF_INET, IP, &server.sin_addr) <= 0)	// Convert from string to byte array
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
	int server_len = sizeof(server);

	// Socket creation, note that the socket type is datagram
	SOCKET out = socket(AF_INET, SOCK_DGRAM, NULL);

	// Write out to that socket
	char bufofmsg[4096]("Hi");
		int sendOk = sendto(out, bufofmsg, strlen(bufofmsg) + 1, NULL, (sockaddr*)&server, sizeof(server));

		if (sendOk == SOCKET_ERROR)
		{
			Message("sendOk == SOCKET_ERROR");
			shutdown(out, SD_SEND);
			closesocket(out);
			WSACleanup();
			return NULL;
		}
		char buf[4096];
		ZeroMemory(buf, 4096);
		if (recvfrom(out, buf, 4096, NULL, (struct sockaddr*)&server, &server_len) <= NULL)
		{
			cout << "recvfrom(out, buf, 4096, NULL, (struct sockaddr*)&server, &client_len) <= NULL" << endl;
		}

		ZeroMemory(host, NI_MAXHOST);
		ZeroMemory(service, NI_MAXHOST);
		ZeroMemory(ipofclient, NI_MAXHOST);

		if (getnameinfo((sockaddr*)&server, sizeof(server), host, NI_MAXHOST, service, NI_MAXSERV, NULL) == NULL)
		{
			inet_ntop(AF_INET, &server.sin_addr, ipofclient, NI_MAXHOST);
			cout << "String received from " << host << "(" << ipofclient << ":" << service << ") - " << buf << endl;
		}
		else
		{
			inet_ntop(AF_INET, &server.sin_addr, ipofclient, NI_MAXHOST);
			cout << "String received from " << ipofclient << ":" << service << " - " << buf << endl;
		}

	cout << "Complete" << endl;
	system("PAUSE");

	// Close the socket
	closesocket(out);

	// Close down Winsock
	WSACleanup();

	return NULL;
}