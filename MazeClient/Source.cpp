#define _CRT_SECURE_NO_WARNINGS
#include "client_constants.h"
#include "client_communications.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main() {
	int res;
	char* ip_buf = new char[IP_BUF_SIZE];
	char* nickname = new char[NICKNAME_LEN];
	char* buf = new char[MSG_SIZE];


	WSADATA wsaData;
	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		wcout << L"WSAStartup failed: " << res << endl;
		return 1;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (WSAGetLastError() != 0) {
		wcout << L"Creating socket error: " << WSAGetLastError() << endl;
		return 1;
	}
	
	sockaddr_in tcpaddr;
	tcpaddr.sin_family = AF_INET;
	tcpaddr.sin_port = htons(1006);


	while(true)
	{
		get_ip(ip_buf, IP_BUF_SIZE);
		int res = inet_pton(AF_INET, ip_buf, &tcpaddr.sin_addr.s_addr);
		if (res > 0)
			break;
		else
			wcout << L"Invalid IPv4 address." << endl;
	}

	connect(s, (SOCKADDR*)&tcpaddr, sizeof(tcpaddr));
	if (WSAGetLastError() != 0) {
		wcout << L"Connection error: " << WSAGetLastError() << endl;
		system("pause");
		return 0;
	}

	while (true) {
		res = get_nickname(nickname, NICKNAME_LEN);
		if (res >= 0)
			break;
	}
	
	//получить greetings message
	if (get_command(s, buf, MSG_SIZE) < 0)
	{
		wcout << L"Can not connect to the server" << endl;
		goto end_of_the_program;
	}
	
	sprintf(buf, "HELO %s\r\n", nickname);
	//buf[MSG_SIZE - 1] = '\0';
	send(s, buf, strlen(buf), 0);

	get_command(s, buf, MSG_SIZE);
	if (!is_ok_command(buf, strlen(buf)))	{
		wcout << buf << endl;
		goto end_of_the_program;
	}

	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	int pos = strlen(buf) - 1;
	buf[pos] = '\0'; //затереть точку в конце
	char* index = strstr(buf, "UID is");
	int uid = atoi(index + 7);	

	int result;
	while(true)
	{
		system("cls");
		output_header(nickname, uid);
		output_menu();
		wstring str;
		transform(str.begin(), str.end(), str.begin(), tolower);
		getline(wcin, str);
		if (str.compare(L"quit") == 0) {
			goto end_of_the_program;
		} else if (str.substr(0, 6).compare(L"start ") == 0)	{
			process_start_command(s, str, uid);
		}
	}


	wcout << L"╚╗╚═╝ ╚═╝╔╝" << endl;

	strncpy(buf, "QUIT\r\n", 256);
	buf[255] = '\0';
	send(s, buf, strlen(buf), 0);
	if (WSAGetLastError() != 0) {
		wcout << L"Sending error: " << WSAGetLastError() << endl;
		goto end_of_the_program;
	}
	wcout << L"Client sent message: " << buf;

	recv(s, buf, sizeof(buf), 0);
	if (WSAGetLastError() != 0) {
		wcout << L"Receiving error: " << WSAGetLastError() << endl;
		goto end_of_the_program;
	}
	wcout << L"Client received message: " << buf;



end_of_the_program:
	char command[MSG_SIZE] = "QUIT\r\n";
	result = send_command(s, command, strlen(command));
	delete[] ip_buf;
	delete[] nickname;
	delete[] buf;
	closesocket(s);
	system("pause");
	return 0;
}
