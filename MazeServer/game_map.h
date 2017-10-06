#pragma once
#include <list>
#include <vector>
using namespace std;

//Представляет игрока.
class player
{
	//уникальный идентификатор игрока на сервере.
	int uid;
public:
	player(int _uid);
	~player();

	int get_uid();
	void set_uid(int _uid);

	
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
	node(int _id, int _up, int _down, int _left, int _right, 
		int _up_left, int _up_right, int _down_left, int _down_right);
	~node();

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
};

class game_map
{
	//номер карты
	int id;
	//список узлов
	vector<node*> nodes;

public:
	game_map(int _id);
	~game_map();

	int get_id();
	//добавляет узел в карту
	void add_node(node* _node);
	//получить узел по индексу
	node* get_node(int index);
	//получить число узлов в карте
	int node_count();
};
