#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "node.h"
#include <list>
#include <vector>
#include <map>
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
	map<player*, node*> players;
	//vector<player*> players;
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
	map<player*, node*>::iterator get_players_iterator_begin();
	map<player*, node*>::iterator get_players_iterator_end();
	//получить узел, в котором находится игрок (возвращет nullptr, если не удалось)
	node* get_player_node(player* pl);
	node* get_player_node(int uid);

	vector<player*> get_players_at_node(int node);

	//добавить игрока в игру, в случа неуспеха возвращается -1
	int add_player(player* pl);
	int add_player_to_node(player* pl, int node_num);
	//удаляет игрока из игры
	int remove_player(player* pl);
	int remove_player(int uid);

	//возвращает -1 в случае неудачи
	int set_player_node(player* pl, int new_node);
	int set_player_node(int uid, int new_node);


	virtual void output_map();
	//отображает один из пяти рядов комнаты (0-4); ширина - 11 ячеек

	//уведомляет других игроков о перемещении игрока с заданными uid с узла from к узлу to
	int notify_players_move(int uid, int from, int to);
	int notify_players_quit(int uid, int from);
	int notify_players_join(player* pl);
protected:
	virtual void output_node_row(node* _node, node* _left, node* _right, int row, vector<player*> pls);

};