#pragma once
#define PORT_COMMUNICATION 1006
#define PORT_NOTIFICATION 1007
#define MSG_SIZE 1024
#define IP_BUF_SIZE 20
#define COMMAND_LEN 4 
#define NICKNAME_LEN 20
#define PLAYERS_COUNT 10000 //нулевой id не используется, от 1 до 9999 включительно
#define STANDARD_PLAYERS_COUNT_ON_MAP 8
#define GAMES_COUNT 1000

//состояния сервера
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