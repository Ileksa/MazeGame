#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "common_constants.h"
#include <list>
#include <vector>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <winsock2.h>
using namespace std;

//������������ ������.
class player
{
	//���������� �������������, ����������� ��� ����� �������� ���������� ������
	int uid;
	//�����, �� ������� ������������ ����������� �� ����� ����; ���� �� ��������, �� ����� -1
	SOCKET socket_notifications;
	char* name;
	int color;
	int points;
public:
	player(char* _name);
	~player();

	char* get_name();
	void set_uid(int _uid);
	int get_uid();

	int get_color();
	void set_color(int color);
	
	void add_point();
	void set_points(int value);
	int get_points();

	//���������� ��� �������� ����� ��� �����������
	void set_socket_notifications(SOCKET s);
	SOCKET get_socket_notifications();
};