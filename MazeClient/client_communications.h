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
//получить айпи и записать в буфер размера buf_size; message - текст приглашения ко вводу
int get_ip(char* ip_buf, int buf_size);
//считать команду для символов \r\n включительно и записать ее в буфер; в случае переполнения буфера возвращает -1, иначе - длину строки
//в случае прерывания подключения возвращает -2
int get_command(SOCKET s, char* buf, int buf_size);
//считать имя пользователя и записать его в буфер
int get_nickname(char* buf, int buf_size);
//проверяет, является ли команда "ок командной".
bool is_ok_command(char* message, int msg_size);

//отправить команду на сервер, 0 в случае нормальной работы возвращается 
int send_command(SOCKET s, char* buf, int buf_size);

//набор функций вывода-------------------------------------------
void output_header(char* nickname, int uid);
void output_menu();

//обработка команд-----------------------------------------------
//start [num] -> будет посылаться команда STAR [num]; в случае ошибки возвращает -1, в случае успеха - 0
int process_start_command(SOCKET s, wstring message, int uid);


//работа второго потока ---------------------------------------------------
//отвечает за закрытие переданного сокета!
struct notifications_data
{
	SOCKET s;
	game* g;
};
//получает на вход указатель на структуру notifications_data
DWORD WINAPI process_notifications(LPVOID data);