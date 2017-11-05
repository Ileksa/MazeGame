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

//ѕредставл€ет игрока.
class player
{
	//уникальный идентификатор, назначаетс€ уже после создани€ экземпл€ра класса
	int uid;
	//сокет, на который отправл€ютс€ уведомлени€ во врем€ игры; если не настроен, то равен -1
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

	//установить или получить сокет дл€ уведомлений
	void set_socket_notifications(SOCKET s);
	SOCKET get_socket_notifications();
};