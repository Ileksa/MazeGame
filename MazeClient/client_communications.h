#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "client_constants.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "../MazeServer/game.h"

using namespace std;
//�������� ���� � �������� � ����� ������� buf_size; message - ����� ����������� �� �����
int get_ip(char* ip_buf, int buf_size);
//������� ������� ��� �������� \r\n ������������ � �������� �� � �����; � ������ ������������ ������ ���������� -1, ����� - ����� ������
//� ������ ���������� ����������� ���������� -2
int get_command(SOCKET s, char* buf, int buf_size);
//������� ��� ������������ � �������� ��� � �����
int get_nickname(char* buf, int buf_size);
//���������, �������� �� ������� "�� ���������".
bool is_ok_command(char* message, int msg_size);

//��������� ������� �� ������, 0 � ������ ���������� ������ ������������ 
int send_command(SOCKET s, char* buf, int buf_size);

//����� ������� ������-------------------------------------------
void output_header(char* nickname, int uid);
void output_menu();

//��������� ������-----------------------------------------------
//start [num] -> ����� ���������� ������� STAR [num]; � ������ ������ ���������� -1, � ������ ������ - 0
int process_start_command(SOCKET s, wstring message, int uid);


//������ ������� ������ ---------------------------------------------------
//�������� �� �������� ����������� ������!
struct notifications_data
{
	SOCKET s;
	game* g;
};
//�������� �� ���� ��������� �� ��������� notifications_data
DWORD WINAPI process_notifications(LPVOID data);