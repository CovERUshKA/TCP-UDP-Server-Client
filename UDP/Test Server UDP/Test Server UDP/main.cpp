#include <iostream>
#include <WS2tcpip.h>
#include <Windows.h>
#include <time.h>

#pragma comment (lib, "ws2_32.lib")

#define Message(msg) MessageBoxA(GetConsoleWindow(), msg, "Test Server UDP - Error", MB_OK);
#define PORT XXXXX

using namespace std;

HINSTANCE hInst;
HWND Mainhwnd, ListOfUsers, EditMessage, SendPacketButton;
WNDCLASSEXW wc = { 0 };
WSADATA wsData;

char* ConvertToString(int number)
{
	char buf[MAX_PATH];
	sprintf_s(buf, "%d", number);
	return buf;
}

int main()
{
	setlocale(LC_ALL, "");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// Initialize winsock

	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);

	if (wsOk != NULL)
	{
		Message("Can't initialize winsock! Quiting");
		cout << "Can't initialize winsock! Quiting" << endl << endl;
		return NULL;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_DGRAM, NULL);
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
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	char port[NI_MAXHOST];
	char ipofclient[NI_MAXHOST];
	char buf[4096];
	char time[26];

	while (true)
	{
		ZeroMemory(port, NI_MAXHOST);
		ZeroMemory(ipofclient, NI_MAXHOST);
		ZeroMemory(buf, 4096);
		ZeroMemory(time, 26);
		// Wait for client to send data
		int bytesreceived = recvfrom(listening, buf, 4096, NULL, (sockaddr*)&client, &clientSize);

		_strtime_s(time);
		inet_ntop(AF_INET, &client.sin_addr, ipofclient, NI_MAXHOST);

		cout << "Message from " << ipofclient << ":" << ntohs(client.sin_port) << "     [" << time << "]" << endl;
		//Show info
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "String received: " << endl << "-------------" << endl << buf << endl << "-------------" << endl;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "Bytes received: " << bytesreceived << endl << endl;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}

	// Close the socket
	closesocket(listening);

	// Cleanup winsock
	WSACleanup();

	return NULL;
}