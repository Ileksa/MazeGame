#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "common_constants.h"
#include <list>
#include <vector>
#include <iostream>
#include <io.h>
#include <fcntl.h>
using namespace std;

//������������ ������.
class player
{
	//���������� �������������, ����������� ��� ����� �������� ���������� ������
	int uid;
	//�����, �� ������� ������������ ����������� �� ����� ����
	//SOCKET* game_socket;
	char* name;
public:
	player(char* _name);
	~player();
	
	char* get_name();
	void set_uid(int _uid);
	int get_uid();
};

//������������ ���� ���� ����� - ���� �������.
class node {
	//����� ����, ��������� ���������� � ����.
	int id;
	//������ �������� ����� �� ��������������� ������������.
	int up, down, left, right, up_left, up_right, down_left, down_right;
	//������ �������, � ������ ������ ����������� � �������
	list<player*> players;

public:
	//���� ���� ���, ����� ���������� -1
	node(int _id, int _up = -1, int _down = -1, int _left = -1, int _right = -1,
		int _up_left = -1, int _up_right = -1, int _down_left = -1, int _down_right = -1);
	~node();

	//��� ������ ���������� -1, ���� ���� ���
	int get_id();
	int get_up();
	int get_down();
	int get_left();
	int get_right();
	int get_up_left();
	int get_up_right();
	int get_down_left();
	int get_down_right();

	//��������� ������ � �������� ����
	void add_player(player* _player);
	//������� ������ �� ��������� ���� �� ��� �������������� � ���������� �� ���� ������
	player* remove_player(int uid);
	//���������, ���� �� ������ � ������� � ���������� true, ���� ����
	bool contains_players();
};

class game_map
{
	//����� �����
	int id;
	//������ �����
	vector<node*> nodes;
	//����� ����� �� ����� ������ �����
	int level_size;

public:
	game_map(int _id, int _level_size);
	~game_map();

	int get_id();
	int get_level_size();
	//��������� ���� � �����
	void add_node(node* _node);
	//�������� ���� �� �������, ��������� ���������� � ����
	node* get_node(int index);
	//�������� ����� ����� � �����
	int nodes_count();
	
	//���������������� ����� �� �� ������; ��������, ��������� �����
	void initialize(int id);

	virtual void output_map();
	//���������� ���� �� ���� ����� ������� (0-4); ������ - 11 �����
protected:
	virtual void output_node_row(node* _node, node* _left, node* _right, int row);
};