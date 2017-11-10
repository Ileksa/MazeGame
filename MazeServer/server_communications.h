#pragma once
#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
#include <io.h>
#include <cmath>
#include <fstream>
#include <fcntl.h>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "common_constants.h"
#include "game.h"


#pragma comment(lib, "Ws2_32.lib")
using namespace std;

class lsm_server
{
	struct client_communication_data
	{
		lsm_server* initiator;
		SOCKET socket;
		sockaddr_in tcpaddr;
	};

	//слушающий сокет - ожидающий подключений
	SOCKET socket_listener;
	//все игроки сервера
	player** players;
	int players_count;

	//все игры сервера
	game** games;
	int games_count; //фактическое число игр на сервере

	//добавляет игрока в таблицу игроков и возвращает его uid  в случае удачи, если неуспешно - -1
	int add_player(player* pl);
	void remove_player(player* pl);
	
	//метод, инициализирующий карты по умолчанию
	void initialize_default_games();
	//получить индекс игры в массиве игр, возвращает -1 в случае неудачи
	int get_index(game* g);

public:
	lsm_server(int port);
	~lsm_server();

	//основной серверный поток, отвечает за установление соединений с клиентами
	void accepting_clients();
	//поток, создаваемый для работы с отдельным клиентом
	DWORD WINAPI client_communication(LPVOID data);
	//обертка для работы с клиентскими потоками
	static DWORD WINAPI client_communication_wraper(LPVOID data);

	//отправить клиенту приветственное сообщение
	void send_greetings_message(SOCKET s);
	//отправить клиенту сообщение
	void send_message(SOCKET s, char* msg, int msg_size);

	//считать команду для символов \r\n включительно и записать ее в буфер; в случае переполнения буфера возвращает -1, иначе - длину строки
	//в случае прерывания подключения возвращает -2
	int get_command(SOCKET s, char* buf, int buf_size);
	//отослать сообщение об ошибке на заданный сокет
	void send_error(SOCKET s, char* msg);

	//обработка команд
	//обарабывается команду HELO username и возвращает ссылку на профиль игрока или nullptr в случае ошибки
	player* process_helo_message(SOCKET s, char* msg, int size, int* server_state);
	//обрабатывает команду QUIT и возвращает -1, если не удалось отсоединиться
	int process_quit_message(SOCKET s, char* msg, int size, int* server_state, player* pl);
	//обрабатывает команду LIST получения списка доступных карт
	int process_list_message(SOCKET s, char* msg, int size, int* server_state, player* pl);
	//обрабатывает команду STAR и возвращает game в случае успеха, иначе - nullptr
	game* process_star_message(SOCKET s, sockaddr_in tcpaddr, char* msg, int size, int* server_state, player* pl);
	//прекращает текущую игру и возвращает -1, если удалось отсоединить игрока от игры
	int process_endg_message(SOCKET s, char* msg, int size, int* server_state, player* pl, game* g);
	//обрабатывает команду движения игрока из заданной позиции
	int process_move_message(SOCKET s, char* msg, int size, int* server_state, player* pl, game* g);
	//обрабатывает команду выстрела
	int process_shot_message(SOCKET s, char* msg, int size, int* server_state, player* pl, game* g);
};