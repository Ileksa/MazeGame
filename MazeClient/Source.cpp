#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (WSAGetLastError() != 0) {
		printf("Creating socket error: %d\n", WSAGetLastError());
		return 1;
	}

	sockaddr_in tcpaddr;
	tcpaddr.sin_family = AF_INET;
	tcpaddr.sin_port = htons(1006);
	inet_pton(AF_INET, "10.7.13.144", &tcpaddr.sin_addr.s_addr);

	connect(s, (SOCKADDR*)&tcpaddr, sizeof(tcpaddr));
	if (WSAGetLastError() != 0) {
		printf("Connection error: %d\n", WSAGetLastError());
		goto end_of_the_program;
	}

	char buf[256] = "Hello, Computer!";

	send(s, buf, strlen(buf), 0);
	if (WSAGetLastError() != 0) {
		printf("Sending error: %d\n", WSAGetLastError());
		goto end_of_the_program;
	}
	printf("Client sent message: %s\n", buf);

	recv(s, buf, sizeof(buf), 0);
	if (WSAGetLastError() != 0) {
		printf("Receiving error: %d\n", WSAGetLastError());
		goto end_of_the_program;
	}
	printf("Client received message: %s\n", buf);

end_of_the_program:
	closesocket(s);
	system("pause");
	return 0;
}
