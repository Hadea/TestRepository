#include <iostream>
#include <string>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")


int main()
{
	// should be project wide known
	std::string targetIP = "127.0.0.1"; // loopback virtual device
	int targetPort = 54000;

	// Initialize WinSock
	WSADATA WinSockData;
	int WinSockError;
	WinSockError = WSAStartup(0x0202, &WinSockData);

	if (WinSockError)
	{
		std::cerr << "Error init socket";
		return WinSockError;
	}


	// create socket
	SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listeningSocket == INVALID_SOCKET)
	{
		std::cerr << "Error create socket \n";
		return WSAGetLastError();
	}

	//bind to address and port on socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(targetPort); // host to network short (pc little endian, net big endien)
	inet_pton(AF_INET, targetIP.c_str(), &hint.sin_addr);

	// connect
	int connError = connect(listeningSocket, (sockaddr*)&hint, sizeof(hint));
	
	if (connError == SOCKET_ERROR)
	{
		std::cerr << "Cannot connect to server";
		closesocket(listeningSocket);
		WSACleanup();
		return WSAGetLastError();
	}

	char buffer[4096];
	std::string userInput;
	std::cout << "Server verbunden, bitte texte eingeben!\n";

	do
	{
		getline(std::cin, userInput);
		if (userInput.size()) // für if gilt 0 = false, alles andere ist true
		{
			if (userInput == "exit")
				break;

			if (send(listeningSocket, userInput.c_str(), userInput.size() + 1, 0) != SOCKET_ERROR)
			{
				// send successful
				ZeroMemory(buffer, 4096);
				int bytesRecieved = recv(listeningSocket, buffer, 4096, 0);
				if (bytesRecieved > 0)
				{
					std::cout << "echo: " << std::string(buffer, 0, bytesRecieved) << "\n";
				}
			}

		}

	} while (true);
	
	
	//close socket	

	closesocket(listeningSocket);

	//shutdown connection
	WSACleanup();


}
