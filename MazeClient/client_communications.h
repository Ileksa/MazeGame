#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "client_constants.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <conio.h>
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
void output_header_games_info();
void output_game_info(int uid, int count, int max_count, char* name);
void output_delimiter_game_info();
void output_unexpected_error();

//��������� ������-----------------------------------------------
//start [num] -> ����� ���������� ������� STAR [num]; � ������ ������ ���������� -1, � ������ ������ - 0
int process_start_command(SOCKET s, wstring message, int uid);
//������������ ��������� ������, �������� � ����������
int process_game_commands(SOCKET s, int uid, int start_node);
//���������� -1 � ������ ������
int process_list_command(SOCKET s);

//������ ������� ������ ---------------------------------------------------
//�������� �� �������� ����������� ������!
struct notifications_data
{
	SOCKET s;
	game* g;
};
//�������� �� ���� ��������� �� ��������� notifications_data
DWORD WINAPI process_notifications(LPVOID data);