#include <iostream>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	// Initialize WinSock
	WSADATA WinSockData;
	int WinSockError;
	WinSockError = WSAStartup(0x0202, &WinSockData);

	if (WinSockError)
	{
		std::cerr << "Error init socket";
		return WinSockError;
	}

	//Create a socket

	SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listeningSocket == INVALID_SOCKET)
	{
		std::cerr << "Error create socket";
	}

	//bind to address and port on socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	hint.sin_port = htons(54000); // host to network short (pc little endian, net big endien)
	bind(listeningSocket, (sockaddr*)&hint, sizeof(hint));

	//listen on port and wait for client
	listen(listeningSocket, SOMAXCONN);

	//accept connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST]; // client names
	char service[NI_MAXHOST]; // port number

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << " connected to port " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected to port " << ntohs(client.sin_port) << std::endl;
	}

	//closesocket (temporary ... keep open for constant connections)
	closesocket(listeningSocket);

	char buffer[4096];

	while (true)
	{
		ZeroMemory(buffer, 4096); // clear buffer

		int bytesReceived = recv(clientSocket, buffer, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cerr << "Error recieve from socket\n";
			break;
		}
		if (bytesReceived == 0)
		{
			std::cout << "Client Disconnected\n";
			break;
		}
		send(clientSocket, buffer, bytesReceived + 1, 0);

	}

	//close socket	

	closesocket(clientSocket);

	//shutdown connection
	WSACleanup();

}
//https://www.youtube.com/watch?v=dquxuXeZXgo