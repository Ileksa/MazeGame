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
	//�������� �����
	char* name;
	//������ �����
	vector<node*> nodes;
	//����� ����� �� ����� ������ �����
	int level_size;

	//������ ���� �������
	map<player*, node*> players;
	//vector<player*> players;
	int max_players_count;

	//��������� �����
	vector<ConsoleColor> available_colors;

public:
	game(int _level_size);
	virtual ~game();
	void set_game_name(char* _name);
	char* get_game_name();

	//int get_id();
	int get_level_size();
	void set_level_size(int size);
	//��������� ���� � �����; ���������� �������� ���� �� �����, � ������� ��� ���� ������
	int add_node(node* _node);
	//�������� ���� �� �������, ��������� ���������� � ����
	node* get_node(int index);
	//�������� ����� ����� � �����
	int get_nodes_count();

	//�������� ����� ������� �� �����
	int get_players_count();
	int get_max_players_count();
	//�������� ������ �� �������
	map<player*, node*>::iterator get_players_iterator_begin();
	map<player*, node*>::iterator get_players_iterator_end();
	//�������� ����, � ������� ��������� ����� (��������� nullptr, ���� �� �������)
	node* get_player_node(player* pl);
	node* get_player_node(int uid);

	vector<player*> get_players_at_node(int node);

	//�������� ������ � ����, � ����� �������� ������������ -1
	int add_player(player* pl, int color = -1);
	int add_player_to_node(player* pl, int node_num, int color = -1);
	//������� ������ �� ����
	int remove_player(player* pl);
	int remove_player(int uid);

	//���������� -1 � ������ �������
	int set_player_node(player* pl, int new_node);
	int set_player_node(int uid, int new_node);


	virtual void output_map();
	virtual void output_stat();
	//���������� ������ � �������� ���� ��� �������, ��� ����� ������� ���� ����� ����� ������� (x,y)
	virtual void output_players_at_node(int node_num, int x = 0, int y = 0);


	//���������� ������ ������� � ����������� ������ � ��������� uid � ���� from � ���� to
	int notify_players_move(int uid, int from, int to);
	int notify_players_quit(int uid, int from);
	int notify_players_join(player* pl);
	int notify_players_pnts(player* pl);

	//������� ��������� ����
	int pop_available_color();
protected:
	//���������� ���� �� ���� ����� ������� � ����
	virtual void output_players_at_node_one_row(HANDLE hStdOut, int node, bool is_upper_row);

	virtual void output_node_row(node* _node, node* _left, node* _right, int row, vector<player*> pls);
	virtual void set_available_colors();
};