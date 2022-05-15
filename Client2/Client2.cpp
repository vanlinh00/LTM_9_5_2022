
#include <stdio.h>
#include <WinSock2.h>
#include <wchar.h>
#include <comdef.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("192.168.1.207");
	addr.sin_port = htons(8000);
	system("pause");

	int ret = connect(server, (SOCKADDR*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		printf("Ket noi khong thanh cong - %d\n", ret);
		return 1;
	}


	char buf[256];
	while (1)
	{
		printf("Nhap xau ky tu: ");
		fgets(buf, sizeof(buf), stdin);

		if (strncmp(buf, "exit", 4) == 0)
			break;

		//const char* msg = "<192.168.1.207> <777>\n";
		//send(server, msg, strlen(msg), 0);

		send(server, buf, strlen(buf), 0);

		ret = recv(server, buf, sizeof(buf), 0);
		if (ret <= 0)
			break;

		// Them ky tu ket thuc xau va in ra man hinh
		buf[ret] = 0;
		printf("Du lieu tu server: %s\n", buf);
	}


	closesocket(server);
	WSACleanup();



}