#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "common_constants.h"
#include <list>
#include <vector>
#include <iostream>
#include <io.h>
#include <fcntl.h>
using namespace std;

//Представляет игрока.
class player
{
	//уникальный идентификатор, назначается уже после создания экземпляра класса
	int uid;
	//сокет, на который отправляются уведомления во время игры
	//SOCKET* game_socket;
	char* name;
public:
	player(char* _name);
	~player();
	
	char* get_name();
	void set_uid(int _uid);
	int get_uid();
};

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
};

class game_map
{
	//номер карты
	int id;
	//список узлов
	vector<node*> nodes;
	//число узлов на одном уровне карты
	int level_size;

public:
	game_map(int _id, int _level_size);
	~game_map();

	int get_id();
	int get_level_size();
	//добавляет узел в карту
	void add_node(node* _node);
	//получить узел по индексу, нумерация начинается с нуля
	node* get_node(int index);
	//получить число узлов в карте
	int nodes_count();
	
	//инициализировать карту по ее номеру; возможно, временный метод
	void initialize(int id);

	virtual void output_map();
	//отображает один из пяти рядов комнаты (0-4); ширина - 11 ячеек
protected:
	virtual void output_node_row(node* _node, node* _left, node* _right, int row);
};