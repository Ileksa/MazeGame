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
	//����� �����
	//int id;
	//������ �����
	vector<node*> nodes;
	//����� ����� �� ����� ������ �����
	int level_size;

	//������ ���� �������
	vector<player*> players;
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
	vector<player*>::iterator get_players_iterator_begin();
	vector<player*>::iterator get_players_iterator_end();
	//�������� ����, � ������� ��������� ����� (��������� nullptr, ���� �� �������)
	node* get_player_node(player* pl);
	node* get_player_node(int uid);

	//�������� ������ � ����, � ����� �������� ������������ -1
	int add_player(player* pl);
	int add_player_to_node(player* pl, int node_num);
	//������� ������ �� ����
	int remove_player(player* pl);
	int remove_player(int uid);

	virtual void output_map();
	//���������� ���� �� ���� ����� ������� (0-4); ������ - 11 �����
protected:
	virtual void output_node_row(node* _node, node* _left, node* _right, int row);

};