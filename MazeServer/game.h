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
	//����� �����
	//int id;
	//������ �����
	vector<node*> nodes;
	//����� ����� �� ����� ������ �����
	int level_size;

	//������ ���� �������
	map<player*, node*> players;
	//vector<player*> players;
	int max_players_count;

public:
	game(int _level_size);
	virtual ~game();

	//int get_id();
	int get_level_size();
	void set_level_size(int size);
	//��������� ���� � �����
	void add_node(node* _node);
	//�������� ���� �� �������, ��������� ���������� � ����
	node* get_node(int index);
	//�������� ����� ����� � �����
	int get_nodes_count();

	//�������� ����� ������� �� �����
	int get_players_count();
	//�������� ������ �� �������
	map<player*, node*>::iterator get_players_iterator_begin();
	map<player*, node*>::iterator get_players_iterator_end();
	//�������� ����, � ������� ��������� ����� (��������� nullptr, ���� �� �������)
	node* get_player_node(player* pl);
	node* get_player_node(int uid);

	vector<player*> get_players_at_node(int node);

	//�������� ������ � ����, � ����� �������� ������������ -1
	int add_player(player* pl);
	int add_player_to_node(player* pl, int node_num);
	//������� ������ �� ����
	int remove_player(player* pl);
	int remove_player(int uid);

	//���������� -1 � ������ �������
	int set_player_node(player* pl, int new_node);
	int set_player_node(int uid, int new_node);


	virtual void output_map();
	//���������� ���� �� ���� ����� ������� (0-4); ������ - 11 �����

	//���������� ������ ������� � ����������� ������ � ��������� uid � ���� from � ���� to
	int notify_players_move(int uid, int from, int to);
	int notify_players_quit(int uid, int from);
	int notify_players_join(player* pl);
protected:
	virtual void output_node_row(node* _node, node* _left, node* _right, int row, vector<player*> pls);

};