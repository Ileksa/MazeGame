#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include <list>
#include <vector>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <winsock2.h>
using namespace std;


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
	//���������� ����� ������� � ����
	int get_players_count();
	//���������� ������ �� ��������� �������
	player* get_player(int index);
};