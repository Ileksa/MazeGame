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
	int players_count;
	int max_players_count;

public:
	game(int _level_size);
	virtual ~game();

	//int get_id();
	int get_level_size();
	//��������� ���� � �����
	void add_node(node* _node);
	//�������� ���� �� �������, ��������� ���������� � ����
	node* get_node(int index);
	//�������� ����� ����� � �����
	int get_nodes_count();

	//�������� ����� ������� �� �����
	int get_players_count();
	//�������� ������ � ����, � ����� �������� ������������ -1
	int add_player(player* pl);
	//������� ������ �� ����
	int remove_player(player* pl);

	virtual void output_map();
	//���������� ���� �� ���� ����� ������� (0-4); ������ - 11 �����
protected:
	virtual void output_node_row(node* _node, node* _left, node* _right, int row);


	//������� ������� �� �����
protected:
	int index_in_node = -1;
	int index_node = -1;
public:
	//���������� ������
	player* current_player();
	node* current_node();
	//���������� true, ���� ������� ������� �������������
	bool move_next();
	//������� ������������� ��������� �� ������� ������; ���� ������� ������ ���, ���������� false
	bool reset_players_iterator();

};