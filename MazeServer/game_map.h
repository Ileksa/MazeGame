#pragma once
#include <list>
#include <vector>
using namespace std;

//������������ ������.
class player
{
	//���������� ������������� ������ �� �������.
	int uid;
public:
	player(int _uid);
	~player();

	int get_uid();
	void set_uid(int _uid);

	
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

	//��������� ������ � �������� ����
	void add_player(player* _player);
	//������� ������ �� ��������� ���� �� ��� �������������� � ���������� �� ���� ������
	player* remove_player(int uid);
};

class game_map
{
	//����� �����
	int id;
	//������ �����
	vector<node*> nodes;

public:
	game_map(int _id);
	~game_map();

	int get_id();
	//��������� ���� � �����
	void add_node(node* _node);
	//�������� ���� �� �������
	node* get_node(int index);
	//�������� ����� ����� � �����
	int node_count();
};
