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
	//�����, �� ������� ������������ ����������� �� ����� ����
	//SOCKET* socket_notifications;
	char* name;
public:
	player(char* _name);
	~player();

	char* get_name();
	void set_uid(int _uid);
	int get_uid();

	//���������� ��� �������� ����� ��� �����������
	//void set_socket_notifications(SOCKET* s);
	//SOCKET* get_socket_notifications();
};