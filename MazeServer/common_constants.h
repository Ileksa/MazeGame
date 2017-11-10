#pragma once
#define PORT_COMMUNICATION 1006
#define PORT_NOTIFICATION 1007
#define TIMEOUT_SECS 300
#define MSG_SIZE 1024
#define IP_BUF_SIZE 20
#define COMMAND_LEN 4 
#define NICKNAME_LEN 20
#define GAMENAME_LEN 20
#define PLAYERS_COUNT 6000 //������� id �� ������������, �� 1 �� 9999 ������������
#define STANDARD_PLAYERS_COUNT_ON_MAP 6
#define GAMES_COUNT 1000
#define DEFAULT_GAME_COUNT 2

//��������� �������
#define STATE_WAIT_HELO 0
#define STATE_WORKING 1
#define STATE_IN_GAME 2
#define STATE_QUIT 3

enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};