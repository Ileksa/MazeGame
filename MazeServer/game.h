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
	int players_count;
	int max_players_count;

public:
	game(int _level_size);
	virtual ~game();

	//int get_id();
	int get_level_size();
	//добавляет узел в карту
	void add_node(node* _node);
	//получить узел по индексу, нумерация начинается с нуля
	node* get_node(int index);
	//получить число узлов в карте
	int get_nodes_count();

	//получить число игроков на карте
	int get_players_count();
	//добавить игрока в игру, в случа неуспеха возвращается -1
	int add_player(player* pl);
	//удаляет игрока из игры
	int remove_player(player* pl);

	virtual void output_map();
	//отображает один из пяти рядов комнаты (0-4); ширина - 11 ячеек
protected:
	virtual void output_node_row(node* _node, node* _left, node* _right, int row);


	//перебор игроков на карте
protected:
	int index_in_node = -1;
	int index_node = -1;
public:
	//возвращает игрока
	player* current_player();
	node* current_node();
	//возвращает true, если удалось успешно передвинуться
	bool move_next();
	//функция устанавливает указатель на первого игрока; если игроков вообще нет, возвращает false
	bool reset_players_iterator();

};