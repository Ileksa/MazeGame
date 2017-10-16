#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "node.h"
#include <list>
#include <vector>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <winsock2.h>
using namespace std;


class game
{
	//номер карты
	//int id;
	//список узлов
	vector<node*> nodes;
	//число узлов на одном уровне карты
	int level_size;

	//список всех игроков
	vector<player*> players;
	int max_players_count;

public:
	game(int _level_size);
	virtual ~game();

	//int get_id();
	int get_level_size();
	void set_level_size(int size);
	//добавляет узел в карту
	void add_node(node* _node);
	//получить узел по индексу, нумерация начинается с нуля
	node* get_node(int index);
	//получить число узлов в карте
	int get_nodes_count();

	//получить число игроков на карте
	int get_players_count();
	//получить игрока по индексу
	vector<player*>::iterator get_players_iterator_begin();
	vector<player*>::iterator get_players_iterator_end();
	//получить узел, в котором находится игрок (возвращет nullptr, если не удалось)
	node* get_player_node(player* pl);
	node* get_player_node(int uid);

	//добавить игрока в игру, в случа неуспеха возвращается -1
	int add_player(player* pl);
	int add_player_to_node(player* pl, int node_num);
	//удаляет игрока из игры
	int remove_player(player* pl);
	int remove_player(int uid);

	virtual void output_map();
	//отображает один из пяти рядов комнаты (0-4); ширина - 11 ячеек
protected:
	virtual void output_node_row(node* _node, node* _left, node* _right, int row);

};