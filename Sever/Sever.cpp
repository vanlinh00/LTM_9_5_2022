#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")

SOCKET clients[64];
int numClients = 0;

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET*)lpParam;
	int ret;
	char buf[256];
	// Nhan du lieu tu client va in ra man hinh
	while (1)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
			break;
		buf[ret] = 0;
		printf("Du lieu nhan duoc: %s\n", buf);
		int length = strcspn(buf, "\0");
		for (int i = 0; i < numClients; i++)
		{
			if (clients[i] == client)
			{
				continue;
			}
			send(clients[i], buf, length, 0);
		}

	}
	closesocket(client);
}
int main()
{
	// Khoi tao thu vien
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	// Tao socket
	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Khai bao dia chi server
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8000);
	// Gan cau truc dia chi voi socket
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	// Chuyen sang trang thai cho ket noi
	listen(listener, 5);
	while (1)
	{
		clients[numClients] = accept(listener, NULL, NULL);
		printf("Client moi ket noi: %d\n", clients[numClients]);

		CreateThread(0, 0, ClientThread, &clients[numClients], 0, 0);
		numClients++;
	}
}