#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include <list>
#include <vector>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <winsock2.h>
using namespace std;


//Представляет один узел карты - одну комнату.
class node {
	//номер узла, нумерация начинается с нуля.
	int id;
	//номера соседних узлов по соответствующим направлениям.
	int up, down, left, right, up_left, up_right, down_left, down_right;
	//список игроков, в данный момент находящихся в локации
	list<player*> players;

public:
	//если узла нет, нужно установить -1
	node(int _id, int _up = -1, int _down = -1, int _left = -1, int _right = -1,
		int _up_left = -1, int _up_right = -1, int _down_left = -1, int _down_right = -1);
	~node();

	//все методы возвращают -1, если узла нет
	int get_id();
	int get_up();
	int get_down();
	int get_left();
	int get_right();
	int get_up_left();
	int get_up_right();
	int get_down_left();
	int get_down_right();

	//добавляет игрока в заданный узел
	void add_player(player* _player);
	//удаляет игрока из заданного узла по его идентификатору и возвращает на него ссылку
	player* remove_player(int uid);
	//проверяет, есть ли игроки в локации и возвращает true, если есть
	bool contains_players();
	//возвращает число игроков в узле
	int get_players_count();
	//возвращает игрока по заданному индексу
	player* get_player(int index);
};