#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <io.h>
#include <cmath>
#include <fcntl.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "common_constants.h"
#include "game_map.h"


#pragma comment(lib, "Ws2_32.lib")
using namespace std;

class lsm_server
{
	struct client_communication_data
	{
		lsm_server* initiator;
		SOCKET* socket;
		sockaddr_in tcpaddr;
	};

	//��������� ����� - ��������� �����������
	SOCKET socket_listener;
	//������ ��� ����
	player** players;
	int players_count;

	//��������� ������ � ������� ������� � ���������� ��� uid  � ������ �����, ���� ��������� - -1
	int add_player(player* pl);
	void remove_player(player* pl);
public:
	lsm_server(int port);
	~lsm_server();

	//�������� ��������� �����, �������� �� ������������ ���������� � ���������
	void accepting_clients();
	//�����, ����������� ��� ������ � ��������� ��������
	DWORD WINAPI client_communication(LPVOID data);
	//������� ��� ������ � ����������� ��������
	static DWORD WINAPI client_communication_wraper(LPVOID data);

	//��������� ������� �������������� ���������
	void send_greetings_message(SOCKET s);
	//��������� ������� ���������
	void send_message(SOCKET s, char* msg, int msg_size);

	//������� ������� ��� �������� \r\n ������������ � �������� �� � �����; � ������ ������������ ������ ���������� -1, ����� - ����� ������
	//� ������ ���������� ����������� ���������� -2
	int get_command(SOCKET s, char* buf, int buf_size);
	//�������� ��������� �� ������ �� �������� �����
	void send_error(SOCKET s, char* msg);

	//��������� ������
	//������������� ������� HELO username � ���������� ������ �� ������� ������ ��� nullptr � ������ ������
	player* process_helo_message(SOCKET s, char* msg, int size, int* server_state);
	//������������ ������� QUIT � ���������� -1, ���� �� ������� �������������
	int process_quit_message(SOCKET s, char* msg, int size, int* server_state, player* pl);
};