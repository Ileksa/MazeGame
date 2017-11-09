#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "node.h"
#include "common_constants.h"
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <winsock2.h>
using namespace std;


class game
{
	//название карты
	char* name;
	//список узлов
	vector<node*> nodes;
	//число узлов на одном уровне карты
	int level_size;

	//список всех игроков
	map<player*, node*> players;
	//vector<player*> players;
	int max_players_count;

	//доступные цвета
	vector<ConsoleColor> available_colors;

public:
	game(int _level_size);
	virtual ~game();
	void set_game_name(char* _name);
	char* get_game_name();

	//int get_id();
	int get_level_size();
	void set_level_size(int size);
	//добавляет узел в карту; невозможно добавить узел на карту, в которой уже есть игроки
	int add_node(node* _node);
	//получить узел по индексу, нумерация начинается с нуля
	node* get_node(int index);
	//получить число узлов в карте
	int get_nodes_count();

	//получить число игроков на карте
	int get_players_count();
	int get_max_players_count();
	//получить игрока по индексу
	map<player*, node*>::iterator get_players_iterator_begin();
	map<player*, node*>::iterator get_players_iterator_end();
	//получить узел, в котором находится игрок (возвращет nullptr, если не удалось)
	node* get_player_node(player* pl);
	node* get_player_node(int uid);

	vector<player*> get_players_at_node(int node);

	//добавить игрока в игру, в случа неуспеха возвращается -1
	int add_player(player* pl, int color = -1);
	int add_player_to_node(player* pl, int node_num, int color = -1);
	//удаляет игрока из игры
	int remove_player(player* pl);
	int remove_player(int uid);

	//возвращает -1 в случае неудачи
	int set_player_node(player* pl, int new_node);
	int set_player_node(int uid, int new_node);


	virtual void output_map();
	virtual void output_stat();
	//отображает игрока в заданном узле при условии, что левый верхний угол карты задан точками (x,y)
	virtual void output_players_at_node(int node_num, int x = 0, int y = 0);


	//уведомляет других игроков о перемещении игрока с заданными uid с узла from к узлу to
	int notify_players_move(int uid, int from, int to);
	int notify_players_quit(int uid, int from);
	int notify_players_join(player* pl);
	int notify_players_pnts(player* pl);

	//извлечь доступный цвет
	int pop_available_color();
protected:
	//отображает один из двух рядов игроков в узле
	virtual void output_players_at_node_one_row(HANDLE hStdOut, int node, bool is_upper_row);

	virtual void output_node_row(node* _node, node* _left, node* _right, int row, vector<player*> pls);
	virtual void set_available_colors();
};